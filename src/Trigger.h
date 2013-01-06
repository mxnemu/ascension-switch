/*
 * Trigger.h
 *
 *  Created on: 02.01.2013
 *      Author: nehmulos
 */
#pragma once

#include "Constants.h"
#include "Scene.h"

CLASS(Trigger) {
	Entity* entity;
	int luaCallbackReference;
};

void Trigger_exportToLua(lua_State*);
Trigger* Trigger_checkFromLua(lua_State*l, int idx);
int Trigger_luaCreate(lua_State*);
int Trigger_luaDestroy(lua_State*);

void Trigger_setScene(Trigger* this, Scene* scene);
