#include <stdio.h>
#include <assert.h>

#include "keymaps.h"



namespace Kmswm {
Keymap::Keymap() {
}
Keymap::Keymap(const Keymap& obj) :
	data(obj.data) {
}
Keymap::~Keymap() {
}
const KeyAction& Keymap::operator[] (Key key) const {
	assert(key < KEYMAP_SIZE); //you might want to increase KEYMAP_SIZE
	return this->data[key];
}
KeyAction& Keymap::operator[] (Key key) {
	assert(key < KEYMAP_SIZE); //you might want to increase KEYMAP_SIZE
	return this->data[key];
}

/*
struct Node {
	Node *next;
	bool used;
	Keymap value;
};
Node *stack;
*/
KeymapStack::Node::~Node() {
	if (next) delete next;
}
KeymapStack::KeymapStack() {
}
KeymapStack::KeymapStack(std::vector<Keymap> keymaps, size_t defaultKeymap/* = 0*/) :
		keymaps(keymaps) {
	Node **n = &root;
	Node *prev = nullptr;
	for (int i = 0; i < KEYMAP_STACK_SIZE; ++i) {
		*n = new Node();
		(*n)->prev = prev;
		prev = *n;
		n = &(prev->next);
	}
	*n = nullptr;
	root->value = keymaps[defaultKeymap];
	root->used = true;
	node = root;
}
KeymapStack::~KeymapStack() {
	delete root;
}
void KeymapStack::Pop() {
	if (!node->prev) return;
	node->used = false;
	node = node->prev;
}
void KeymapStack::Push(const Keymap& keymap) {
	node = node->next;
	node->used = true;
	node->value = keymap;
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

