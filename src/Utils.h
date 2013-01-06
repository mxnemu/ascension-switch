#pragma once

#include "Constants.h"

void stackDump (lua_State *L);
void* lua_checklightuserdata(lua_State* l, int idx);
void lua_createLib(lua_State* l, const char* tableName, const char* globalName, const luaL_Reg* functions, const luaL_Reg* methods, lua_CFunction destructor);

// SDL utils
void SDL_Rect_init(SDL_Rect* rect);
bool SDL_Rect_touches(SDL_Rect* a, SDL_Rect* b);
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

#define ENUM_TO_STRING_MATCH(e, s) if (strcmp(STR(e), s) == 0) { return e; }

void emptyInit(void* context);
void emptyUpdate(void* context, RawTime dt);
void emptyDraw(void* context, SDL_Surface* surface);
void emptyDestroy(void* context);
void emptyHandleEvent(void* context, SDL_Event*);
