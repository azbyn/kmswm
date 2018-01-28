#include <linux/input-event-codes.h>
#include <stdio.h>

#include "keymaps.h"
#include "input_handler.h"


namespace Kmswm {

enum Keymaps : KeymapIndex {
	NORMAL,
	CAPS,
	ANTICAPS,
	SHIFT,

	MAX_KM,
};

#define SEND_CHR(_char) putchar(_char)
#define SEND_STR(_str) fputs(stdout, _str)

#define SETFUNC_(_km, _key) keymaps[_km][_key].down = [](KeymapStack *)
#define SET_FUNC(_ks, _key, _ev, _km, _impl) \
	keymaps[_km][_key]. _ev = [](KeymapStack *_ks) _impl

#define SET_DOWN(_ks, _key, _km, _impl) \
	SET_FUNC(_ks, _key, down, _km, _impl)

#define SET_NORMAL(_ks, _key, _impl) \
	SET_FUNC(_ks, _key, down, NORMAL, _impl); \
	SET_FUNC(_ks, _key, hold, NORMAL, _impl);


#define SET_CAPS(_ks, _key, _nrm, _caps) \
	SET_FUNC(_ks, _key, down, NORMAL, _nrm); \
	SET_FUNC(_ks, _key, hold, NORMAL, _nrm); \
	SET_FUNC(_ks, _key, down, ANTICAPS, _nrm); \
	SET_FUNC(_ks, _key, hold, ANTICAPS, _nrm); \
	SET_FUNC(_ks, _key, down, CAPS, _caps); \
	SET_FUNC(_ks, _key, hold, CAPS, _caps);

#define SET_SHIFT(_ks, _key, _nrm, _shift) \
	SET_FUNC(_ks, _key, down, NORMAL, _nrm); \
	SET_FUNC(_ks, _key, hold, NORMAL, _nrm); \
	SET_FUNC(_ks, _key, down, SHIFT, _shift); \
	SET_FUNC(_ks, _key, hold, SHIFT, _shift);



#define SET_FUNC2(_ks, _key1, _key2, _ev, _km, _impl) \
	SET_FUNC(_ks, _key1, _ev, _km, _impl); \
	SET_FUNC(_ks, _key2, _ev, _km, _impl)


KeymapStack InputHandler::GenenerateKeymapStack() {
	std::vector<Keymap> keymaps;
	for (KeymapIndex i = 0; i < MAX_KM; ++i)
		keymaps.push_back(Keymap());
	
	SET_FUNC(ks, KEY_CAPSLOCK, down, NORMAL, {
			ks->Push(CAPS);
		});
	SET_FUNC(ks, KEY_CAPSLOCK, down, CAPS, {
			ks->Pop(CAPS);
		});

	SET_FUNC2(ks, KEY_RIGHTSHIFT, KEY_LEFTSHIFT, down, NORMAL, {
			ks->Push(CAPS); ks->Push(SHIFT);
		});
	SET_FUNC2(ks, KEY_RIGHTSHIFT, KEY_LEFTSHIFT, down, CAPS, {
			ks->Push(ANTICAPS); ks->Push(SHIFT);
		});
	SET_FUNC2(ks, KEY_RIGHTSHIFT, KEY_LEFTSHIFT, up, SHIFT, {
			ks->Pop(SHIFT);
			auto top = ks->Top();
			if (top == CAPS || top == ANTICAPS)
				ks->Pop();
		});
	SET_FUNC2(ks, KEY_RIGHTSHIFT, KEY_LEFTSHIFT, down, SHIFT, {
			ks->Push(SHIFT);
		});

	SET_NORMAL(, KEY_SPACE, { SEND_CHR(' '); });
	SET_NORMAL(, KEY_ENTER, { SEND_CHR('\n'); });
	SET_NORMAL(, KEY_TAB,   { SEND_CHR('\t'); });

	SET_DOWN(ks, KEY_ESC, NORMAL, { ks->Exit(); });

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

	//not implemented

	SETFUNC_(NORMAL, KEY_RESERVED) { printf("reserved"); };
	SETFUNC_(NORMAL, KEY_BACKSPACE) { printf("<bs>"); };


	SETFUNC_(NORMAL, KEY_LEFTCTRL) { printf("l-ctrl"); };

	SETFUNC_(NORMAL, KEY_KPASTERISK) { printf("kp-asterix"); };
	SETFUNC_(NORMAL, KEY_LEFTALT) { printf("l-alt"); };
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
	SETFUNC_(NORMAL, KEY_KP7) { printf("kp-7"); };
	SETFUNC_(NORMAL, KEY_KP8) { printf("kp-8"); };
	SETFUNC_(NORMAL, KEY_KP9) { printf("kp-9"); };
	SETFUNC_(NORMAL, KEY_KPMINUS) { printf("kp-minus"); };
	SETFUNC_(NORMAL, KEY_KP4) { printf("kp-4"); };
	SETFUNC_(NORMAL, KEY_KP5) { printf("kp-5"); };
	SETFUNC_(NORMAL, KEY_KP6) { printf("kp-6"); };
	SETFUNC_(NORMAL, KEY_KPPLUS) { printf("kp-plus"); };
	SETFUNC_(NORMAL, KEY_KP1) { printf("kp-1"); };
	SETFUNC_(NORMAL, KEY_KP2) { printf("kp-2"); };
	SETFUNC_(NORMAL, KEY_KP3) { printf("kp-3"); };
	SETFUNC_(NORMAL, KEY_KP0) { printf("kp-0"); };
	SETFUNC_(NORMAL, KEY_KPDOT) { printf("kp-dot"); };

	SETFUNC_(NORMAL, KEY_102ND) { printf("102ND"); };
	SETFUNC_(NORMAL, KEY_F11) { printf("F11"); };
	SETFUNC_(NORMAL, KEY_F12) { printf("F12"); };

	SETFUNC_(NORMAL, KEY_KPENTER) { printf("kp-enter"); };
	SETFUNC_(NORMAL, KEY_RIGHTCTRL) { printf("r-ctrl"); };
	SETFUNC_(NORMAL, KEY_KPSLASH) { printf("kp-slash"); };
	SETFUNC_(NORMAL, KEY_SYSRQ) { printf("sys-rq"); };
	SETFUNC_(NORMAL, KEY_RIGHTALT) { printf("r-alt"); };
	SETFUNC_(NORMAL, KEY_LINEFEED) { printf("lf"); };
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
	SETFUNC_(NORMAL, KEY_KPEQUAL) { printf("kp-equal"); };
	SETFUNC_(NORMAL, KEY_KPPLUSMINUS) { printf("kp-plus-minus"); };
	SETFUNC_(NORMAL, KEY_PAUSE) { printf("pause"); };

	SETFUNC_(NORMAL, KEY_LEFTMETA) { printf("l-meta"); };
	SETFUNC_(NORMAL, KEY_RIGHTMETA) { printf("r-meta"); };

	SETFUNC_(NORMAL, KEY_COMPOSE) { printf("compose"); };
	
	return KeymapStack([this] { Stop(); },keymaps);

}
} // namespace Kmswm

