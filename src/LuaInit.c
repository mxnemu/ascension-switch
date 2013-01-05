#include "LuaInit.h"

#include "Scene.h"
#include "Trigger.h"

void LuaInit_initCustomTypes(lua_State* l) {
	Scene_luaExport(l);
	Trigger_exportToLua(l);
	printf("initialized custom Lua Types\n");
}
