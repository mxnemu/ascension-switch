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
		printf("%d: %s | axes:%d buttons:%d hats:%d \n", i, SDL_JoystickName(j), axes, buttons, hats);
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


SDL_Keycode Input_stringToKeycode(const char* keyText) {
	// lazy workaround to make at least ascii keys work
	if (keyText[0] != '\0' && keyText[1] == '\0' && keyText[0] >= 97 && keyText[0] <= 122) {
		return (SDL_Keycode)keyText[0];
	}
	ENUM_TO_STRING_MATCH(SDLK_RETURN, keyText);
	ENUM_TO_STRING_MATCH(SDLK_ESCAPE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_BACKSPACE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_TAB, keyText);
	ENUM_TO_STRING_MATCH(SDLK_SPACE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_EXCLAIM, keyText);
	ENUM_TO_STRING_MATCH(SDLK_QUOTEDBL, keyText);
	ENUM_TO_STRING_MATCH(SDLK_HASH, keyText);
	ENUM_TO_STRING_MATCH(SDLK_PERCENT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_DOLLAR, keyText);
	ENUM_TO_STRING_MATCH(SDLK_AMPERSAND, keyText);
	ENUM_TO_STRING_MATCH(SDLK_QUOTE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_LEFTPAREN, keyText);
	ENUM_TO_STRING_MATCH(SDLK_RIGHTPAREN, keyText);
	ENUM_TO_STRING_MATCH(SDLK_ASTERISK, keyText);
	ENUM_TO_STRING_MATCH(SDLK_PLUS, keyText);
	ENUM_TO_STRING_MATCH(SDLK_COMMA, keyText);
	ENUM_TO_STRING_MATCH(SDLK_MINUS, keyText);
	ENUM_TO_STRING_MATCH(SDLK_PERIOD, keyText);
	ENUM_TO_STRING_MATCH(SDLK_SLASH, keyText);
	ENUM_TO_STRING_MATCH(SDLK_0, keyText);
	ENUM_TO_STRING_MATCH(SDLK_1, keyText);
	ENUM_TO_STRING_MATCH(SDLK_2, keyText);
	ENUM_TO_STRING_MATCH(SDLK_3, keyText);
	ENUM_TO_STRING_MATCH(SDLK_4, keyText);
	ENUM_TO_STRING_MATCH(SDLK_5, keyText);
	ENUM_TO_STRING_MATCH(SDLK_6, keyText);
	ENUM_TO_STRING_MATCH(SDLK_7, keyText);
	ENUM_TO_STRING_MATCH(SDLK_8, keyText);
	ENUM_TO_STRING_MATCH(SDLK_9, keyText);
	ENUM_TO_STRING_MATCH(SDLK_COLON, keyText);
	ENUM_TO_STRING_MATCH(SDLK_SEMICOLON, keyText);
	ENUM_TO_STRING_MATCH(SDLK_LESS, keyText);
	ENUM_TO_STRING_MATCH(SDLK_EQUALS, keyText);
	ENUM_TO_STRING_MATCH(SDLK_GREATER, keyText);
	ENUM_TO_STRING_MATCH(SDLK_QUESTION, keyText);
	ENUM_TO_STRING_MATCH(SDLK_AT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_LEFTBRACKET, keyText);
	ENUM_TO_STRING_MATCH(SDLK_BACKSLASH, keyText);
	ENUM_TO_STRING_MATCH(SDLK_RIGHTBRACKET, keyText);
	ENUM_TO_STRING_MATCH(SDLK_CARET, keyText);
	ENUM_TO_STRING_MATCH(SDLK_UNDERSCORE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_BACKQUOTE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_a, keyText);
	ENUM_TO_STRING_MATCH(SDLK_b, keyText);
	ENUM_TO_STRING_MATCH(SDLK_c, keyText);
	ENUM_TO_STRING_MATCH(SDLK_d, keyText);
	ENUM_TO_STRING_MATCH(SDLK_e, keyText);
	ENUM_TO_STRING_MATCH(SDLK_f, keyText);
	ENUM_TO_STRING_MATCH(SDLK_g, keyText);
	ENUM_TO_STRING_MATCH(SDLK_h, keyText);
	ENUM_TO_STRING_MATCH(SDLK_i, keyText);
	ENUM_TO_STRING_MATCH(SDLK_j, keyText);
	ENUM_TO_STRING_MATCH(SDLK_k, keyText);
	ENUM_TO_STRING_MATCH(SDLK_l, keyText);
	ENUM_TO_STRING_MATCH(SDLK_m, keyText);
	ENUM_TO_STRING_MATCH(SDLK_n, keyText);
	ENUM_TO_STRING_MATCH(SDLK_o, keyText);
	ENUM_TO_STRING_MATCH(SDLK_p, keyText);
	ENUM_TO_STRING_MATCH(SDLK_q, keyText);
	ENUM_TO_STRING_MATCH(SDLK_r, keyText);
	ENUM_TO_STRING_MATCH(SDLK_s, keyText);
	ENUM_TO_STRING_MATCH(SDLK_t, keyText);
	ENUM_TO_STRING_MATCH(SDLK_u, keyText);
	ENUM_TO_STRING_MATCH(SDLK_v, keyText);
	ENUM_TO_STRING_MATCH(SDLK_w, keyText);
	ENUM_TO_STRING_MATCH(SDLK_x, keyText);
	ENUM_TO_STRING_MATCH(SDLK_y, keyText);
	ENUM_TO_STRING_MATCH(SDLK_z, keyText);

	ENUM_TO_STRING_MATCH(SDLK_CAPSLOCK, keyText);

	ENUM_TO_STRING_MATCH(SDLK_F1, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F2, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F3, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F4, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F5, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F6, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F7, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F8, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F9, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F10, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F11, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F12, keyText);

	ENUM_TO_STRING_MATCH(SDLK_PRINTSCREEN, keyText);
	ENUM_TO_STRING_MATCH(SDLK_SCROLLLOCK, keyText);
	ENUM_TO_STRING_MATCH(SDLK_PAUSE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_INSERT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_HOME, keyText);
	ENUM_TO_STRING_MATCH(SDLK_PAGEUP, keyText);
	ENUM_TO_STRING_MATCH(SDLK_DELETE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_END, keyText);
	ENUM_TO_STRING_MATCH(SDLK_PAGEDOWN, keyText);
	ENUM_TO_STRING_MATCH(SDLK_RIGHT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_LEFT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_DOWN, keyText);
	ENUM_TO_STRING_MATCH(SDLK_UP, keyText);

	ENUM_TO_STRING_MATCH(SDLK_NUMLOCKCLEAR, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_DIVIDE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_MULTIPLY, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_MINUS, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_PLUS, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_ENTER, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_1, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_2, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_3, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_4, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_5, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_6, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_7, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_8, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_9, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_0, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_PERIOD, keyText);

	ENUM_TO_STRING_MATCH(SDLK_APPLICATION, keyText);
	ENUM_TO_STRING_MATCH(SDLK_POWER, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_EQUALS, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F13, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F14, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F15, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F16, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F17, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F18, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F19, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F20, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F21, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F22, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F23, keyText);
	ENUM_TO_STRING_MATCH(SDLK_F24, keyText);
	ENUM_TO_STRING_MATCH(SDLK_EXECUTE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_HELP, keyText);
	ENUM_TO_STRING_MATCH(SDLK_MENU, keyText);
	ENUM_TO_STRING_MATCH(SDLK_SELECT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_STOP, keyText);
	ENUM_TO_STRING_MATCH(SDLK_AGAIN, keyText);
	ENUM_TO_STRING_MATCH(SDLK_UNDO, keyText);
	ENUM_TO_STRING_MATCH(SDLK_CUT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_COPY, keyText);
	ENUM_TO_STRING_MATCH(SDLK_PASTE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_FIND, keyText);
	ENUM_TO_STRING_MATCH(SDLK_MUTE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_VOLUMEUP, keyText);
	ENUM_TO_STRING_MATCH(SDLK_VOLUMEDOWN, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_COMMA, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_EQUALSAS400, keyText);

	ENUM_TO_STRING_MATCH(SDLK_ALTERASE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_SYSREQ, keyText);
	ENUM_TO_STRING_MATCH(SDLK_CANCEL, keyText);
	ENUM_TO_STRING_MATCH(SDLK_CLEAR, keyText);
	ENUM_TO_STRING_MATCH(SDLK_PRIOR, keyText);
	ENUM_TO_STRING_MATCH(SDLK_RETURN2, keyText);
	ENUM_TO_STRING_MATCH(SDLK_SEPARATOR, keyText);
	ENUM_TO_STRING_MATCH(SDLK_OUT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_OPER, keyText);
	ENUM_TO_STRING_MATCH(SDLK_CLEARAGAIN, keyText);
	ENUM_TO_STRING_MATCH(SDLK_CRSEL, keyText);
	ENUM_TO_STRING_MATCH(SDLK_EXSEL, keyText);

	ENUM_TO_STRING_MATCH(SDLK_KP_00, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_000, keyText);
	ENUM_TO_STRING_MATCH(SDLK_THOUSANDSSEPARATOR, keyText);
	ENUM_TO_STRING_MATCH(SDLK_DECIMALSEPARATOR, keyText);
	ENUM_TO_STRING_MATCH(SDLK_CURRENCYUNIT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_CURRENCYSUBUNIT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_LEFTPAREN, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_RIGHTPAREN, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_LEFTBRACE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_RIGHTBRACE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_TAB, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_BACKSPACE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_A, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_B, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_C, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_D, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_E, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_F, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_XOR, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_POWER, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_PERCENT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_LESS, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_GREATER, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_AMPERSAND, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_DBLAMPERSAND, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_VERTICALBAR, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_DBLVERTICALBAR, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_COLON, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_HASH, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_SPACE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_AT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_EXCLAM, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_MEMSTORE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_MEMRECALL, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_MEMCLEAR, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_MEMADD, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_MEMSUBTRACT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_MEMMULTIPLY, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_MEMDIVIDE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_PLUSMINUS, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_CLEAR, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_CLEARENTRY, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_BINARY, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_OCTAL, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_DECIMAL, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KP_HEXADECIMAL, keyText);

	ENUM_TO_STRING_MATCH(SDLK_LCTRL, keyText);
	ENUM_TO_STRING_MATCH(SDLK_LSHIFT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_LALT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_LGUI, keyText);
	ENUM_TO_STRING_MATCH(SDLK_RCTRL, keyText);
	ENUM_TO_STRING_MATCH(SDLK_RSHIFT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_RALT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_RGUI, keyText);

	ENUM_TO_STRING_MATCH(SDLK_MODE, keyText);

	ENUM_TO_STRING_MATCH(SDLK_AUDIONEXT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_AUDIOPREV, keyText);
	ENUM_TO_STRING_MATCH(SDLK_AUDIOSTOP, keyText);
	ENUM_TO_STRING_MATCH(SDLK_AUDIOPLAY, keyText);
	ENUM_TO_STRING_MATCH(SDLK_AUDIOMUTE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_MEDIASELECT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_WWW, keyText);
	ENUM_TO_STRING_MATCH(SDLK_MAIL, keyText);
	ENUM_TO_STRING_MATCH(SDLK_CALCULATOR, keyText);
	ENUM_TO_STRING_MATCH(SDLK_COMPUTER, keyText);
	ENUM_TO_STRING_MATCH(SDLK_AC_SEARCH, keyText);
	ENUM_TO_STRING_MATCH(SDLK_AC_HOME, keyText);
	ENUM_TO_STRING_MATCH(SDLK_AC_BACK, keyText);
	ENUM_TO_STRING_MATCH(SDLK_AC_FORWARD, keyText);
	ENUM_TO_STRING_MATCH(SDLK_AC_STOP, keyText);
	ENUM_TO_STRING_MATCH(SDLK_AC_REFRESH, keyText);
	ENUM_TO_STRING_MATCH(SDLK_AC_BOOKMARKS, keyText);

	ENUM_TO_STRING_MATCH(SDLK_BRIGHTNESSDOWN, keyText);
	ENUM_TO_STRING_MATCH(SDLK_BRIGHTNESSUP, keyText);
	ENUM_TO_STRING_MATCH(SDLK_DISPLAYSWITCH, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KBDILLUMTOGGLE, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KBDILLUMDOWN, keyText);
	ENUM_TO_STRING_MATCH(SDLK_KBDILLUMUP, keyText);
	ENUM_TO_STRING_MATCH(SDLK_EJECT, keyText);
	ENUM_TO_STRING_MATCH(SDLK_SLEEP, keyText);
	return SDLK_UNKNOWN;
}

ActionId Input_stringToActionId(const char* actionIdText) {
	ENUM_TO_STRING_MATCH(horizontal, actionIdText)
	ENUM_TO_STRING_MATCH(vertical, actionIdText)
	ENUM_TO_STRING_MATCH(attackSword, actionIdText)
	ENUM_TO_STRING_MATCH(attackBow, actionIdText)
	ENUM_TO_STRING_MATCH(jump, actionIdText)
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
				if (this->joysticks->usedElements > 0) {
					int newValue = SDL_JoystickGetAxis(this->joysticks->elements[0], it->hotkey.joystick.axisNumber);
					if (newValue < 0) {
						value = MAX(value + newValue, -AXIS_MAX);
					} else {
						value = MIN(value + newValue, AXIS_MAX);
					}
				}
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

bool Input_keysymIsDown(Input* this, SDL_Keysym* keysym) {
	return this->keystates[SDL_GetScancodeFromKey(keysym->sym)] && (this->keymods & keysym->mod) == keysym->mod;
}

void Input_update(Input* this) {
	this->keystates = SDL_GetKeyboardState(NULL);
	this->keymods = SDL_GetModState();
}

InputHotkey* InputHotkey_create(int type, int actionId) {
	InputHotkey* this = malloc(sizeof(InputHotkey));
	memset(this, 0, sizeof(InputHotkey));
	this->hotkeyType = type;
	this->id = actionId;
	return this;
}
