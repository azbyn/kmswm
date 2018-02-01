#pragma once

#include <functional>
#include <vector>
#include <map>

#ifndef KEYMAP_SIZE
#define KEYMAP_SIZE 256
#endif
#ifndef KEYMAP_STACK_SIZE
#define KEYMAP_STACK_SIZE 16
#endif

#define KEY_VAL_UP   0
#define KEY_VAL_DOWN 1
#define KEY_VAL_HOLD 2

namespace kmswm {
typedef size_t Key;
typedef uint8_t Modifier;
//using std::function gives significantly slower compile and run times
typedef void (*KeyAction) (class KeymapStack *);
//typedef std::function<void(KeymapStack*)> KeyAction;

typedef std::array<KeyAction, 3> KeyData;
typedef std::array<KeyData, KEYMAP_SIZE> Keymap;
typedef int KeymapIndex;

//keys that change state when held
enum Modifiers : Modifier {
	MOD_SHIFT,
	MOD_CTRL,
	MOD_ALT,
	MOD_SUPER,
	//MOD_ALTGR

	MAX_MOD,
};
enum Keymaps : KeymapIndex {
	KM_NORMAL = 0,
	KM_SHIFT  = 1 << MOD_SHIFT,
	KM_CTRL   = 1 << MOD_CTRL ,
	KM_ALT    = 1 << MOD_ALT  ,
	KM_SUPER  = 1 << MOD_SUPER,

	//keys that toggle state when pressed
	KM_CAPS = 1 << MAX_MOD,
	KM_ANTICAPS,

	//COMPOSE,
	//NUMLOCK
	
	MAX_KM,
};
class KeymapStack {
	struct Node {
		Node *prev;
		Node *next;
		bool used;
		KeymapIndex value;

		Node(Node *prev, Node *next, KeymapIndex value, bool used);
	};
	Node *rootNode;
	Node *node;

	// we use uint8_t because we could press multiple of the same
	// modifier at once (ie. pressing both l-ctrl and r-ctrl)
	std::array<uint8_t, MAX_MOD> modifiers;
	std::map<Key, Modifier> modifierKeys;
	class InputHandler *inputHandler;
	const std::vector<Keymap> keymaps;
	
	void PressBase(Key key, int event);
	void Logger();
public:
	KeymapStack(
			class InputHandler *ih,
			std::vector<Keymap> keymaps,
			std::vector<KeymapIndex> initialKeymaps,
			std::array<std::vector<Key>, MAX_MOD> modifierKeys);

	~KeymapStack();
	void Exit();
	void SetLed(uint16_t led, bool value);

	void Pop();
	void Pop(KeymapIndex index);
	void Push(KeymapIndex index);
	void HandleKey(Key key, int value);

	bool GetModifier(Modifier mod) const;
	KeymapIndex Top() const;

	static KeymapStack generate(class InputHandler *ih);


};
}//namespace Kmswm
