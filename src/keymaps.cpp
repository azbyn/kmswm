#include <stdio.h>
#include <assert.h>

#include <stdexcept>

#include "keymaps.h"

namespace Kmswm {
KeymapStack::KeymapStack() {
}
KeymapStack::KeymapStack(std::vector<Keymap> keymaps, KeymapIndex defaultKeymap/* = 0*/) :
		keymaps(keymaps) {

	static_assert(KEYMAP_STACK_SIZE >= 2);// KEYMAP_STACK_SIZE must be greater than 2
	nodes = new Node[KEYMAP_STACK_SIZE];
	nodes[0].prev = nullptr;
	nodes[0].next = nodes + 1;
	
	for (int i = 1; i < KEYMAP_STACK_SIZE - 1; ++i) {
		nodes[i].prev = nodes + i - 1;
		nodes[i].next = nodes + i + 1;
		nodes[i].used = false;
	}
	nodes[KEYMAP_STACK_SIZE-1].prev = nodes + KEYMAP_STACK_SIZE - 2;
	nodes[KEYMAP_STACK_SIZE-1].next = nullptr;

	node = nodes;
	node->value = defaultKeymap;
	node->used = true;
}
KeymapStack::~KeymapStack() {
	delete[] nodes;
}

void KeymapStack::Pop() {
	if (!node->prev) return;
	node->used = false;
	node = node->prev;
}
void KeymapStack::Pop(KeymapIndex index) {
	if (node->value == index)
		Pop();
}

void KeymapStack::Push(KeymapIndex km) {
	if (!node->next)
		throw std::out_of_range("KeymapStack Out of range, you might want to increase KEYMAP_STACK_SIZE");
	node = node->next;
	node->used = true;
	node->value = km;
}
#define _PRESS_KEY_IMPL(_val) \
	Node *n = node; \
	KeyAction fun; \
	while (n) { \
		fun = keymaps[n->value][key]. _val; \
		if (fun) { \
			fun(this); \
			return; \
		} \
		n = n->prev; \
	}


void KeymapStack::PressKey(Key key) {
	_PRESS_KEY_IMPL(down);
}
void KeymapStack::HoldKey(Key key) {
	_PRESS_KEY_IMPL(up);
}
void KeymapStack::ReleaseKey(Key key) {
	_PRESS_KEY_IMPL(hold);
}

}//namespace Kmswm

