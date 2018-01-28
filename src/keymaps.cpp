#include <stdio.h>
#include <assert.h>

#include <stdexcept>

#include "keymaps.h"

namespace Kmswm {
KeymapStack::KeymapStack() {
}
KeymapStack::KeymapStack(std::vector<Keymap> keymaps, size_t defaultKeymap/* = 0*/) :
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
	node->value = keymaps[defaultKeymap];
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
void KeymapStack::Push(const Keymap& val) {
	if (!node->next)
		throw std::out_of_range("KeymapStack Out of range, you might want to increase KEYMAP_STACK_SIZE");
	//assert(node->next); //overflow you might want to increase KEYMAP_STACK_SIZE
	node = node->next;
	node->used = true;
	node->value = val;
}
const Keymap& KeymapStack::Top() {
	return node->value;
}
void KeymapStack::PressKey(Key key) {
	Node *n = node;
	KeyAction fun;
	while (n) {
		fun = n->value[key];
		if (fun) {
			fun(this);
			return;
		}
		n = n->prev;
	}
}
}//namespace Kmswm

