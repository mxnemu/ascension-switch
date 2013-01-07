#pragma once
#include "Constants.h"
#include "Vector.h"

#define AXIS_MIN_TRIGGER 200
#define AXIS_MAX 32767

// TODO somehow differentiate the player. Store hotkeys in Player.c or something
typedef enum ActionId {
	none, horizontal, vertical, kickLeft, kickRight, hitLeft, hitRight, action
} ActionId;

CLASS(KeyHotkey) {
	SDL_keysym key;
	int axisValue;
};

CLASS(JoystickHotkey) {
	SDL_Joystick* joystick;
	int axisNumber;
	int buttonNumber;
	int trackballNumber;
};

#define HOTKEY_TYPE_KEYBOARD 1
#define HOTKEY_TYPE_JOYSTICK 2
CLASS(InputHotkey) {
	union {
		KeyHotkey key;
		JoystickHotkey joystick;
	} hotkey;
	short hotkeyType;
	ActionId id;
};

CLASS(Input) {
	Uint8* keystates;
	SDLMod keymods;
	Vector* joysticks;

	Vector* hotkeys;
};

Input* Input_create();
void Input_destroy(Input* this);
void Input_update(Input* this);
void Input_addHotkey(Input* this, InputHotkey* hotkey);
void Input_loadHotkeys(Input* this, lua_State* l, const char* filePath);
void Input_parseKeyboardHotkeys(Input* this, lua_State* l);
void Input_parseJoystickHotkeys(Input* this, lua_State* l);


int Input_getAxis(Input* this, ActionId id);
float Input_getAxisMultiplier(Input* this, ActionId hotkeyId);
bool Input_isDown(Input* this, ActionId id);

bool Input_keysymIsDown(Input* this, SDL_keysym* keysym);


InputHotkey* InputHotkey_create(int type, int actionId);

// Utils
SDLKey Input_stringToKeycode(const char* keyText);
ActionId Input_stringToActionId(const char* actionIdText);

