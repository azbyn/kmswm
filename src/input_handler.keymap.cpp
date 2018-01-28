#include <linux/input-event-codes.h>
#include <stdio.h>

#include "keymaps.h"
#include "input_handler.h"


namespace Kmswm {

enum Keymaps : KeymapIndex {
	NORMAL,
	CAPS,
	SHIFT,

	MAX_KM,
};

#define SETFUNC_(_km, _key) keymaps[_km][_key].down = [](KeymapStack *)
#define SET_FUNC(_ks, _key, _ev, _km, _impl) \
	keymaps[_km][_key]. _ev = [](KeymapStack *_ks) _impl

#define SET_CAPS(_ks, _key, _nrm, _caps) \
	keymaps[NORMAL][_key].down = [](KeymapStack * const _ks) _nrm; \
	keymaps[CAPS][_key].down = [](KeymapStack * const _ks) _caps; \
	keymaps[NORMAL][_key].hold = [](KeymapStack * const _ks) _nrm; \
	keymaps[CAPS][_key].hold = [](KeymapStack * const _ks) _caps; \


KeymapStack InputHandler::GenenerateKeymapStack() {
	std::vector<Keymap> keymaps;
	for (KeymapIndex i = 0; i < MAX_KM; ++i)
		keymaps.push_back(Keymap());
	
	SET_FUNC(ks, KEY_CAPSLOCK, down, NORMAL, {
			printf("<caps-on>");
			ks->Push(CAPS);
		});
	SET_FUNC(ks, KEY_CAPSLOCK, down, CAPS, {
			printf("<caps-off>");
			ks->Pop(CAPS);
		});

	
	SET_CAPS(, KEY_Q, { putchar('q'); }, { putchar('Q'); });
	SET_CAPS(, KEY_W, { putchar('w'); }, { putchar('W'); });
	SET_CAPS(, KEY_E, { putchar('e'); }, { putchar('E'); });
	SET_CAPS(, KEY_R, { putchar('r'); }, { putchar('R'); });
	SET_CAPS(, KEY_T, { putchar('t'); }, { putchar('T'); });
	SET_CAPS(, KEY_Y, { putchar('y'); }, { putchar('Y'); });
	SET_CAPS(, KEY_U, { putchar('u'); }, { putchar('U'); });
	SET_CAPS(, KEY_I, { putchar('i'); }, { putchar('I'); });
	SET_CAPS(, KEY_O, { putchar('o'); }, { putchar('O'); });
	SET_CAPS(, KEY_P, { putchar('p'); }, { putchar('P'); });
	SET_CAPS(, KEY_A, { putchar('a'); }, { putchar('A'); });
	SET_CAPS(, KEY_S, { putchar('s'); }, { putchar('S'); });
	SET_CAPS(, KEY_D, { putchar('d'); }, { putchar('D'); });
	SET_CAPS(, KEY_F, { putchar('f'); }, { putchar('F'); });
	SET_CAPS(, KEY_G, { putchar('g'); }, { putchar('G'); });
	SET_CAPS(, KEY_H, { putchar('h'); }, { putchar('H'); });
	SET_CAPS(, KEY_J, { putchar('j'); }, { putchar('J'); });
	SET_CAPS(, KEY_K, { putchar('k'); }, { putchar('K'); });
	SET_CAPS(, KEY_L, { putchar('l'); }, { putchar('L'); });
	SET_CAPS(, KEY_Z, { putchar('z'); }, { putchar('Z'); });
	SET_CAPS(, KEY_X, { putchar('x'); }, { putchar('X'); });
	SET_CAPS(, KEY_C, { putchar('c'); }, { putchar('C'); });
	SET_CAPS(, KEY_V, { putchar('v'); }, { putchar('V'); });
	SET_CAPS(, KEY_B, { putchar('b'); }, { putchar('B'); });
	SET_CAPS(, KEY_N, { putchar('n'); }, { putchar('N'); });
	SET_CAPS(, KEY_M, { putchar('m'); }, { putchar('M'); });





	//not implemented

	SETFUNC_(NORMAL, KEY_RESERVED) { printf("reserved"); };
	SETFUNC_(NORMAL, KEY_ESC) { printf("Esc"); };
	SETFUNC_(NORMAL, KEY_1) { printf("1"); };
	SETFUNC_(NORMAL, KEY_2) { printf("2"); };
	SETFUNC_(NORMAL, KEY_3) { printf("3"); };
	SETFUNC_(NORMAL, KEY_4) { printf("4"); };
	SETFUNC_(NORMAL, KEY_5) { printf("5"); };
	SETFUNC_(NORMAL, KEY_6) { printf("6"); };
	SETFUNC_(NORMAL, KEY_7) { printf("7"); };
	SETFUNC_(NORMAL, KEY_8) { printf("8"); };
	SETFUNC_(NORMAL, KEY_9) { printf("9"); };
	SETFUNC_(NORMAL, KEY_0) { printf("0"); };
	SETFUNC_(NORMAL, KEY_MINUS) { printf("minus"); };
	SETFUNC_(NORMAL, KEY_EQUAL) { printf("equal"); };
	SETFUNC_(NORMAL, KEY_BACKSPACE) { printf("backspace"); };
	SETFUNC_(NORMAL, KEY_TAB) { printf("tab"); };
	SETFUNC_(NORMAL, KEY_LEFTBRACE) { printf("["); };
	SETFUNC_(NORMAL, KEY_RIGHTBRACE) { printf("]"); };
	SETFUNC_(NORMAL, KEY_ENTER) { printf("enter"); };
	SETFUNC_(NORMAL, KEY_LEFTCTRL) { printf("l-ctrl"); };
	SETFUNC_(NORMAL, KEY_SEMICOLON) { printf(";"); };
	SETFUNC_(NORMAL, KEY_APOSTROPHE) { printf("'"); };
	SETFUNC_(NORMAL, KEY_GRAVE) { printf("~"); };
	SETFUNC_(NORMAL, KEY_LEFTSHIFT) { printf("l-shift"); };
	SETFUNC_(NORMAL, KEY_BACKSLASH) { printf("\\"); };
	SETFUNC_(NORMAL, KEY_COMMA) { printf(","); };
	SETFUNC_(NORMAL, KEY_DOT) { printf("."); };
	SETFUNC_(NORMAL, KEY_SLASH) { printf("/"); };
	SETFUNC_(NORMAL, KEY_RIGHTSHIFT) { printf("r-shift"); };
	SETFUNC_(NORMAL, KEY_KPASTERISK) { printf("kp-asterix"); };
	SETFUNC_(NORMAL, KEY_LEFTALT) { printf("l-alt"); };
	SETFUNC_(NORMAL, KEY_SPACE) { printf("space"); };
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
	
	printf("end-\n");
	return KeymapStack(keymaps);

}
} // namespace Kmswm

