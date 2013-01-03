#include "LuaInit.h"

#include "Scene.h"

void LuaInit_initCustomTypes(lua_State* l) {
	Scene_luaExport(l);
	printf("initialized custom Lua Types\n");
}
