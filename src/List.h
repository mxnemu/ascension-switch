#pragma once
#include "Constants.h"

CLASS(ListNode) {
	ListNode* previous;
	ListNode* next;
	void* data; // more void pointers baby!
};

CLASS(List) {
	ListNode* first;
	ListNode* last;
};

List* List_create();
void List_destroy(List* this);

void List_pushBack(List* this, void* data);
void List_pushFront(List* this, void* data);
void List_eraseByData(List* this, void* data);
int List_getIndexForData(List* this, void* data);
ListNode* List_getNodeForData(List* this, void* data);


ListNode* ListNode_create();
void ListNode_destroy(ListNode* this); ///< warning this does not delete data
