#include "Utils.h"

void stackDump (lua_State *L) {
  int i;
  int top = lua_gettop(L);
  printf("luastacksize:%d\n",top);
  for (i = 1; i <= top; i++) {  /* repeat for each level */
	int t = lua_type(L, i);
	switch (t) {

	  case LUA_TSTRING:  /* strings */
		printf("`%s'", lua_tostring(L, i));
		break;

	  case LUA_TBOOLEAN:  /* booleans */
		printf(lua_toboolean(L, i) ? "true" : "false");
		break;

	  case LUA_TNUMBER:  /* numbers */
		printf("%g", lua_tonumber(L, i));
		break;

	  default:  /* other values */
		printf("%s", lua_typename(L, t));
		break;

	}
	printf("  ");  /* put a separator */
  }
  printf("\n");  /* end the listing */
}


void* lua_checklightuserdata(lua_State* l, int idx){
	luaL_checktype(l, idx, LUA_TLIGHTUSERDATA);
	return lua_touserdata(l, idx);
}

void initRect(SDL_Rect* rect) {
	rect->x = 0;
	rect->y = 0;
	rect->w = 0;
	rect->h = 0;
}

// Thanks to user46874 on stackoverflow.com
// http://stackoverflow.com/a/11197262
void lua_createLib(lua_State* l, const char* name, const luaL_Reg* functions, const luaL_Reg* methods) {
    /* newclass = {} */
    lua_createtable(l, 0, 0);
    int lib_id = lua_gettop(l);

    /* metatable = {} */
    luaL_newmetatable(l, name);
    int meta_id = lua_gettop(l);
    luaL_setfuncs(l, functions, 0);

    /* metatable.__index = _methods */
    luaL_newlib(l, methods);
    lua_setfield(l, meta_id, "__index");

    /* metatable.__metatable = functions */
    luaL_newlib(l, functions);
    lua_setfield(l, meta_id, "__metatable");

    /* class.__metatable = metatable */
    lua_setmetatable(l, lib_id);

    /* _G["Foo"] = newclass */
    lua_setglobal(l, name);
}
