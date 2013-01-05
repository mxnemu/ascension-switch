#pragma once

#include "Constants.h"

void stackDump (lua_State *L);
void* lua_checklightuserdata(lua_State* l, int idx);
void lua_createLib(lua_State* l, const char* tableName, const char* globalName, const luaL_Reg* functions, const luaL_Reg* methods, lua_CFunction destructor);
void initRect(SDL_Rect* rect);


#define CHECK_LUA_USERCLASS(l, clazz, metatable, idx) \
	void *userData = luaL_checkudata(l, idx, metatable); \
	luaL_argcheck(l, (userData != NULL), idx, "'"STR(clazz)"' expected"); \
	return (clazz*) userData;


#define FREE_VECTOR_WITH_ELMENTS(vector, destructor) \
	for (int _i=0; _i < vector->allocatedElements; ++_i) { \
		void* it = vector->elements[_i]; \
		if (it != NULL) { \
			destructor(it); \
		} \
	}
