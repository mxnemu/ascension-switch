
#include "Vector.h"

// public
Vector* Vector_Create(void) {
    Vector* this = malloc(sizeof(Vector));
	this->elements = NULL;
    this->usedElements = 0;
    this->_allocatedElements = 0;
    return this;
}

void Vector_AddElement(Vector* this, ElementType element) {
    if (this->_allocatedElements <= this->usedElements) {
        _Vector_RecreateWithoutSizeCheck(this, this->_allocatedElements + CacheIncreaseRate);
    }
    _Vector_AddElementWithoutSizeCheck(this, element);
}

void Vector_InsertInFirstFreeSpace(Vector* this, ElementType element) {
	for (int i=0; i < this->usedElements; ++i) {
		if (this->elements[i] == NULL) {
			this->elements[i] = element;
			return;
		}
	}

	Vector_AddElement(this, element);
}

ElementType Vector_getLastElement(Vector* this) {
	for (int i=0; i < this->usedElements; ++i) {
		if (this->elements[i] != NULL) {
			return this->elements[i];
		}
	}
	return NULL;
}

void Vector_ShrinkCache(Vector* this, bool maximumShrink) {
    if (this->_allocatedElements - CacheIncreaseRate > this->usedElements) {
        _Vector_RecreateWithoutSizeCheck(this, this->usedElements+CacheIncreaseRate);
    } else if (maximumShrink && this->_allocatedElements > this->usedElements) {
        _Vector_RecreateWithoutSizeCheck(this, this->usedElements);
    }
}

ElementType Vector_Get(Vector* this, unsigned int i) {
    if (i < this->usedElements) {
        return this->elements[i];
    }
    return NULL;
}

void Vector_Set(Vector* this, unsigned int i, ElementType element) {
    if (i < this->usedElements) {
        this->elements[i] = element;
    }
}

void Vector_Destroy(Vector* this) {
    if (this->elements) {
        free(this->elements);
    }
}

// private
void _Vector_AddElementWithoutSizeCheck(Vector* this, ElementType element) {
    this->elements[this->usedElements] = element;
    this->usedElements++;
}

/// newallocatedElements must be higher than this->usedElements to succeed
void _Vector_RecreateWithoutSizeCheck(Vector* this, unsigned int newallocatedElements) {
    ElementType* copy = malloc(newallocatedElements * sizeof(ElementType));
    for (unsigned int i=0; i < this->usedElements; ++i) {
        copy[i] = this->elements[i];
    }
    free(this->elements);
    this->elements = copy;
    this->_allocatedElements = newallocatedElements;
}
