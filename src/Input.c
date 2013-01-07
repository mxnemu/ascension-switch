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
		printf("%d: %s\n", i, SDL_JoystickName(i));
		SDL_Joystick* j = SDL_JoystickOpen(i);
		Vector_AddElement(this->joysticks, j);
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
}

void Input_parseKeyboardHotkeys(Input* this, lua_State* l) {
	lua_getglobal(l, "keyboardHotkeys");
	lua_pushnil(l);
	while (lua_next(l, 1)) {

		lua_pushvalue(l, -2);
		const char* key = lua_tostring(l, -1);
		int actionId = Input_stringToActionId(key);
//		printf("%s: {", key);

		if (lua_istable(l, -2)) {
			InputHotkey* hotkey = InputHotkey_create(HOTKEY_TYPE_KEYBOARD, actionId);

			lua_pushnil(l);
			while (lua_next(l, -3)) {
				lua_pushvalue(l, -2);
				const char* subKey = lua_tostring(l, -1);

				if (strcmp(subKey, "key") == 0) {
					hotkey->hotkey.key.key.sym = Input_stringToKeycode(lua_tostring(l, -2));
				} else if (strcmp(subKey, "axisValue") == 0) {
					hotkey->hotkey.key.axisValue = lua_tointeger(l, -2);
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

				//printf("['%s']=%s ", subKey, subValue);
				lua_pop(l, 2);
			}

			Input_addHotkey(this, hotkey);
		}
//		printf("}\n");
		lua_pop(l,2);
	}
}

void Input_parseJoystickHotkeys(Input* this, lua_State* l) {
	lua_getglobal(l, "joystickHotkeys");
	lua_pushnil(l);
	while (lua_next(l, 1)) {

		lua_pushvalue(l, -2);
		const char* key = lua_tostring(l, -1);
		int actionId = Input_stringToActionId(key);
//		printf("%s: {", key);

		if (lua_istable(l, -2)) {
			InputHotkey* hotkey = InputHotkey_create(HOTKEY_TYPE_KEYBOARD, actionId);

			lua_pushnil(l);
			while (lua_next(l, -3)) {
				lua_pushvalue(l, -2);
				const char* subKey = lua_tostring(l, -1);

				if (strcmp(subKey, "axis") == 0) {
					hotkey->hotkey.joystick.axisNumber = lua_tointeger(l, -2);
				} else if (strcmp(subKey, "button") == 0) {
					hotkey->hotkey.joystick.buttonNumber = lua_tointeger(l, -2);
				} else if (strcmp(subKey, "trackball") == 0) {
					hotkey->hotkey.joystick.trackballNumber = lua_tointeger(l, -2);
				} else {
					printf("\nunknown key %s\n", subKey);
				}

				//printf("['%s']=%s ", subKey, subValue);
				lua_pop(l, 2);
			}

			Input_addHotkey(this, hotkey);
		}
//		printf("}\n");
		lua_pop(l,2);
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
	ENUM_TO_STRING_MATCH(left, actionIdText)
	ENUM_TO_STRING_MATCH(right, actionIdText)
	ENUM_TO_STRING_MATCH(up, actionIdText)
	ENUM_TO_STRING_MATCH(down, actionIdText)
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
				value = MAX(abs(value), abs(it->hotkey.key.axisValue));
			} else if(it->hotkeyType == HOTKEY_TYPE_JOYSTICK) {
				value = MAX(abs(value), abs(SDL_JoystickGetAxis(it->hotkey.joystick.joystick, it->hotkey.joystick.axisNumber)));
			}
		}
	}
	return value;
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
//	for (int i=0; i < this->joysticks->usedElements; ++i) {
//		SDL_Joystick* it = this->joysticks->elements[i];
//		if (it) {
//
//		}
//	}
}

InputHotkey* InputHotkey_create(int type, int actionId) {
	InputHotkey* this = malloc(sizeof(InputHotkey));
	this->hotkeyType = type;
	this->id = actionId;
	return this;
}
