#pragma once

#include "Constants.h"

void stackDump (lua_State *L);
void* lua_checklightuserdata(lua_State* l, int idx);
void lua_createLib(lua_State* l, const char* tableName, const char* globalName, const luaL_Reg* functions, const luaL_Reg* methods, lua_CFunction destructor);
void lua_newlib(lua_State* l, const luaL_Reg* functions);

// SDL utils
void SDL_Rect_init(SDL_Rect* rect);
bool SDL_Rect_touches(SDL_Rect* a, SDL_Rect* b);
bool SDL_Rect_above(SDL_Rect* a, SDL_Rect* b);
bool SDL_Rect_isInside(SDL_Rect* a, int x, int y);


#define CHECK_LUA_USERCLASS(l, clazz, metatable, idx) \
	void *userData = luaL_checkudata(l, idx, metatable); \
	luaL_argcheck(l, (userData != NULL), idx, "'"STR(clazz)"' expected"); \
	return (clazz*) userData;


#define FREE_VECTOR_WITH_ELMENTS(vector, destructor) \
	for (int _i=0; _i < vector->usedElements; ++_i) { \
		void* it = vector->elements[_i]; \
		if (it != NULL) { \
			destructor(it); \
		} \
	} \
	Vector_Destroy(vector);

#define FREE_LIST_WITH_ELMENTS(list, destructor) \
	{ \
		ListNode* util_freeingNode = list->first; \
		while (util_freeingNode) { \
			void* util_data = util_freeingNode->data; \
			if (util_data != NULL) { \
				destructor(util_data); \
			} \
			util_freeingNode = util_freeingNode->next; \
		} \
		List_destroy(list); \
	}

#define STRING_TO_ENUM(e, s) if (strcmp(STR(e), s) == 0) { return e; }
#define ENUM_TO_STRING(e, v) if (e == v) { return STR(e); }

void emptyInit(void* context);
void emptyUpdate(void* context, RawTime dt);
void emptyDraw(void* context, SDL_Renderer* surface);
void emptyResize(void* context, SDL_Point size);
void emptyDestroy(void* context);
void emptyHandleEvent(void* context, SDL_Event*);
