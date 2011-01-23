#ifndef LISTITERATOR_H
#define LISTITERATOR_H

#include "co2/Iterator.h"
#include "co2/List.h"

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
