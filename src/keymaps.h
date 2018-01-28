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
//using std::function gives significantly slower compile times
typedef void (*KeyAction) (KeymapStack *);
//typedef std::function<void(KeymapStack*)> KeyAction;
struct KeyData {
	KeyAction up;
	KeyAction down;
	KeyAction hold;
};
typedef std::array<KeyData, KEYMAP_SIZE> Keymap;
typedef int KeymapIndex;

class KeymapStack {
public:
	KeymapStack(std::function<void()> exit, std::vector<Keymap> keymaps, KeymapIndex defaultKeymap = 0);

	~KeymapStack();
	void Exit();
	void Pop();
	void Pop(KeymapIndex index);
	void Push(KeymapIndex index);
	void PressKey(Key key);
	void HoldKey(Key key);
	void ReleaseKey(Key key);

	KeymapIndex Top();

private:
	struct Node {
		Node *next;
		Node *prev;
		bool used;
		KeymapIndex value;
	};
	Node *nodes;
	Node *node;
	std::function<void()> exit;
	std::vector<Keymap> keymaps;//keep track of keymaps and handle memory for them
};
}//namespace Kmswm
