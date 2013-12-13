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

int List_length(List* this) {
    int n = 0;
    for (ListNode* it = this->first; it; it = it->next) {
	++n;
    }
    return n;
}
void List_qSort(List* this, int (*compare)(void* a, void* b)) {
    ListNode* a = this->first;
    ListNode* b = this->last;
    if (a && b) {
	List_qSortIt(this, compare, a, b);
    }
}

void List_qSortIt(List* this, int (*compare)(void* a, void* b), ListNode* start, ListNode* end) {
    if (start == end) {
	return;
    }
    ListNode* it = start;
    ListNode* jt = end->previous ? end->previous : it; // skip the while hack
    void* pivot = end->data;

    while (it != jt) {
	while (1 != compare(it->data, pivot) && it != jt) {
	    it = it->next;
	}

	while (-1 != compare(jt->data, pivot) && jt != it) {
	    jt = jt->previous;
	}

	if (it != jt) {
	    void* swapCache = it->data;
	    it->data = jt->data;
	    jt->data = swapCache;
	}
    }

    if (1 == compare(it->data, pivot)) {
	void* swapCache = it->data;
	it->data = end->data;
	end->data = swapCache;
    }

    if (it->previous && start != it) {
	List_qSortIt(this, compare, start, it->previous);
    }
    if (it->next && end != it) {
	List_qSortIt(this, compare, it->next, end);
    }
}
