#include "UiNode.h"
#include "../Utils.h"

UiNode* UiNode_create(void* context, UiNode* parent) {
	UiNode* this = malloc(sizeof(UiNode));
	this->parent = parent;
	this->context = context;
	this->children = Vector_Create();
	this->callbackContext = NULL;

	this->destroy = UiNode_emptyDestroy;
	this->draw = UiNode_emptyDraw;
	this->handleEvent = UiNode_emptyHandleEvent;
	this->resize = UiNode_emptyResize;

	SDL_Rect_init(&this->bounds);

	this->selectable = true;
	this->selectedChild = NULL;

	if (parent) {
		Vector_AddElement(parent->children, this); // TODO call add funct
	}

	return this;
}

void UiNode_destroy(UiNode* this) {
	if (this->parent) {
		UiNode_removeChild(this->parent, this);
	}
	free(this);
}

void UiNode_draw(UiNode* this, SDL_Renderer* renderer) {
	this->draw(this->context, renderer);
	for (unsigned int i=0; i < this->children->usedElements; ++i) {
		UiNode* it = this->children->elements[i];
		if (it != NULL) {
			UiNode_draw(it, renderer);
		}
	}
}

bool UiNode_handleEvent(UiNode* this, SDL_Event* event) {
	if (this->handleEvent(this->context, event)) {
		for (unsigned int i=0; i < this->children->usedElements; ++i) {
			UiNode* it = this->children->elements[i];
			if (it != NULL) {
				if (!UiNode_handleEvent(it, event)) {
					return false;
				}
			}
		}
	} else {
		return false;
	}

	// Default actions if the event wasn't canceled until now
	UiNode_eventDefaultActions(this, event);
	return true;
}

void UiNode_eventDefaultActions(UiNode* this, SDL_Event* event) {
	if (this->selectable) {
		if (event->type == SDL_MOUSEBUTTONUP) {
			if (this->parent && SDL_Rect_isInside(&this->bounds, event->button.x, event->button.y)) {
				this->parent->selectedChild = this;
			}
		}
	}

	for (unsigned int i=0; i < this->children->usedElements; ++i) {
		UiNode* it = this->children->elements[i];
		if (it != NULL) {
			UiNode_eventDefaultActions(it, event);
		}
	}
}

void UiNode_removeChild(UiNode* this, UiNode* child) {
	for (unsigned int i=0; i < this->children->usedElements; ++i) {
		UiNode* it = this->children->elements[i];
		if (it == child) {
			Vector_Set(this->children, i, NULL);
		}
	}
}

void UiNode_moveTo(UiNode* this, int x, int y) {
	for (unsigned int i=0; i < this->children->usedElements; ++i) {
		UiNode* it = this->children->elements[i];
		if (it != NULL) {
			int tx = x+(it->bounds.x - this->bounds.x);
			int ty = y+(it->bounds.y - this->bounds.y);
			UiNode_moveTo(it, tx, ty);
		}
	}
	this->bounds.x = x;
	this->bounds.y = y;
}


void UiNode_emptyDestroy(void* context) {}
void UiNode_emptyDraw(void* context, SDL_Renderer* renderer) {}
bool UiNode_emptyHandleEvent(void* context, SDL_Event* event) { return true; }
void UiNode_emptyResize(void* context, SDL_Surface* surface) {}
