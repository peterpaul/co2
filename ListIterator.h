#ifndef LISTITERATOR_H
#define LISTITERATOR_H

#include "Iterator.h"
#include "List.h"

#define ListIteratorClass_Attr			\
	ObjectClass_Attr;			\
	O_METHOD(Iterator, getNext);		\
	O_METHOD(Iterator, hasNext)

#define ListIterator_Attr			\
	Object_Attr;				\
	struct List * list;			\
	int pos

O_CLASS(ListIterator, Object);

#endif /* LISTITERATOR_H */
