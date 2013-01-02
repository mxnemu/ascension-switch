#ifndef C_VECTOR_H 
#define C_VECTOR_H 

#define CacheIncreaseRate 8
#define ElementType void*

#include "stdbool.h"
#include "malloc.h"

#ifndef NULL
    #define NULL 0
#endif


typedef struct {
    ElementType* elements;
    unsigned int usedElements;
    unsigned int allocatedElements;
} Vector;

// public
Vector* Vector_Create(void);
void Vector_AddElement(Vector* this, ElementType element);
void Vector_InsertInFirstFreeSpace(Vector* this, ElementType element);
void Vector_ShrinkCache(Vector* this, bool maximumShrink);
ElementType Vector_Get(Vector* this, unsigned int i);
void Vector_Set(Vector* this, unsigned int i, ElementType element);
void Vector_Destroy(Vector* this); ///< free array of elements, but not the elements themself
// private
void _Vector_AddElementWithoutSizeCheck(Vector* this, ElementType element);
void _Vector_RecreateWithoutSizeCheck(Vector* this, unsigned int newallocatedElements);
#endif
