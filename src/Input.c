#include "Input.h"
#include "Utils.h"

Input* Input_create() {
	Input* this = malloc(sizeof(Input));
	this->hotkeys = Vector_Create();

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
		SDL_Joystick* j = SDL_JoystickOpen(i);
		int axes = SDL_JoystickNumAxes(j);
		int buttons = SDL_JoystickNumButtons(j);
		int hats = SDL_JoystickNumHats(j);
		Vector_AddElement(this->joysticks, j);
		printf("%d: %s | axes:%d buttons:%d hats:%d \n", i, SDL_JoystickName(i), axes, buttons, hats);
	}

//	SDL_JoystickEventState (SDL_IGNORE);


	return this;
}

void Input_destroy(Input* this) {
	FREE_VECTOR_WITH_ELMENTS(this->hotkeys, free);
	free(this);
}

void Input_addHotkey(Input* this, InputHotkey* hotkey) {
	Vector_InsertInFirstFreeSpace(this->hotkeys, hotkey);
	printf("added hotkey %d for keycode %d \n", hotkey->id, hotkey->hotkey.key.key.sym);
}

void Input_loadHotkeys(Input* this, lua_State* l, const char* filePath) {
	if (luaL_dofile(l, filePath)) {
		printf("Could not execute Lua hotkeys file: %s\n", filePath);
		printf("%s\n", lua_tostring(l, -1));
		return;
	}
	Input_parseKeyboardHotkeys(this, l);
	Input_parseJoystickHotkeys(this, l);
}

void Input_parseKeyboardHotkeys(Input* this, lua_State* l) {
	lua_getglobal(l, "keyboardHotkeys");
	lua_pushnil(l);
	while (lua_next(l, -2)) {

		if (lua_istable(l, -1)) {
			InputHotkey* hotkey = InputHotkey_create(HOTKEY_TYPE_KEYBOARD, none);

			lua_pushnil(l);
			while (lua_next(l, -2)) {
				const char* subKey = lua_tostring(l, -2);

				if (strcmp(subKey, "name") == 0) {
					hotkey->id = Input_stringToActionId(lua_tostring(l, -1));
				} else if (strcmp(subKey, "key") == 0) {
					hotkey->hotkey.key.key.sym = Input_stringToKeycode(lua_tostring(l, -1));
				} else if (strcmp(subKey, "axisValue") == 0) {
					hotkey->hotkey.key.axisValue = lua_tointeger(l, -1);
				// mods
				} else if (strcmp(subKey, "lshift") == 0) {
					hotkey->hotkey.key.key.mod = hotkey->hotkey.key.key.mod | KMOD_LSHIFT;
				} else if (strcmp(subKey, "rshift") == 0) {
					hotkey->hotkey.key.key.mod = hotkey->hotkey.key.key.mod | KMOD_RSHIFT;
				} else if (strcmp(subKey, "lctrl") == 0) {
					hotkey->hotkey.key.key.mod = hotkey->hotkey.key.key.mod | KMOD_LCTRL;
				} else if (strcmp(subKey, "rctrl") == 0) {
					hotkey->hotkey.key.key.mod = hotkey->hotkey.key.key.mod | KMOD_RCTRL;
				} else if (strcmp(subKey, "caps") == 0) {
					hotkey->hotkey.key.key.mod = hotkey->hotkey.key.key.mod | KMOD_LCTRL;
				} else {
					printf("\nunknown key %s\n", subKey);
				}

				lua_pop(l, 1);
			}
			if (hotkey->id != none) {
				//printf("['%d']=%d ", hotkey->id, hotkey->hotkey.key.key.sym);
				Input_addHotkey(this, hotkey);
			} else {
				free(hotkey);
			}
		}
//		printf("}\n");
		lua_pop(l,1);
	}
}

void Input_parseJoystickHotkeys(Input* this, lua_State* l) {
	lua_getglobal(l, "joystickHotkeys");
	lua_pushnil(l);
	while (lua_next(l, -2)) {

		if (lua_istable(l, -1)) {
			InputHotkey* hotkey = InputHotkey_create(HOTKEY_TYPE_JOYSTICK, none);

			lua_pushnil(l);
			while (lua_next(l, -2)) {
				const char* subKey = lua_tostring(l, -2);

				if (strcmp(subKey, "name") == 0) {
					hotkey->id = Input_stringToActionId(lua_tostring(l, -1));
				} else if (strcmp(subKey, "axis") == 0) {
					hotkey->hotkey.joystick.axisNumber = lua_tointeger(l, -1);
				} else if (strcmp(subKey, "hat") == 0) {
					hotkey->hotkey.joystick.hatNumber= lua_tointeger(l, -1);
				} else if (strcmp(subKey, "button") == 0) {
					hotkey->hotkey.joystick.buttonNumber = lua_tointeger(l, -1);
				} else if (strcmp(subKey, "trackball") == 0) {
					hotkey->hotkey.joystick.trackballNumber = lua_tointeger(l, -1);
				} else {
					printf("\nunknown joystick key %s\n", subKey);
				}

				//printf("['%s']=%s ", subKey, subValue);
				lua_pop(l, 1);
			}
			if (hotkey->id != none) {
				Input_addHotkey(this, hotkey);
			} else {
				free(hotkey);
			}
		}
//		printf("}\n");
		lua_pop(l,1);
	}
}



SDLKey Input_stringToKeycode(const char* keyText) {
	// lazy workaround to make at least ascii keys work
	if (keyText[0] != '\0' && keyText[1] == '\0' && keyText[0] >= 97 && keyText[0] <= 122) {
		return (SDLKey)keyText[0];
	}
	return SDLK_UNKNOWN;
}

ActionId Input_stringToActionId(const char* actionIdText) {
	ENUM_TO_STRING_MATCH(horizontal, actionIdText)
	ENUM_TO_STRING_MATCH(vertical, actionIdText)
	ENUM_TO_STRING_MATCH(kickLeft, actionIdText)
	ENUM_TO_STRING_MATCH(kickRight, actionIdText)
	ENUM_TO_STRING_MATCH(hitLeft, actionIdText)
	ENUM_TO_STRING_MATCH(hitRight, actionIdText)
	ENUM_TO_STRING_MATCH(action, actionIdText)
	return none;
}

int Input_getAxis(Input* this, ActionId hotkeyId) {
	int value = 0;
	for (int i=0; i < this->hotkeys->usedElements; ++i) {
		InputHotkey* it = this->hotkeys->elements[i];
		if (it != NULL && it->id == hotkeyId) {

			if (it->hotkeyType == HOTKEY_TYPE_KEYBOARD && Input_keysymIsDown(this, &it->hotkey.key.key)) {
				value += it->hotkey.key.axisValue*AXIS_MAX;
			} else if(it->hotkeyType == HOTKEY_TYPE_JOYSTICK) {
				int newValue = SDL_JoystickGetAxis(this->joysticks->elements[0], it->hotkey.joystick.axisNumber);
				value = MIN(value + newValue, AXIS_MAX);
			}
		}
	}
	if (abs(value) < AXIS_MIN_TRIGGER) {
		return 0;
	}
	return value;
}

float Input_getAxisMultiplier(Input* this, ActionId hotkeyId) {
	int axis = Input_getAxis(this, hotkeyId);
	if (abs(axis) > AXIS_MIN_TRIGGER) {
		return (float)AXIS_MAX/(float)axis;
	}
	return 0.f;
}

bool Input_isDown(Input* this, ActionId hotkeyId) {
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
}

InputHotkey* InputHotkey_create(int type, int actionId) {
	InputHotkey* this = malloc(sizeof(InputHotkey));
	memset(this, 0, sizeof(InputHotkey));
	this->hotkeyType = type;
	this->id = actionId;
	return this;
}
