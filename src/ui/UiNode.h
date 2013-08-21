#pragma once
#include "../Vector.h"
#include "../Constants.h"

CLASS(UiNode) {
	void (*destroy)(void* context);
	bool (*handleEvent)(void* context, SDL_Event* event);
	void (*resize)(void* context, SDL_Surface* surface);
	void (*draw)(void* context, SDL_Renderer* surface);

	bool selectable;
	UiNode* selectedChild;
	SDL_Rect bounds;

	void* callbackContext;
	void* context;
	UiNode* parent;
	Vector* children;
};

CLASS(UiCallbackContext) {
	void* context;
	UiNode* node;
};

UiNode* UiNode_create(void* context, UiNode* parent);
void UiNode_destroy(UiNode* this);

void UiNode_draw(UiNode* this, SDL_Renderer* renderer);
bool UiNode_handleEvent(UiNode* this, SDL_Event* event);
void UiNode_eventDefaultActions(UiNode* this, SDL_Event* event);
void UiNode_removeChild(UiNode* this, UiNode* child);
void UiNode_moveTo(UiNode* this, int x, int y);

void UiNode_emptyDestroy(void* context);
void UiNode_emptyDraw(void* context, SDL_Renderer* surface);
bool UiNode_emptyHandleEvent(void* context, SDL_Event* event);
void UiNode_emptyResize(void* context, SDL_Surface* surface);
