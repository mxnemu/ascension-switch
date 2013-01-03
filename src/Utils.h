#pragma once

#include "Constants.h"

void stackDump (lua_State *L);
void* lua_checklightuserdata(lua_State* l, int idx);
void lua_createLib(lua_State* l, const char* tableName, const char* globalName, const luaL_Reg* functions, const luaL_Reg* methods);
void initRect(SDL_Rect* rect);
