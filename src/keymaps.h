#pragma once

#include <functional>
#include <stack>
#include <map>

#ifndef KEYMAP_SIZE
#define KEYMAP_SIZE 256
#endif
#ifndef KEYMAP_STACK_SIZE
#define KEYMAP_STACK_SIZE 16
#endif

namespace Kmswm {
struct KeymapStack;
typedef size_t Key;
typedef void (*KeyAction) (const KeymapStack *);
//typedef std::function<void(const KeymapStack*)> KeyAction;

struct Keymap {
	std::array<KeyAction, KEYMAP_SIZE> data;
	Keymap();
	Keymap(const Keymap& obj);
	~Keymap();
	KeyAction& operator[] (Key key);
	const KeyAction& operator[] (Key key) const;
};
class KeymapStack {
public:
	std::vector<Keymap> keymaps;//keep track of keymaps and handle memory for them
	KeymapStack();
	KeymapStack(std::vector<Keymap> keymaps, size_t defaultKeymap = 0);

	~KeymapStack();
	void Pop();
	void Push(const Keymap& keymap);
	const Keymap& Top();
	void PressKey(Key key);

private:
	struct Node {
		Node *next;
		Node *prev;
		bool used;
		Keymap value;
		~Node();
	};
	Node *root;
	Node *node;
};
}//namespace Kmswm
