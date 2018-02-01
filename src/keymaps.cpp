#include <assert.h>
#include <stdio.h>

#include <stdexcept>

#include "input_handler.h"
#include "misc.h"

#include "keymaps.h"

namespace kmswm {

KeymapStack::Node::Node(Node *prev, Node *next, KeymapIndex value, bool used) :
			prev(prev), next(next), used(used), value(value) {}

KeymapStack::KeymapStack(
		InputHandler *inputHandler,
		std::vector<Keymap> keymaps,
		std::vector<KeymapIndex> initialKeymaps,
		std::array<std::vector<Key>, MAX_MOD> _modifierKeys) :
			inputHandler(inputHandler), keymaps(keymaps) {
	static_assert(KEYMAP_STACK_SIZE >= 2);// KEYMAP_STACK_SIZE must be greater than 2
	if (initialKeymaps.size() >= KEYMAP_STACK_SIZE - 1) {
		throw new std::runtime_error(
				string_format("initialKeymaps too big (%d >= %d)",
					initialKeymaps.size(),
					KEYMAP_STACK_SIZE - 1));
	}
	// we use malloc instead of new because we don't use destructors and default constructors
	Node *nodes =  (Node *) malloc(sizeof(Node) * KEYMAP_STACK_SIZE);

	nodes[0] = Node(nullptr, &nodes[1], 0, true);
	for (size_t i = 1; i < KEYMAP_STACK_SIZE - 1; ++i) {
		auto hasVal = i <= initialKeymaps.size();
		auto val = hasVal ? initialKeymaps[i - 1] : 0;
		nodes[i] = Node(
				&nodes[i - 1],
				&nodes[i + 1],
				val,
				hasVal);
	}
	nodes[KEYMAP_STACK_SIZE-1] = Node(
			&nodes[KEYMAP_STACK_SIZE - 2],
			nullptr, 0, false);
	
	rootNode = node = &nodes[0];

	for (auto mod = 0; mod < MAX_MOD; ++mod) {
		for (Key key : _modifierKeys[mod]) {
			modifierKeys.emplace(key, mod);
		}
	}
}
KeymapStack::~KeymapStack() {
	free(rootNode);
}
void KeymapStack::Exit() {
	inputHandler->Stop();
}
void KeymapStack::SetLed(uint16_t led, bool value) {
	inputHandler->SetLed(led, value);
}

bool KeymapStack::GetModifier(Modifier mod) const {
	return modifiers[mod] != 0;
}

void KeymapStack::Pop() {
	if (!node->prev) return;
	node->used = false;
	node = node->prev;
	Logger();

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
	Logger();
}
KeymapIndex KeymapStack::Top() const {
	return node->value;
}
void KeymapStack::PressBase(Key key, int event) {
	Node *n = node;
	KeyAction fun;
	while (n) {
		fun = keymaps[n->value][key][event];
		if (fun) {
			fun(this);
			return;
		}
		n = n->prev;
	}

}

void KeymapStack::HandleKey(Key key, int event) {
	assert(event < 3);
	PressBase(key, event);
	if (event == KEY_VAL_HOLD) return;

	auto it = modifierKeys.find(key);
	if (it == modifierKeys.end()) goto end;

	if (event == KEY_VAL_DOWN) {
		++modifiers[it->second];
		rootNode->value |= (1 << it->second);
	}
	else /*KEY_VAL_UP*/ {
		if (--modifiers[it->second]) goto end;
		rootNode->value &= ~(1 << it->second);
	}
end:
	Logger();

}

}// namespace Kmswm

