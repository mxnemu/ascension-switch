#include "UiMenuList.h"
#include "../Utils.h"

UiMenuList* UiMenuList_create(UiNode* parent) {
	UiMenuList* this = malloc(sizeof(UiMenuList));
	this->node = UiNode_create(this, parent);
	this->node->destroy = UiMenuList_destroy;
	return this;
}

void UiMenuList_destroy(void* context) {
	UiMenuList* this = context;
	free(this);
}

void UiMenuList_addElement(UiMenuList* this, UiMenuElement* element) {
	element->node->bounds.x = this->node->bounds.x;
	element->node->bounds.y = this->node->bounds.y + this->node->bounds.h;

	this->node->bounds.w = MAX(this->node->bounds.w, element->node->bounds.w);
	this->node->bounds.h += element->node->bounds.h;
}

UiMenuElement* UiMenuElement_create(UiMenuList* list, const char* text, Sprite* icon, void (*actionCallback)(UiNode*, void*), void* callbackContext) {
	UiMenuElement* this = malloc(sizeof(UiMenuElement));
	this->node = UiNode_create(this, list->node);
	this->node->draw = UiMenuElement_draw;
	this->node->handleEvent = UiMenuElement_handleEvent;
	this->node->callbackContext = callbackContext;
	this->actionCallback = actionCallback;

	TTF_Font* font = TTF_OpenFont("fonts/Black-Chancery.ttf", 24);
	SDL_Color color = {.r=255, .g = 255, .b= 255};
	this->icon = Sprite_create(NULL);
	this->label = Sprite_create(TTF_RenderUTF8_Blended(font, text, color));
	TTF_CloseFont(font);

	// TODO won't somebody think about the icons?!
	this->node->bounds.h = this->label->bounds.h;
	this->node->bounds.w = this->label->bounds.w;

	if (list) {
		UiMenuList_addElement(list, this);
	}
	return this;
}

void UiMenuElement_destroy(void* context) {
	UiMenuElement* this = context;
	Sprite_destroy(this->icon);
	Sprite_destroy(this->label);
	free(this);
}

void UiMenuElement_draw(void* context, SDL_Surface* screen) {
	UiMenuElement* this = context;

	Sprite_drawRelative(this->label, screen, &this->node->bounds);
}

bool UiMenuElement_handleEvent(void* context, SDL_Event* event) {
	UiMenuElement* this = context;
	if (this->node->selectable) {
		if (event->type == SDL_MOUSEBUTTONUP) {
			if (this->node->parent && this->actionCallback && SDL_Rect_isInside(&this->node->bounds, event->button.x, event->button.y)) {
				this->actionCallback(this->node, this->node->callbackContext);
			}
		}
	}
	return true;
}
