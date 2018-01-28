#pragma once

#include <functional>
#include <vector>

#ifndef KEYMAP_SIZE
#define KEYMAP_SIZE 256
#endif
#ifndef KEYMAP_STACK_SIZE
#define KEYMAP_STACK_SIZE 16
#endif

namespace Kmswm {
struct KeymapStack;
typedef size_t Key;
typedef void (*KeyAction) (KeymapStack *);
struct KeyData {
	KeyAction up;
	KeyAction down;
	KeyAction hold;
};
//typedef std::function<void(const KeymapStack*)> KeyAction;
typedef std::array<KeyData, KEYMAP_SIZE> Keymap;
typedef int KeymapIndex;

class KeymapStack {
public:
	std::vector<Keymap> keymaps;//keep track of keymaps and handle memory for them
	KeymapStack();
	KeymapStack(std::vector<Keymap> keymaps, KeymapIndex defaultKeymap = 0);

	~KeymapStack();
	void Pop();
	void Pop(KeymapIndex index);
	void Push(KeymapIndex index);
	void PressKey(Key key);
	void HoldKey(Key key);
	void ReleaseKey(Key key);

private:
	struct Node {
		Node *next;
		Node *prev;
		bool used;
		KeymapIndex value;
	};
	Node *nodes;
	Node *node;
};
}//namespace Kmswm
