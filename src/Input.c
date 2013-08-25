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
	STRING_TO_ENUM(SDLK_RETURN, keyText);
	STRING_TO_ENUM(SDLK_ESCAPE, keyText);
	STRING_TO_ENUM(SDLK_BACKSPACE, keyText);
	STRING_TO_ENUM(SDLK_TAB, keyText);
	STRING_TO_ENUM(SDLK_SPACE, keyText);
	STRING_TO_ENUM(SDLK_EXCLAIM, keyText);
	STRING_TO_ENUM(SDLK_QUOTEDBL, keyText);
	STRING_TO_ENUM(SDLK_HASH, keyText);
	STRING_TO_ENUM(SDLK_PERCENT, keyText);
	STRING_TO_ENUM(SDLK_DOLLAR, keyText);
	STRING_TO_ENUM(SDLK_AMPERSAND, keyText);
	STRING_TO_ENUM(SDLK_QUOTE, keyText);
	STRING_TO_ENUM(SDLK_LEFTPAREN, keyText);
	STRING_TO_ENUM(SDLK_RIGHTPAREN, keyText);
	STRING_TO_ENUM(SDLK_ASTERISK, keyText);
	STRING_TO_ENUM(SDLK_PLUS, keyText);
	STRING_TO_ENUM(SDLK_COMMA, keyText);
	STRING_TO_ENUM(SDLK_MINUS, keyText);
	STRING_TO_ENUM(SDLK_PERIOD, keyText);
	STRING_TO_ENUM(SDLK_SLASH, keyText);
	STRING_TO_ENUM(SDLK_0, keyText);
	STRING_TO_ENUM(SDLK_1, keyText);
	STRING_TO_ENUM(SDLK_2, keyText);
	STRING_TO_ENUM(SDLK_3, keyText);
	STRING_TO_ENUM(SDLK_4, keyText);
	STRING_TO_ENUM(SDLK_5, keyText);
	STRING_TO_ENUM(SDLK_6, keyText);
	STRING_TO_ENUM(SDLK_7, keyText);
	STRING_TO_ENUM(SDLK_8, keyText);
	STRING_TO_ENUM(SDLK_9, keyText);
	STRING_TO_ENUM(SDLK_COLON, keyText);
	STRING_TO_ENUM(SDLK_SEMICOLON, keyText);
	STRING_TO_ENUM(SDLK_LESS, keyText);
	STRING_TO_ENUM(SDLK_EQUALS, keyText);
	STRING_TO_ENUM(SDLK_GREATER, keyText);
	STRING_TO_ENUM(SDLK_QUESTION, keyText);
	STRING_TO_ENUM(SDLK_AT, keyText);
	STRING_TO_ENUM(SDLK_LEFTBRACKET, keyText);
	STRING_TO_ENUM(SDLK_BACKSLASH, keyText);
	STRING_TO_ENUM(SDLK_RIGHTBRACKET, keyText);
	STRING_TO_ENUM(SDLK_CARET, keyText);
	STRING_TO_ENUM(SDLK_UNDERSCORE, keyText);
	STRING_TO_ENUM(SDLK_BACKQUOTE, keyText);
	STRING_TO_ENUM(SDLK_a, keyText);
	STRING_TO_ENUM(SDLK_b, keyText);
	STRING_TO_ENUM(SDLK_c, keyText);
	STRING_TO_ENUM(SDLK_d, keyText);
	STRING_TO_ENUM(SDLK_e, keyText);
	STRING_TO_ENUM(SDLK_f, keyText);
	STRING_TO_ENUM(SDLK_g, keyText);
	STRING_TO_ENUM(SDLK_h, keyText);
	STRING_TO_ENUM(SDLK_i, keyText);
	STRING_TO_ENUM(SDLK_j, keyText);
	STRING_TO_ENUM(SDLK_k, keyText);
	STRING_TO_ENUM(SDLK_l, keyText);
	STRING_TO_ENUM(SDLK_m, keyText);
	STRING_TO_ENUM(SDLK_n, keyText);
	STRING_TO_ENUM(SDLK_o, keyText);
	STRING_TO_ENUM(SDLK_p, keyText);
	STRING_TO_ENUM(SDLK_q, keyText);
	STRING_TO_ENUM(SDLK_r, keyText);
	STRING_TO_ENUM(SDLK_s, keyText);
	STRING_TO_ENUM(SDLK_t, keyText);
	STRING_TO_ENUM(SDLK_u, keyText);
	STRING_TO_ENUM(SDLK_v, keyText);
	STRING_TO_ENUM(SDLK_w, keyText);
	STRING_TO_ENUM(SDLK_x, keyText);
	STRING_TO_ENUM(SDLK_y, keyText);
	STRING_TO_ENUM(SDLK_z, keyText);

	STRING_TO_ENUM(SDLK_CAPSLOCK, keyText);

	STRING_TO_ENUM(SDLK_F1, keyText);
	STRING_TO_ENUM(SDLK_F2, keyText);
	STRING_TO_ENUM(SDLK_F3, keyText);
	STRING_TO_ENUM(SDLK_F4, keyText);
	STRING_TO_ENUM(SDLK_F5, keyText);
	STRING_TO_ENUM(SDLK_F6, keyText);
	STRING_TO_ENUM(SDLK_F7, keyText);
	STRING_TO_ENUM(SDLK_F8, keyText);
	STRING_TO_ENUM(SDLK_F9, keyText);
	STRING_TO_ENUM(SDLK_F10, keyText);
	STRING_TO_ENUM(SDLK_F11, keyText);
	STRING_TO_ENUM(SDLK_F12, keyText);

	STRING_TO_ENUM(SDLK_PRINTSCREEN, keyText);
	STRING_TO_ENUM(SDLK_SCROLLLOCK, keyText);
	STRING_TO_ENUM(SDLK_PAUSE, keyText);
	STRING_TO_ENUM(SDLK_INSERT, keyText);
	STRING_TO_ENUM(SDLK_HOME, keyText);
	STRING_TO_ENUM(SDLK_PAGEUP, keyText);
	STRING_TO_ENUM(SDLK_DELETE, keyText);
	STRING_TO_ENUM(SDLK_END, keyText);
	STRING_TO_ENUM(SDLK_PAGEDOWN, keyText);
	STRING_TO_ENUM(SDLK_RIGHT, keyText);
	STRING_TO_ENUM(SDLK_LEFT, keyText);
	STRING_TO_ENUM(SDLK_DOWN, keyText);
	STRING_TO_ENUM(SDLK_UP, keyText);

	STRING_TO_ENUM(SDLK_NUMLOCKCLEAR, keyText);
	STRING_TO_ENUM(SDLK_KP_DIVIDE, keyText);
	STRING_TO_ENUM(SDLK_KP_MULTIPLY, keyText);
	STRING_TO_ENUM(SDLK_KP_MINUS, keyText);
	STRING_TO_ENUM(SDLK_KP_PLUS, keyText);
	STRING_TO_ENUM(SDLK_KP_ENTER, keyText);
	STRING_TO_ENUM(SDLK_KP_1, keyText);
	STRING_TO_ENUM(SDLK_KP_2, keyText);
	STRING_TO_ENUM(SDLK_KP_3, keyText);
	STRING_TO_ENUM(SDLK_KP_4, keyText);
	STRING_TO_ENUM(SDLK_KP_5, keyText);
	STRING_TO_ENUM(SDLK_KP_6, keyText);
	STRING_TO_ENUM(SDLK_KP_7, keyText);
	STRING_TO_ENUM(SDLK_KP_8, keyText);
	STRING_TO_ENUM(SDLK_KP_9, keyText);
	STRING_TO_ENUM(SDLK_KP_0, keyText);
	STRING_TO_ENUM(SDLK_KP_PERIOD, keyText);

	STRING_TO_ENUM(SDLK_APPLICATION, keyText);
	STRING_TO_ENUM(SDLK_POWER, keyText);
	STRING_TO_ENUM(SDLK_KP_EQUALS, keyText);
	STRING_TO_ENUM(SDLK_F13, keyText);
	STRING_TO_ENUM(SDLK_F14, keyText);
	STRING_TO_ENUM(SDLK_F15, keyText);
	STRING_TO_ENUM(SDLK_F16, keyText);
	STRING_TO_ENUM(SDLK_F17, keyText);
	STRING_TO_ENUM(SDLK_F18, keyText);
	STRING_TO_ENUM(SDLK_F19, keyText);
	STRING_TO_ENUM(SDLK_F20, keyText);
	STRING_TO_ENUM(SDLK_F21, keyText);
	STRING_TO_ENUM(SDLK_F22, keyText);
	STRING_TO_ENUM(SDLK_F23, keyText);
	STRING_TO_ENUM(SDLK_F24, keyText);
	STRING_TO_ENUM(SDLK_EXECUTE, keyText);
	STRING_TO_ENUM(SDLK_HELP, keyText);
	STRING_TO_ENUM(SDLK_MENU, keyText);
	STRING_TO_ENUM(SDLK_SELECT, keyText);
	STRING_TO_ENUM(SDLK_STOP, keyText);
	STRING_TO_ENUM(SDLK_AGAIN, keyText);
	STRING_TO_ENUM(SDLK_UNDO, keyText);
	STRING_TO_ENUM(SDLK_CUT, keyText);
	STRING_TO_ENUM(SDLK_COPY, keyText);
	STRING_TO_ENUM(SDLK_PASTE, keyText);
	STRING_TO_ENUM(SDLK_FIND, keyText);
	STRING_TO_ENUM(SDLK_MUTE, keyText);
	STRING_TO_ENUM(SDLK_VOLUMEUP, keyText);
	STRING_TO_ENUM(SDLK_VOLUMEDOWN, keyText);
	STRING_TO_ENUM(SDLK_KP_COMMA, keyText);
	STRING_TO_ENUM(SDLK_KP_EQUALSAS400, keyText);

	STRING_TO_ENUM(SDLK_ALTERASE, keyText);
	STRING_TO_ENUM(SDLK_SYSREQ, keyText);
	STRING_TO_ENUM(SDLK_CANCEL, keyText);
	STRING_TO_ENUM(SDLK_CLEAR, keyText);
	STRING_TO_ENUM(SDLK_PRIOR, keyText);
	STRING_TO_ENUM(SDLK_RETURN2, keyText);
	STRING_TO_ENUM(SDLK_SEPARATOR, keyText);
	STRING_TO_ENUM(SDLK_OUT, keyText);
	STRING_TO_ENUM(SDLK_OPER, keyText);
	STRING_TO_ENUM(SDLK_CLEARAGAIN, keyText);
	STRING_TO_ENUM(SDLK_CRSEL, keyText);
	STRING_TO_ENUM(SDLK_EXSEL, keyText);

	STRING_TO_ENUM(SDLK_KP_00, keyText);
	STRING_TO_ENUM(SDLK_KP_000, keyText);
	STRING_TO_ENUM(SDLK_THOUSANDSSEPARATOR, keyText);
	STRING_TO_ENUM(SDLK_DECIMALSEPARATOR, keyText);
	STRING_TO_ENUM(SDLK_CURRENCYUNIT, keyText);
	STRING_TO_ENUM(SDLK_CURRENCYSUBUNIT, keyText);
	STRING_TO_ENUM(SDLK_KP_LEFTPAREN, keyText);
	STRING_TO_ENUM(SDLK_KP_RIGHTPAREN, keyText);
	STRING_TO_ENUM(SDLK_KP_LEFTBRACE, keyText);
	STRING_TO_ENUM(SDLK_KP_RIGHTBRACE, keyText);
	STRING_TO_ENUM(SDLK_KP_TAB, keyText);
	STRING_TO_ENUM(SDLK_KP_BACKSPACE, keyText);
	STRING_TO_ENUM(SDLK_KP_A, keyText);
	STRING_TO_ENUM(SDLK_KP_B, keyText);
	STRING_TO_ENUM(SDLK_KP_C, keyText);
	STRING_TO_ENUM(SDLK_KP_D, keyText);
	STRING_TO_ENUM(SDLK_KP_E, keyText);
	STRING_TO_ENUM(SDLK_KP_F, keyText);
	STRING_TO_ENUM(SDLK_KP_XOR, keyText);
	STRING_TO_ENUM(SDLK_KP_POWER, keyText);
	STRING_TO_ENUM(SDLK_KP_PERCENT, keyText);
	STRING_TO_ENUM(SDLK_KP_LESS, keyText);
	STRING_TO_ENUM(SDLK_KP_GREATER, keyText);
	STRING_TO_ENUM(SDLK_KP_AMPERSAND, keyText);
	STRING_TO_ENUM(SDLK_KP_DBLAMPERSAND, keyText);
	STRING_TO_ENUM(SDLK_KP_VERTICALBAR, keyText);
	STRING_TO_ENUM(SDLK_KP_DBLVERTICALBAR, keyText);
	STRING_TO_ENUM(SDLK_KP_COLON, keyText);
	STRING_TO_ENUM(SDLK_KP_HASH, keyText);
	STRING_TO_ENUM(SDLK_KP_SPACE, keyText);
	STRING_TO_ENUM(SDLK_KP_AT, keyText);
	STRING_TO_ENUM(SDLK_KP_EXCLAM, keyText);
	STRING_TO_ENUM(SDLK_KP_MEMSTORE, keyText);
	STRING_TO_ENUM(SDLK_KP_MEMRECALL, keyText);
	STRING_TO_ENUM(SDLK_KP_MEMCLEAR, keyText);
	STRING_TO_ENUM(SDLK_KP_MEMADD, keyText);
	STRING_TO_ENUM(SDLK_KP_MEMSUBTRACT, keyText);
	STRING_TO_ENUM(SDLK_KP_MEMMULTIPLY, keyText);
	STRING_TO_ENUM(SDLK_KP_MEMDIVIDE, keyText);
	STRING_TO_ENUM(SDLK_KP_PLUSMINUS, keyText);
	STRING_TO_ENUM(SDLK_KP_CLEAR, keyText);
	STRING_TO_ENUM(SDLK_KP_CLEARENTRY, keyText);
	STRING_TO_ENUM(SDLK_KP_BINARY, keyText);
	STRING_TO_ENUM(SDLK_KP_OCTAL, keyText);
	STRING_TO_ENUM(SDLK_KP_DECIMAL, keyText);
	STRING_TO_ENUM(SDLK_KP_HEXADECIMAL, keyText);

	STRING_TO_ENUM(SDLK_LCTRL, keyText);
	STRING_TO_ENUM(SDLK_LSHIFT, keyText);
	STRING_TO_ENUM(SDLK_LALT, keyText);
	STRING_TO_ENUM(SDLK_LGUI, keyText);
	STRING_TO_ENUM(SDLK_RCTRL, keyText);
	STRING_TO_ENUM(SDLK_RSHIFT, keyText);
	STRING_TO_ENUM(SDLK_RALT, keyText);
	STRING_TO_ENUM(SDLK_RGUI, keyText);

	STRING_TO_ENUM(SDLK_MODE, keyText);

	STRING_TO_ENUM(SDLK_AUDIONEXT, keyText);
	STRING_TO_ENUM(SDLK_AUDIOPREV, keyText);
	STRING_TO_ENUM(SDLK_AUDIOSTOP, keyText);
	STRING_TO_ENUM(SDLK_AUDIOPLAY, keyText);
	STRING_TO_ENUM(SDLK_AUDIOMUTE, keyText);
	STRING_TO_ENUM(SDLK_MEDIASELECT, keyText);
	STRING_TO_ENUM(SDLK_WWW, keyText);
	STRING_TO_ENUM(SDLK_MAIL, keyText);
	STRING_TO_ENUM(SDLK_CALCULATOR, keyText);
	STRING_TO_ENUM(SDLK_COMPUTER, keyText);
	STRING_TO_ENUM(SDLK_AC_SEARCH, keyText);
	STRING_TO_ENUM(SDLK_AC_HOME, keyText);
	STRING_TO_ENUM(SDLK_AC_BACK, keyText);
	STRING_TO_ENUM(SDLK_AC_FORWARD, keyText);
	STRING_TO_ENUM(SDLK_AC_STOP, keyText);
	STRING_TO_ENUM(SDLK_AC_REFRESH, keyText);
	STRING_TO_ENUM(SDLK_AC_BOOKMARKS, keyText);

	STRING_TO_ENUM(SDLK_BRIGHTNESSDOWN, keyText);
	STRING_TO_ENUM(SDLK_BRIGHTNESSUP, keyText);
	STRING_TO_ENUM(SDLK_DISPLAYSWITCH, keyText);
	STRING_TO_ENUM(SDLK_KBDILLUMTOGGLE, keyText);
	STRING_TO_ENUM(SDLK_KBDILLUMDOWN, keyText);
	STRING_TO_ENUM(SDLK_KBDILLUMUP, keyText);
	STRING_TO_ENUM(SDLK_EJECT, keyText);
	STRING_TO_ENUM(SDLK_SLEEP, keyText);
	return SDLK_UNKNOWN;
}

ActionId Input_stringToActionId(const char* actionIdText) {
	STRING_TO_ENUM(horizontal, actionIdText)
	STRING_TO_ENUM(vertical, actionIdText)
	STRING_TO_ENUM(attackSword, actionIdText)
	STRING_TO_ENUM(attackBow, actionIdText)
	STRING_TO_ENUM(jump, actionIdText)
	STRING_TO_ENUM(action, actionIdText)
	return none;
}

// Warning you will own the result and have to free it
// TODO make sure this doesn't kill everything
// too much strcpy malloc magic
char* Input_keycodeToPrintable(SDL_Keycode key) {
	const char* raw = Input_keycodeToConstantName(key);
	char* formatted = NULL;
	const char* keyPrefix = "SDLK_";
	if (strncmp(keyPrefix, raw, strlen(keyPrefix)) == 0) {
		const char* numPrefix = "SDLK_KP_";
		if (strncmp(numPrefix, raw, strlen(numPrefix)) == 0) {
			formatted = malloc((1+strlen(raw) - strlen(numPrefix))* sizeof(char));
			strcpy(formatted, raw + strlen(numPrefix));
		} else {
			formatted = malloc((5+strlen(raw) - strlen(keyPrefix))* sizeof(char));
			sprintf(formatted, "NUM_%s", raw + strlen(keyPrefix));
		}
	} else {
		formatted = malloc((1+strlen(raw))* sizeof(char));
		strcpy(formatted, raw);
	}
	return formatted;
}

const char* Input_keycodeToConstantName(SDL_Keycode keyText) {
	ENUM_TO_STRING(SDLK_RETURN, keyText);
	ENUM_TO_STRING(SDLK_ESCAPE, keyText);
	ENUM_TO_STRING(SDLK_BACKSPACE, keyText);
	ENUM_TO_STRING(SDLK_TAB, keyText);
	ENUM_TO_STRING(SDLK_SPACE, keyText);
	ENUM_TO_STRING(SDLK_EXCLAIM, keyText);
	ENUM_TO_STRING(SDLK_QUOTEDBL, keyText);
	ENUM_TO_STRING(SDLK_HASH, keyText);
	ENUM_TO_STRING(SDLK_PERCENT, keyText);
	ENUM_TO_STRING(SDLK_DOLLAR, keyText);
	ENUM_TO_STRING(SDLK_AMPERSAND, keyText);
	ENUM_TO_STRING(SDLK_QUOTE, keyText);
	ENUM_TO_STRING(SDLK_LEFTPAREN, keyText);
	ENUM_TO_STRING(SDLK_RIGHTPAREN, keyText);
	ENUM_TO_STRING(SDLK_ASTERISK, keyText);
	ENUM_TO_STRING(SDLK_PLUS, keyText);
	ENUM_TO_STRING(SDLK_COMMA, keyText);
	ENUM_TO_STRING(SDLK_MINUS, keyText);
	ENUM_TO_STRING(SDLK_PERIOD, keyText);
	ENUM_TO_STRING(SDLK_SLASH, keyText);
	ENUM_TO_STRING(SDLK_0, keyText);
	ENUM_TO_STRING(SDLK_1, keyText);
	ENUM_TO_STRING(SDLK_2, keyText);
	ENUM_TO_STRING(SDLK_3, keyText);
	ENUM_TO_STRING(SDLK_4, keyText);
	ENUM_TO_STRING(SDLK_5, keyText);
	ENUM_TO_STRING(SDLK_6, keyText);
	ENUM_TO_STRING(SDLK_7, keyText);
	ENUM_TO_STRING(SDLK_8, keyText);
	ENUM_TO_STRING(SDLK_9, keyText);
	ENUM_TO_STRING(SDLK_COLON, keyText);
	ENUM_TO_STRING(SDLK_SEMICOLON, keyText);
	ENUM_TO_STRING(SDLK_LESS, keyText);
	ENUM_TO_STRING(SDLK_EQUALS, keyText);
	ENUM_TO_STRING(SDLK_GREATER, keyText);
	ENUM_TO_STRING(SDLK_QUESTION, keyText);
	ENUM_TO_STRING(SDLK_AT, keyText);
	ENUM_TO_STRING(SDLK_LEFTBRACKET, keyText);
	ENUM_TO_STRING(SDLK_BACKSLASH, keyText);
	ENUM_TO_STRING(SDLK_RIGHTBRACKET, keyText);
	ENUM_TO_STRING(SDLK_CARET, keyText);
	ENUM_TO_STRING(SDLK_UNDERSCORE, keyText);
	ENUM_TO_STRING(SDLK_BACKQUOTE, keyText);
	ENUM_TO_STRING(SDLK_a, keyText);
	ENUM_TO_STRING(SDLK_b, keyText);
	ENUM_TO_STRING(SDLK_c, keyText);
	ENUM_TO_STRING(SDLK_d, keyText);
	ENUM_TO_STRING(SDLK_e, keyText);
	ENUM_TO_STRING(SDLK_f, keyText);
	ENUM_TO_STRING(SDLK_g, keyText);
	ENUM_TO_STRING(SDLK_h, keyText);
	ENUM_TO_STRING(SDLK_i, keyText);
	ENUM_TO_STRING(SDLK_j, keyText);
	ENUM_TO_STRING(SDLK_k, keyText);
	ENUM_TO_STRING(SDLK_l, keyText);
	ENUM_TO_STRING(SDLK_m, keyText);
	ENUM_TO_STRING(SDLK_n, keyText);
	ENUM_TO_STRING(SDLK_o, keyText);
	ENUM_TO_STRING(SDLK_p, keyText);
	ENUM_TO_STRING(SDLK_q, keyText);
	ENUM_TO_STRING(SDLK_r, keyText);
	ENUM_TO_STRING(SDLK_s, keyText);
	ENUM_TO_STRING(SDLK_t, keyText);
	ENUM_TO_STRING(SDLK_u, keyText);
	ENUM_TO_STRING(SDLK_v, keyText);
	ENUM_TO_STRING(SDLK_w, keyText);
	ENUM_TO_STRING(SDLK_x, keyText);
	ENUM_TO_STRING(SDLK_y, keyText);
	ENUM_TO_STRING(SDLK_z, keyText);

	ENUM_TO_STRING(SDLK_CAPSLOCK, keyText);

	ENUM_TO_STRING(SDLK_F1, keyText);
	ENUM_TO_STRING(SDLK_F2, keyText);
	ENUM_TO_STRING(SDLK_F3, keyText);
	ENUM_TO_STRING(SDLK_F4, keyText);
	ENUM_TO_STRING(SDLK_F5, keyText);
	ENUM_TO_STRING(SDLK_F6, keyText);
	ENUM_TO_STRING(SDLK_F7, keyText);
	ENUM_TO_STRING(SDLK_F8, keyText);
	ENUM_TO_STRING(SDLK_F9, keyText);
	ENUM_TO_STRING(SDLK_F10, keyText);
	ENUM_TO_STRING(SDLK_F11, keyText);
	ENUM_TO_STRING(SDLK_F12, keyText);

	ENUM_TO_STRING(SDLK_PRINTSCREEN, keyText);
	ENUM_TO_STRING(SDLK_SCROLLLOCK, keyText);
	ENUM_TO_STRING(SDLK_PAUSE, keyText);
	ENUM_TO_STRING(SDLK_INSERT, keyText);
	ENUM_TO_STRING(SDLK_HOME, keyText);
	ENUM_TO_STRING(SDLK_PAGEUP, keyText);
	ENUM_TO_STRING(SDLK_DELETE, keyText);
	ENUM_TO_STRING(SDLK_END, keyText);
	ENUM_TO_STRING(SDLK_PAGEDOWN, keyText);
	ENUM_TO_STRING(SDLK_RIGHT, keyText);
	ENUM_TO_STRING(SDLK_LEFT, keyText);
	ENUM_TO_STRING(SDLK_DOWN, keyText);
	ENUM_TO_STRING(SDLK_UP, keyText);

	ENUM_TO_STRING(SDLK_NUMLOCKCLEAR, keyText);
	ENUM_TO_STRING(SDLK_KP_DIVIDE, keyText);
	ENUM_TO_STRING(SDLK_KP_MULTIPLY, keyText);
	ENUM_TO_STRING(SDLK_KP_MINUS, keyText);
	ENUM_TO_STRING(SDLK_KP_PLUS, keyText);
	ENUM_TO_STRING(SDLK_KP_ENTER, keyText);
	ENUM_TO_STRING(SDLK_KP_1, keyText);
	ENUM_TO_STRING(SDLK_KP_2, keyText);
	ENUM_TO_STRING(SDLK_KP_3, keyText);
	ENUM_TO_STRING(SDLK_KP_4, keyText);
	ENUM_TO_STRING(SDLK_KP_5, keyText);
	ENUM_TO_STRING(SDLK_KP_6, keyText);
	ENUM_TO_STRING(SDLK_KP_7, keyText);
	ENUM_TO_STRING(SDLK_KP_8, keyText);
	ENUM_TO_STRING(SDLK_KP_9, keyText);
	ENUM_TO_STRING(SDLK_KP_0, keyText);
	ENUM_TO_STRING(SDLK_KP_PERIOD, keyText);

	ENUM_TO_STRING(SDLK_APPLICATION, keyText);
	ENUM_TO_STRING(SDLK_POWER, keyText);
	ENUM_TO_STRING(SDLK_KP_EQUALS, keyText);
	ENUM_TO_STRING(SDLK_F13, keyText);
	ENUM_TO_STRING(SDLK_F14, keyText);
	ENUM_TO_STRING(SDLK_F15, keyText);
	ENUM_TO_STRING(SDLK_F16, keyText);
	ENUM_TO_STRING(SDLK_F17, keyText);
	ENUM_TO_STRING(SDLK_F18, keyText);
	ENUM_TO_STRING(SDLK_F19, keyText);
	ENUM_TO_STRING(SDLK_F20, keyText);
	ENUM_TO_STRING(SDLK_F21, keyText);
	ENUM_TO_STRING(SDLK_F22, keyText);
	ENUM_TO_STRING(SDLK_F23, keyText);
	ENUM_TO_STRING(SDLK_F24, keyText);
	ENUM_TO_STRING(SDLK_EXECUTE, keyText);
	ENUM_TO_STRING(SDLK_HELP, keyText);
	ENUM_TO_STRING(SDLK_MENU, keyText);
	ENUM_TO_STRING(SDLK_SELECT, keyText);
	ENUM_TO_STRING(SDLK_STOP, keyText);
	ENUM_TO_STRING(SDLK_AGAIN, keyText);
	ENUM_TO_STRING(SDLK_UNDO, keyText);
	ENUM_TO_STRING(SDLK_CUT, keyText);
	ENUM_TO_STRING(SDLK_COPY, keyText);
	ENUM_TO_STRING(SDLK_PASTE, keyText);
	ENUM_TO_STRING(SDLK_FIND, keyText);
	ENUM_TO_STRING(SDLK_MUTE, keyText);
	ENUM_TO_STRING(SDLK_VOLUMEUP, keyText);
	ENUM_TO_STRING(SDLK_VOLUMEDOWN, keyText);
	ENUM_TO_STRING(SDLK_KP_COMMA, keyText);
	ENUM_TO_STRING(SDLK_KP_EQUALSAS400, keyText);

	ENUM_TO_STRING(SDLK_ALTERASE, keyText);
	ENUM_TO_STRING(SDLK_SYSREQ, keyText);
	ENUM_TO_STRING(SDLK_CANCEL, keyText);
	ENUM_TO_STRING(SDLK_CLEAR, keyText);
	ENUM_TO_STRING(SDLK_PRIOR, keyText);
	ENUM_TO_STRING(SDLK_RETURN2, keyText);
	ENUM_TO_STRING(SDLK_SEPARATOR, keyText);
	ENUM_TO_STRING(SDLK_OUT, keyText);
	ENUM_TO_STRING(SDLK_OPER, keyText);
	ENUM_TO_STRING(SDLK_CLEARAGAIN, keyText);
	ENUM_TO_STRING(SDLK_CRSEL, keyText);
	ENUM_TO_STRING(SDLK_EXSEL, keyText);

	ENUM_TO_STRING(SDLK_KP_00, keyText);
	ENUM_TO_STRING(SDLK_KP_000, keyText);
	ENUM_TO_STRING(SDLK_THOUSANDSSEPARATOR, keyText);
	ENUM_TO_STRING(SDLK_DECIMALSEPARATOR, keyText);
	ENUM_TO_STRING(SDLK_CURRENCYUNIT, keyText);
	ENUM_TO_STRING(SDLK_CURRENCYSUBUNIT, keyText);
	ENUM_TO_STRING(SDLK_KP_LEFTPAREN, keyText);
	ENUM_TO_STRING(SDLK_KP_RIGHTPAREN, keyText);
	ENUM_TO_STRING(SDLK_KP_LEFTBRACE, keyText);
	ENUM_TO_STRING(SDLK_KP_RIGHTBRACE, keyText);
	ENUM_TO_STRING(SDLK_KP_TAB, keyText);
	ENUM_TO_STRING(SDLK_KP_BACKSPACE, keyText);
	ENUM_TO_STRING(SDLK_KP_A, keyText);
	ENUM_TO_STRING(SDLK_KP_B, keyText);
	ENUM_TO_STRING(SDLK_KP_C, keyText);
	ENUM_TO_STRING(SDLK_KP_D, keyText);
	ENUM_TO_STRING(SDLK_KP_E, keyText);
	ENUM_TO_STRING(SDLK_KP_F, keyText);
	ENUM_TO_STRING(SDLK_KP_XOR, keyText);
	ENUM_TO_STRING(SDLK_KP_POWER, keyText);
	ENUM_TO_STRING(SDLK_KP_PERCENT, keyText);
	ENUM_TO_STRING(SDLK_KP_LESS, keyText);
	ENUM_TO_STRING(SDLK_KP_GREATER, keyText);
	ENUM_TO_STRING(SDLK_KP_AMPERSAND, keyText);
	ENUM_TO_STRING(SDLK_KP_DBLAMPERSAND, keyText);
	ENUM_TO_STRING(SDLK_KP_VERTICALBAR, keyText);
	ENUM_TO_STRING(SDLK_KP_DBLVERTICALBAR, keyText);
	ENUM_TO_STRING(SDLK_KP_COLON, keyText);
	ENUM_TO_STRING(SDLK_KP_HASH, keyText);
	ENUM_TO_STRING(SDLK_KP_SPACE, keyText);
	ENUM_TO_STRING(SDLK_KP_AT, keyText);
	ENUM_TO_STRING(SDLK_KP_EXCLAM, keyText);
	ENUM_TO_STRING(SDLK_KP_MEMSTORE, keyText);
	ENUM_TO_STRING(SDLK_KP_MEMRECALL, keyText);
	ENUM_TO_STRING(SDLK_KP_MEMCLEAR, keyText);
	ENUM_TO_STRING(SDLK_KP_MEMADD, keyText);
	ENUM_TO_STRING(SDLK_KP_MEMSUBTRACT, keyText);
	ENUM_TO_STRING(SDLK_KP_MEMMULTIPLY, keyText);
	ENUM_TO_STRING(SDLK_KP_MEMDIVIDE, keyText);
	ENUM_TO_STRING(SDLK_KP_PLUSMINUS, keyText);
	ENUM_TO_STRING(SDLK_KP_CLEAR, keyText);
	ENUM_TO_STRING(SDLK_KP_CLEARENTRY, keyText);
	ENUM_TO_STRING(SDLK_KP_BINARY, keyText);
	ENUM_TO_STRING(SDLK_KP_OCTAL, keyText);
	ENUM_TO_STRING(SDLK_KP_DECIMAL, keyText);
	ENUM_TO_STRING(SDLK_KP_HEXADECIMAL, keyText);

	ENUM_TO_STRING(SDLK_LCTRL, keyText);
	ENUM_TO_STRING(SDLK_LSHIFT, keyText);
	ENUM_TO_STRING(SDLK_LALT, keyText);
	ENUM_TO_STRING(SDLK_LGUI, keyText);
	ENUM_TO_STRING(SDLK_RCTRL, keyText);
	ENUM_TO_STRING(SDLK_RSHIFT, keyText);
	ENUM_TO_STRING(SDLK_RALT, keyText);
	ENUM_TO_STRING(SDLK_RGUI, keyText);

	ENUM_TO_STRING(SDLK_MODE, keyText);

	ENUM_TO_STRING(SDLK_AUDIONEXT, keyText);
	ENUM_TO_STRING(SDLK_AUDIOPREV, keyText);
	ENUM_TO_STRING(SDLK_AUDIOSTOP, keyText);
	ENUM_TO_STRING(SDLK_AUDIOPLAY, keyText);
	ENUM_TO_STRING(SDLK_AUDIOMUTE, keyText);
	ENUM_TO_STRING(SDLK_MEDIASELECT, keyText);
	ENUM_TO_STRING(SDLK_WWW, keyText);
	ENUM_TO_STRING(SDLK_MAIL, keyText);
	ENUM_TO_STRING(SDLK_CALCULATOR, keyText);
	ENUM_TO_STRING(SDLK_COMPUTER, keyText);
	ENUM_TO_STRING(SDLK_AC_SEARCH, keyText);
	ENUM_TO_STRING(SDLK_AC_HOME, keyText);
	ENUM_TO_STRING(SDLK_AC_BACK, keyText);
	ENUM_TO_STRING(SDLK_AC_FORWARD, keyText);
	ENUM_TO_STRING(SDLK_AC_STOP, keyText);
	ENUM_TO_STRING(SDLK_AC_REFRESH, keyText);
	ENUM_TO_STRING(SDLK_AC_BOOKMARKS, keyText);

	ENUM_TO_STRING(SDLK_BRIGHTNESSDOWN, keyText);
	ENUM_TO_STRING(SDLK_BRIGHTNESSUP, keyText);
	ENUM_TO_STRING(SDLK_DISPLAYSWITCH, keyText);
	ENUM_TO_STRING(SDLK_KBDILLUMTOGGLE, keyText);
	ENUM_TO_STRING(SDLK_KBDILLUMDOWN, keyText);
	ENUM_TO_STRING(SDLK_KBDILLUMUP, keyText);
	ENUM_TO_STRING(SDLK_EJECT, keyText);
	ENUM_TO_STRING(SDLK_SLEEP, keyText);
	return "unknown";
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
