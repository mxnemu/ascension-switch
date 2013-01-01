#include "List.h"

List* List_create() {
	List* this = malloc(sizeof(List));
	this->first = NULL;
	this->last = NULL;
	return this;
}

void List_destroy(List* this) {
	ListNode* it = this->first;
	while (it) {
		ListNode* next = it->next;
		ListNode_destroy(it);
		it = next;
	}
	free(this);
}

void List_pushBack(List* this, void* data) {
	ListNode* newNode = ListNode_create(this->last, data);
	if (!this->first) {
		this->first = newNode;
	}
	this->last = newNode;
}

void List_pushFront(List* this, void* data) {
	ListNode* newNode = ListNode_create(NULL, data);
	newNode->next = this->first;
	if (!this->last) {
		this->last = newNode;
	}
	this->first = newNode;
}

void List_eraseByData(List* this, void* data) {
	ListNode* it = this->first;
	while (it) {
		if (it->data == data) {
			if (it->previous) {
				it->previous->next = it->next;
			}
			if (it->next) {
				it->next->previous = it->previous;
			}
			if (it == this->first) {
				this->first = it->next;
			}
			if (it == this->last) {
				this->last = it->previous;
			}
			ListNode_destroy(it);
			return;
		}
		it = it->next;
	}
}

int List_getIndexForData(List* this, void* data) {
	int i = 0;
	ListNode* it = this->first;
	while (it) {
		if (it->data == data) {
			return i;
		}
		++i;
		it = it->next;
	}
	return -1;
}

ListNode* List_getNodeForData(List* this, void* data) {
	ListNode* it = this->first;
	while (it) {
		if (it->data == data) {
			return it;
		}
		it = it->next;
	}
	return NULL;
}


ListNode* ListNode_create(ListNode* previous, void* data) {
	ListNode* this = malloc(sizeof(ListNode));
	this->previous = previous;
	this->next = NULL;
	this->data = data;
	if (previous) {
		previous->next = this;
	}
	return this;
}

void ListNode_destroy(ListNode* this) {
	free(this);
}
