#include <linux/input-event-codes.h>
#include <stdio.h>

#include "keymaps.h"
#include "input_handler.h"


namespace Kmswm {

#define SET_FUNC(_key) keymaps[0][_key] = [](const KeymapStack *)
KeymapStack InputHandler::GenenerateKeymapStack() {
	std::vector<Keymap> keymaps;
	keymaps.push_back(Keymap());//size_t normal = 0;

	SET_FUNC(KEY_RESERVED) { printf("reserved"); };
	SET_FUNC(KEY_ESC) { printf("Esc"); };
	SET_FUNC(KEY_1) { printf("1"); };
	SET_FUNC(KEY_2) { printf("2"); };
	SET_FUNC(KEY_3) { printf("3"); };
	SET_FUNC(KEY_4) { printf("4"); };
	SET_FUNC(KEY_5) { printf("5"); };
	SET_FUNC(KEY_6) { printf("6"); };
	SET_FUNC(KEY_7) { printf("7"); };
	SET_FUNC(KEY_8) { printf("8"); };
	SET_FUNC(KEY_9) { printf("9"); };
	SET_FUNC(KEY_0) { printf("0"); };
	SET_FUNC(KEY_MINUS) { printf("minus"); };
	SET_FUNC(KEY_EQUAL) { printf("equal"); };
	SET_FUNC(KEY_BACKSPACE) { printf("backspace"); };
	SET_FUNC(KEY_TAB) { printf("tab"); };
	SET_FUNC(KEY_Q) { printf("Q"); };
	SET_FUNC(KEY_W) { printf("W"); };
	SET_FUNC(KEY_E) { printf("E"); };
	SET_FUNC(KEY_R) { printf("R"); };
	SET_FUNC(KEY_T) { printf("T"); };
	SET_FUNC(KEY_Y) { printf("Y"); };
	SET_FUNC(KEY_U) { printf("U"); };
	SET_FUNC(KEY_I) { printf("I"); };
	SET_FUNC(KEY_O) { printf("O"); };
	SET_FUNC(KEY_P) { printf("P"); };
	SET_FUNC(KEY_LEFTBRACE) { printf("["); };
	SET_FUNC(KEY_RIGHTBRACE) { printf("]"); };
	SET_FUNC(KEY_ENTER) { printf("enter"); };
	SET_FUNC(KEY_LEFTCTRL) { printf("l-ctrl"); };
	SET_FUNC(KEY_A) { printf("A"); };
	SET_FUNC(KEY_S) { printf("S"); };
	SET_FUNC(KEY_D) { printf("D"); };
	SET_FUNC(KEY_F) { printf("F"); };
	SET_FUNC(KEY_G) { printf("G"); };
	SET_FUNC(KEY_H) { printf("H"); };
	SET_FUNC(KEY_J) { printf("J"); };
	SET_FUNC(KEY_K) { printf("K"); };
	SET_FUNC(KEY_L) { printf("L"); };
	SET_FUNC(KEY_SEMICOLON) { printf(";"); };
	SET_FUNC(KEY_APOSTROPHE) { printf("'"); };
	SET_FUNC(KEY_GRAVE) { printf("~"); };
	SET_FUNC(KEY_LEFTSHIFT) { printf("l-shift"); };
	SET_FUNC(KEY_BACKSLASH) { printf("\\"); };
	SET_FUNC(KEY_Z) { printf("Z"); };
	SET_FUNC(KEY_X) { printf("X"); };
	SET_FUNC(KEY_C) { printf("C"); };
	SET_FUNC(KEY_V) { printf("V"); };
	SET_FUNC(KEY_B) { printf("B"); };
	SET_FUNC(KEY_N) { printf("N"); };
	SET_FUNC(KEY_M) { printf("M"); };
	SET_FUNC(KEY_COMMA) { printf(","); };
	SET_FUNC(KEY_DOT) { printf("."); };
	SET_FUNC(KEY_SLASH) { printf("/"); };
	SET_FUNC(KEY_RIGHTSHIFT) { printf("r-shift"); };
	SET_FUNC(KEY_KPASTERISK) { printf("kp-asterix"); };
	SET_FUNC(KEY_LEFTALT) { printf("l-alt"); };
	SET_FUNC(KEY_SPACE) { printf("space"); };
	SET_FUNC(KEY_CAPSLOCK) { printf("caps"); };
	SET_FUNC(KEY_F1) { printf("F1"); };
	SET_FUNC(KEY_F2) { printf("F2"); };
	SET_FUNC(KEY_F3) { printf("F3"); };
	SET_FUNC(KEY_F4) { printf("F4"); };
	SET_FUNC(KEY_F5) { printf("F5"); };
	SET_FUNC(KEY_F6) { printf("F6"); };
	SET_FUNC(KEY_F7) { printf("F7"); };
	SET_FUNC(KEY_F8) { printf("F8"); };
	SET_FUNC(KEY_F9) { printf("F9"); };
	SET_FUNC(KEY_F10) { printf("F10"); };
	SET_FUNC(KEY_NUMLOCK) { printf("num-lock"); };
	SET_FUNC(KEY_SCROLLLOCK) { printf("scroll-lock"); };
	SET_FUNC(KEY_KP7) { printf("kp-7"); };
	SET_FUNC(KEY_KP8) { printf("kp-8"); };
	SET_FUNC(KEY_KP9) { printf("kp-9"); };
	SET_FUNC(KEY_KPMINUS) { printf("kp-minus"); };
	SET_FUNC(KEY_KP4) { printf("kp-4"); };
	SET_FUNC(KEY_KP5) { printf("kp-5"); };
	SET_FUNC(KEY_KP6) { printf("kp-6"); };
	SET_FUNC(KEY_KPPLUS) { printf("kp-plus"); };
	SET_FUNC(KEY_KP1) { printf("kp-1"); };
	SET_FUNC(KEY_KP2) { printf("kp-2"); };
	SET_FUNC(KEY_KP3) { printf("kp-3"); };
	SET_FUNC(KEY_KP0) { printf("kp-0"); };
	SET_FUNC(KEY_KPDOT) { printf("kp-dot"); };

	SET_FUNC(KEY_102ND) { printf("102ND"); };
	SET_FUNC(KEY_F11) { printf("F11"); };
	SET_FUNC(KEY_F12) { printf("F12"); };

	SET_FUNC(KEY_KPENTER) { printf("kp-enter"); };
	SET_FUNC(KEY_RIGHTCTRL) { printf("r-ctrl"); };
	SET_FUNC(KEY_KPSLASH) { printf("kp-slash"); };
	SET_FUNC(KEY_SYSRQ) { printf("sys-rq"); };
	SET_FUNC(KEY_RIGHTALT) { printf("r-alt"); };
	SET_FUNC(KEY_LINEFEED) { printf("lf"); };
	SET_FUNC(KEY_HOME) { printf("home"); };
	SET_FUNC(KEY_UP) { printf("up"); };
	SET_FUNC(KEY_PAGEUP) { printf("page-up"); };
	SET_FUNC(KEY_LEFT) { printf("left"); };
	SET_FUNC(KEY_RIGHT) { printf("right"); };
	SET_FUNC(KEY_END) { printf("end"); };
	SET_FUNC(KEY_DOWN) { printf("down"); };
	SET_FUNC(KEY_PAGEDOWN) { printf("page-down"); };
	SET_FUNC(KEY_INSERT) { printf("insert"); };
	SET_FUNC(KEY_DELETE) { printf("delete"); };
	SET_FUNC(KEY_KPEQUAL) { printf("kp-equal"); };
	SET_FUNC(KEY_KPPLUSMINUS) { printf("kp-plus-minus"); };
	SET_FUNC(KEY_PAUSE) { printf("pause"); };

	SET_FUNC(KEY_LEFTMETA) { printf("l-meta"); };
	SET_FUNC(KEY_RIGHTMETA) { printf("r-meta"); };

	SET_FUNC(KEY_COMPOSE) { printf("compose"); };
	
	printf("end-\n");
	return KeymapStack(keymaps);

}
} // namespace Kmswm

