#pragma once
#include "Constants.h"
#include "Vector.h"

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
	int id;
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
void Input_initDefaultHotkeys(Input* this);
void Input_addHotkey(Input* this, InputHotkey* hotkey);

int Input_getAxis(Input* this, int hotkeyId);
bool Input_isDown(Input* this, int hotkeyId);

bool Input_keysymIsDown(Input* this, SDL_keysym* keysym);
