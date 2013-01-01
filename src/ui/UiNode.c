#include "UiNode.h"

UiNode* UiNode_create(void* context, UiNode* parent) {
	UiNode* this = malloc(sizeof(UiNode));
	this->parent = parent;
	this->context = context;
	this->children = Vector_Create();

	this->destroy = UiNode_emptyDestroy;
	this->draw = UiNode_emptyDraw;
	this->handleEvent = UiNode_emptyHandleEvent;
	this->resize = UiNode_emptyResize;

	return this;
}

void UiNode_destroy(UiNode* this) {
	if (this->parent) {
		UiNode_removeChild(this->parent, this);
	}
	free(this);
}

void UiNode_draw(UiNode* this, SDL_Surface* surface) {
	this->draw(this->context, surface);
	for (unsigned int i=0; i < this->children->allocatedElements; ++i) {
		UiNode* it = this->children->elements[i];
		if (it != NULL) {
			UiNode_draw(it, surface);
		}
	}
}

void UiNode_handleEvent(UiNode* this, SDL_Event* event) {
	if (this->handleEvent(this->context, event)) {
		for (unsigned int i=0; i < this->children->allocatedElements; ++i) {
			UiNode* it = this->children->elements[i];
			if (it != NULL) {
				UiNode_handleEvent(it, event);
				//TODO maybe stop forwarding to neighbor nodes, if one node marks it as handled
				// atm only child nodes won't be notified when a node marks an event as handled
			}
		}
	}
}

void UiNode_removeChild(UiNode* this, UiNode* child) {
	for (unsigned int i=0; i < this->children->allocatedElements; ++i) {
		UiNode* it = this->children->elements[i];
		if (it == child) {
			Vector_Set(this->children, i, NULL);
		}
	}
}


void UiNode_emptyDestroy(void* context) {}
void UiNode_emptyDraw(void* context, SDL_Surface* surface) {}
bool UiNode_emptyHandleEvent(void* context, SDL_Event* event) { return true; }
void UiNode_emptyResize(void* context, SDL_Surface* surface) {}
