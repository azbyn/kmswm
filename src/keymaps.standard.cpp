#include <linux/input-event-codes.h>
#include <stdio.h>

#include "misc.h"
#include "keymaps.h"

#define SEND_CHR(_char) putchar(_char)
#define SEND_STR(_str) fputs(_str, stdout)

#define SET_FUNC(_ks, _key, _ev, _km, _impl) \
	keymaps[_km][_key][_ev] = [](KeymapStack *_ks) _impl

#define SET_FUNC_N(_ks, _keys, _ev, _km, _impl) { \
		constexpr auto lambda_ = [](KeymapStack *_ks) _impl; \
		for (auto& key_ : _keys) { \
			keymaps[_km][key_][_ev] = lambda_; \
		} \
	}

#define SET_ALWAYS(_ks, _key, _ev, _impl) { \
		constexpr auto lambda_ = [](KeymapStack *_ks) _impl; \
		for (KeymapIndex km_ = 0; km_ < MAX_KM; ++km_) { \
			keymaps[km_][_key][_ev]= lambda_; \
		} \
	}


#define SET_DOWN(_ks, _key, _km, _impl) \
	SET_FUNC(_ks, _key, KEY_VAL_DOWN, _km, _impl)

#define SET_NORMAL(_ks, _key, _impl) \
	SET_FUNC(_ks, _key, KEY_VAL_UP,   KM_NORMAL, _impl); \
	SET_FUNC(_ks, _key, KEY_VAL_HOLD, KM_NORMAL, _impl);

#define SET_CAPS(_ks, _key, _nrm, _caps) \
	SET_FUNC(_ks, _key, KEY_VAL_DOWN, KM_NORMAL, _nrm); \
	SET_FUNC(_ks, _key, KEY_VAL_HOLD, KM_NORMAL, _nrm); \
	SET_FUNC(_ks, _key, KEY_VAL_DOWN, KM_ANTICAPS, _nrm); \
	SET_FUNC(_ks, _key, KEY_VAL_HOLD, KM_ANTICAPS, _nrm); \
	SET_FUNC(_ks, _key, KEY_VAL_DOWN, KM_CAPS, _caps); \
	SET_FUNC(_ks, _key, KEY_VAL_HOLD, KM_CAPS, _caps);

#define SET_SHIFT(_ks, _key, _nrm, _shift) \
	SET_FUNC(_ks, _key, KEY_VAL_DOWN, KM_NORMAL, _nrm); \
	SET_FUNC(_ks, _key, KEY_VAL_HOLD, KM_NORMAL, _nrm); \
	SET_FUNC(_ks, _key, KEY_VAL_DOWN, KM_SHIFT, _shift); \
	SET_FUNC(_ks, _key, KEY_VAL_HOLD, KM_SHIFT, _shift);

namespace Kmswm {
void LogKey(KeymapIndex km) {
	if (km == KM_ANTICAPS) { printf("[ANTI] %x", km); return; }
	if (km == KM_CAPS)     { printf("[CAPS] %x", km); return; }
	printf("[%c%c%c%c] %x",
			km & KM_SHIFT?'S':' ',
			km & KM_CTRL ?'C':' ',
			km & KM_ALT  ?'A':' ',
			km & KM_SUPER?'H':' ',
			km
		);
}
void KeymapStack::Logger() {
#if 0
	printf("\033[0;0H");
	
	printf("<%c%c%c%c>\n",
			GetModifier(MOD_SHIFT)?'S':' ',
			GetModifier(MOD_CTRL )?'C':' ',
			GetModifier(MOD_ALT  )?'A':' ',
			GetModifier(MOD_SUPER)?'H':' ');

	for (int i = 0; i < KEYMAP_STACK_SIZE; ++i) {
		LogKey(rootNode[i].value);
		printf("%s\n", rootNode+i == node?"<-":"  ");
	}
	printf("S %d\n", modifiers[MOD_SHIFT]);
	printf("C %d\n", modifiers[MOD_CTRL ]);
	printf("A %d\n", modifiers[MOD_ALT  ]);
	printf("H %d\n", modifiers[MOD_SUPER]);
#endif
}
KeymapStack KeymapStack::Generate(class InputHandler *ih) {
	std::vector<Keymap> keymaps(MAX_KM);
	std::vector<KeymapIndex> initialKeymaps = {/*NUMLOCK*/};
	std::array<std::vector<Key>, MAX_MOD> modifierKeys = {};
	modifierKeys[MOD_SHIFT] = { KEY_LEFTSHIFT, KEY_RIGHTSHIFT };
	modifierKeys[MOD_ALT  ] = { KEY_LEFTALT,   KEY_RIGHTALT   };
	modifierKeys[MOD_CTRL ] = { KEY_LEFTCTRL,  KEY_RIGHTCTRL  };
	modifierKeys[MOD_SUPER] = { KEY_LEFTMETA,  KEY_RIGHTMETA  };


	SET_ALWAYS(ks, KEY_ESC, KEY_VAL_DOWN, { ks->Exit(); });



	//Caps
	SET_FUNC(ks, KEY_CAPSLOCK, KEY_VAL_DOWN, KM_NORMAL, {
			ks->SetLed(LED_CAPSL, true);
			ks->Push(KM_CAPS);
			//we do this twice to override the leds being set by the console
			ks->SetLed(LED_CAPSL, true);
		});
	SET_FUNC(ks, KEY_CAPSLOCK, KEY_VAL_DOWN, KM_CAPS, {
			ks->SetLed(LED_CAPSL, false);
			ks->Pop(KM_CAPS);
			ks->SetLed(LED_CAPSL, false);
		});

	//Shift
	SET_FUNC_N(ks, modifierKeys[MOD_SHIFT], KEY_VAL_DOWN, KM_NORMAL, {
			printf("<shift>"); ks->Push(KM_CAPS); ks->Push(KM_SHIFT);
		});
	// if we press multiple shift keys
	SET_FUNC_N(ks, modifierKeys[MOD_SHIFT], KEY_VAL_DOWN, KM_SHIFT, {
			ks->Push(KM_SHIFT);
		});
	
	SET_FUNC_N(ks, modifierKeys[MOD_SHIFT], KEY_VAL_DOWN, KM_CAPS, {
			printf("<c-shift>"); ks->Push(KM_ANTICAPS); ks->Push(KM_SHIFT);
		});
	SET_FUNC_N(ks, modifierKeys[MOD_SHIFT], KEY_VAL_UP, KM_SHIFT, {
			ks->Pop(KM_SHIFT);
			auto top = ks->Top();
			if (top == KM_CAPS || top == KM_ANTICAPS)
				ks->Pop();
		});

	SET_DOWN(, KEY_PAGEUP,   KM_ALT, { Spawn("amixer", "sset", "Master", "5%+"); });
	SET_DOWN(, KEY_PAGEDOWN, KM_ALT, { Spawn("amixer", "sset", "Master", "5%-"); });
	SET_DOWN(, KEY_PAUSE,    KM_ALT, { Spawn("amixer", "sset", "Master", "toggle"); });
	/*SET_DOWN(, KEY_PAUSE,    KM_ALT, { SpawnCallback(
				[] (std::string s, int) { printf("'%s'", s.c_str()); }, "sh", "-c", "echo 'hello there'; sleep 1; echo 'done' "); });
	*/
	SET_NORMAL(, KEY_SPACE, { SEND_CHR(' '); });
	SET_NORMAL(, KEY_ENTER, { SEND_CHR('\n'); });
	SET_NORMAL(, KEY_TAB,   { SEND_CHR('\t'); });


	SET_CAPS(, KEY_Q, { SEND_CHR('q'); }, { SEND_CHR('Q'); });
	SET_CAPS(, KEY_W, { SEND_CHR('w'); }, { SEND_CHR('W'); });
	SET_CAPS(, KEY_E, { SEND_CHR('e'); }, { SEND_CHR('E'); });
	SET_CAPS(, KEY_R, { SEND_CHR('r'); }, { SEND_CHR('R'); });
	SET_CAPS(, KEY_T, { SEND_CHR('t'); }, { SEND_CHR('T'); });
	SET_CAPS(, KEY_Y, { SEND_CHR('y'); }, { SEND_CHR('Y'); });
	SET_CAPS(, KEY_U, { SEND_CHR('u'); }, { SEND_CHR('U'); });
	SET_CAPS(, KEY_I, { SEND_CHR('i'); }, { SEND_CHR('I'); });
	SET_CAPS(, KEY_O, { SEND_CHR('o'); }, { SEND_CHR('O'); });
	SET_CAPS(, KEY_P, { SEND_CHR('p'); }, { SEND_CHR('P'); });
	SET_CAPS(, KEY_A, { SEND_CHR('a'); }, { SEND_CHR('A'); });
	SET_CAPS(, KEY_S, { SEND_CHR('s'); }, { SEND_CHR('S'); });
	SET_CAPS(, KEY_D, { SEND_CHR('d'); }, { SEND_CHR('D'); });
	SET_CAPS(, KEY_F, { SEND_CHR('f'); }, { SEND_CHR('F'); });
	SET_CAPS(, KEY_G, { SEND_CHR('g'); }, { SEND_CHR('G'); });
	SET_CAPS(, KEY_H, { SEND_CHR('h'); }, { SEND_CHR('H'); });
	SET_CAPS(, KEY_J, { SEND_CHR('j'); }, { SEND_CHR('J'); });
	SET_CAPS(, KEY_K, { SEND_CHR('k'); }, { SEND_CHR('K'); });
	SET_CAPS(, KEY_L, { SEND_CHR('l'); }, { SEND_CHR('L'); });
	SET_CAPS(, KEY_Z, { SEND_CHR('z'); }, { SEND_CHR('Z'); });
	SET_CAPS(, KEY_X, { SEND_CHR('x'); }, { SEND_CHR('X'); });
	SET_CAPS(, KEY_C, { SEND_CHR('c'); }, { SEND_CHR('C'); });
	SET_CAPS(, KEY_V, { SEND_CHR('v'); }, { SEND_CHR('V'); });
	SET_CAPS(, KEY_B, { SEND_CHR('b'); }, { SEND_CHR('B'); });
	SET_CAPS(, KEY_N, { SEND_CHR('n'); }, { SEND_CHR('N'); });
	SET_CAPS(, KEY_M, { SEND_CHR('m'); }, { SEND_CHR('M'); });

	SET_SHIFT(, KEY_1, { SEND_CHR('1'); }, { SEND_CHR('!'); });
	SET_SHIFT(, KEY_2, { SEND_CHR('2'); }, { SEND_CHR('@'); });
	SET_SHIFT(, KEY_3, { SEND_CHR('3'); }, { SEND_CHR('#'); });
	SET_SHIFT(, KEY_4, { SEND_CHR('4'); }, { SEND_CHR('$'); });
	SET_SHIFT(, KEY_5, { SEND_CHR('5'); }, { SEND_CHR('%'); });
	SET_SHIFT(, KEY_6, { SEND_CHR('6'); }, { SEND_CHR('^'); });
	SET_SHIFT(, KEY_7, { SEND_CHR('7'); }, { SEND_CHR('&'); });
	SET_SHIFT(, KEY_8, { SEND_CHR('8'); }, { SEND_CHR('*'); });
	SET_SHIFT(, KEY_9, { SEND_CHR('9'); }, { SEND_CHR('('); });
	SET_SHIFT(, KEY_0, { SEND_CHR('0'); }, { SEND_CHR(')'); });
	SET_SHIFT(, KEY_MINUS,      { SEND_CHR('-'); }, { SEND_CHR('_'); });
	SET_SHIFT(, KEY_EQUAL,      { SEND_CHR('='); }, { SEND_CHR('+'); });
	SET_SHIFT(, KEY_LEFTBRACE,  { SEND_CHR('['); }, { SEND_CHR('{'); });
	SET_SHIFT(, KEY_RIGHTBRACE, { SEND_CHR(']'); }, { SEND_CHR('}'); });
	SET_SHIFT(, KEY_SEMICOLON,  { SEND_CHR(';'); }, { SEND_CHR(':'); });
	SET_SHIFT(, KEY_APOSTROPHE, { SEND_CHR('\''); }, { SEND_CHR('"'); });
	SET_SHIFT(, KEY_BACKSLASH,  { SEND_CHR('\\'); }, { SEND_CHR('|'); });
	SET_SHIFT(, KEY_COMMA,      { SEND_CHR(','); }, { SEND_CHR('<'); });
	SET_SHIFT(, KEY_DOT,        { SEND_CHR('.'); }, { SEND_CHR('>'); });
	SET_SHIFT(, KEY_SLASH,      { SEND_CHR('/'); }, { SEND_CHR('?'); });
	SET_SHIFT(, KEY_GRAVE,      { SEND_CHR('`'); }, { SEND_CHR('~'); });



	SET_NORMAL(, KEY_KPASTERISK, { SEND_CHR('*'); });
	SET_NORMAL(, KEY_KPMINUS,    { SEND_CHR('-'); });
	SET_NORMAL(, KEY_KPPLUS,     { SEND_CHR('+'); });
	SET_NORMAL(, KEY_KPMINUS,    { SEND_CHR('-'); });
	SET_NORMAL(, KEY_KPDOT,      { SEND_CHR('.'); });
	SET_NORMAL(, KEY_KPSLASH,    { SEND_CHR('/'); });
	SET_NORMAL(, KEY_KPENTER,    { SEND_CHR('\n'); });

	SET_NORMAL(, KEY_KP7, { SEND_CHR('7'); });
	SET_NORMAL(, KEY_KP8, { SEND_CHR('8'); });
	SET_NORMAL(, KEY_KP9, { SEND_CHR('9'); });
	SET_NORMAL(, KEY_KP4, { SEND_CHR('4'); });
	SET_NORMAL(, KEY_KP5, { SEND_CHR('5'); });
	SET_NORMAL(, KEY_KP6, { SEND_CHR('6'); });
	SET_NORMAL(, KEY_KP1, { SEND_CHR('1'); });
	SET_NORMAL(, KEY_KP2, { SEND_CHR('2'); });
	SET_NORMAL(, KEY_KP3, { SEND_CHR('3'); });
	SET_NORMAL(, KEY_KP0, { SEND_CHR('0'); });

	//not implemented
	/*
	SET_NORMAL(, KEY_RESERVED,  { SEND_STR("<reserved>"); });
	SET_NORMAL(, KEY_BACKSPACE, { SEND_STR("<bs>"); });
	
	SETFUNC_(NORMAL, KEY_LEFTMETA) { printf("<l-meta>"); };
	SETFUNC_(NORMAL, KEY_RIGHTMETA) { printf("<r-meta>"); };

	SETFUNC_(NORMAL, KEY_F1) { printf("F1"); };
	SETFUNC_(NORMAL, KEY_F2) { printf("F2"); };
	SETFUNC_(NORMAL, KEY_F3) { printf("F3"); };
	SETFUNC_(NORMAL, KEY_F4) { printf("F4"); };
	SETFUNC_(NORMAL, KEY_F5) { printf("F5"); };
	SETFUNC_(NORMAL, KEY_F6) { printf("F6"); };
	SETFUNC_(NORMAL, KEY_F7) { printf("F7"); };
	SETFUNC_(NORMAL, KEY_F8) { printf("F8"); };
	SETFUNC_(NORMAL, KEY_F9) { printf("F9"); };
	SETFUNC_(NORMAL, KEY_F10) { printf("F10"); };
	SETFUNC_(NORMAL, KEY_NUMLOCK) { printf("num-lock"); };
	SETFUNC_(NORMAL, KEY_SCROLLLOCK) { printf("scroll-lock"); };

	SETFUNC_(NORMAL, KEY_102ND) { printf("102ND"); };
	SETFUNC_(NORMAL, KEY_F11) { printf("F11"); };
	SETFUNC_(NORMAL, KEY_F12) { printf("F12"); };

	SETFUNC_(NORMAL, KEY_SYSRQ) { printf("sys-rq"); };
	SETFUNC_(NORMAL, KEY_HOME) { printf("home"); };
	SETFUNC_(NORMAL, KEY_UP) { printf("up"); };
	SETFUNC_(NORMAL, KEY_PAGEUP) { printf("page-up"); };
	SETFUNC_(NORMAL, KEY_LEFT) { printf("left"); };
	SETFUNC_(NORMAL, KEY_RIGHT) { printf("right"); };
	SETFUNC_(NORMAL, KEY_END) { printf("end"); };
	SETFUNC_(NORMAL, KEY_DOWN) { printf("down"); };
	SETFUNC_(NORMAL, KEY_PAGEDOWN) { printf("page-down"); };
	SETFUNC_(NORMAL, KEY_INSERT) { printf("insert"); };
	SETFUNC_(NORMAL, KEY_DELETE) { printf("delete"); };
	SETFUNC_(NORMAL, KEY_PAUSE) { printf("pause"); };

	SETFUNC_(NORMAL, KEY_COMPOSE) { printf("compose"); };
	*/
	return KeymapStack(ih, keymaps, initialKeymaps, modifierKeys);
}

}// namespace Kmswm
