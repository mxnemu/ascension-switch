#include "Input.h"

Input* Input_create() {
	Input* this = malloc(sizeof(this));

	this->joysticks = Vector_Create();

	int joysticksCount = SDL_NumJoysticks();

	if (joysticksCount > 1) {
		printf("Input: found %d Joysticks/Gamepads:\n", joysticksCount);
	} else if (joysticksCount == 1) {
		printf("Input: found 1 Joystick/Gamepad:\n");
	} else {
		printf("Input: found no Joysticks/Gamepads\n");
	}


	for (int i=0; i < joysticksCount; ++i) {
		printf("%d: %s\n", i, SDL_JoystickName(i));
		SDL_Joystick* j = SDL_JoystickOpen(i);
		Vector_AddElement(this->joysticks, j);
	}

//	SDL_JoystickEventState (SDL_IGNORE);


	return this;
}

void Input_destroy(Input* this) {
	free(this);
}

void Input_initDefaultHotkeys(Input* this) {

}

int Input_getAxis(Input* this, int hotkeyId) {
	int value = 0;
	for (int i=0; i < this->hotkeys->usedElements; ++i) {
		InputHotkey* it = this->hotkeys->elements[i];
		if (it != NULL && it->id == hotkeyId) {

			if (it->hotkeyType == HOTKEY_TYPE_KEYBOARD) {
				value = MAX(value, it->hotkey.key.axisValue);
			} else if(it->hotkeyType == HOTKEY_TYPE_JOYSTICK) {
				value = MAX(value, SDL_JoystickGetAxis(it->hotkey.joystick.joystick, it->hotkey.joystick.axisNumber));
			}
		}
	}
	return value;
}

bool Input_isDown(Input* this, int hotkeyId) {
	for (int i=0; i < this->hotkeys->usedElements; ++i) {
		InputHotkey* it = this->hotkeys->elements[i];
		if (it != NULL && it->id == hotkeyId) {

			if (it->hotkeyType == HOTKEY_TYPE_KEYBOARD) {
				if (Input_keysymIsDown(this, &it->hotkey.key.key)) {
					return true;
				}
			} else if(it->hotkeyType == HOTKEY_TYPE_JOYSTICK) {
				if (SDL_JoystickGetButton(it->hotkey.joystick.joystick, it->hotkey.joystick.buttonNumber)) {
					return true;
				}
			}
		}
	}
	return false;
}

bool Input_keysymIsDown(Input* this, SDL_keysym* keysym) {
	return this->keystates[keysym->sym] && (this->keymods & keysym->mod) == keysym->mod;
}

void Input_update(Input* this) {
	this->keystates = SDL_GetKeyState(NULL); // TODO do I have to free this?
	this->keymods = SDL_GetModState();
//	for (int i=0; i < this->joysticks->usedElements; ++i) {
//		SDL_Joystick* it = this->joysticks->elements[i];
//		if (it) {
//
//		}
//	}
}
