#pragma once
#include <malloc.h>

typedef struct ListNode ListNode;
struct ListNode {
    ListNode* previous;
    ListNode* next;
    void* data; // more void pointers baby!
};

typedef struct List List;
struct List {
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

int List_length(List* this);
void List_qSort(List* this, int (*compare)(void* a, void* b));
void List_qSortIt(List* this, int (*compare)(void* a, void* b), ListNode* a, ListNode* b);

ListNode* ListNode_create();
void ListNode_destroy(ListNode* this); ///< warning this does not delete data

#define List_destroyWithContent(this, contentType)                      \
    ListNode* _it = this->first;					\
    while (_it) {                                                       \
        contentType ## _destroy((struct contentType*)_it->data);        \
        _it = _it->next;                                                \
    }                                                                   \
    List_destroy(this);
