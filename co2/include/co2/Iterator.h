#ifndef ITERATOR_H
#define ITERATOR_H

#include "co2/Interface.h"

O_METHOD_DEF(Iterator, void *, getNext, (void *_self));
O_METHOD_DEF(Iterator, bool, hasNext, (void *_self));

#define IteratorClass_Attr			\
	InterfaceClass_Attr

#define Iterator_Attr				\
	Interface_Attr;				\
	O_METHOD(Iterator, getNext);		\
	O_METHOD(Iterator, hasNext)

O_CLASS(Iterator, Interface);

#endif /* ITERATOR_H */
