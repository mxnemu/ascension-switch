#pragma once

#include "Constants.h"

void stackDump (lua_State *L);
void* lua_checklightuserdata(lua_State* l, int idx);
