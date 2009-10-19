#ifndef RefArray_H
#define RefArray_H

#include "RefObject.h"

O_METHOD_DEF(RefArray, void *, resize, (void *_self, unsigned capacity));
O_METHOD_DEF(RefArray, void *, set, (void *_self, unsigned index, void *_item));
O_METHOD_DEF(RefArray, void *, get, (void *_self, unsigned index));

#define RefArrayClass_Attr				\
  RefObjectClass_Attr;				\
  O_METHOD(RefArray, resize);			\
  O_METHOD(RefArray, set);				\
  O_METHOD(RefArray, get)

#define RefArray_Attr				\
  RefObject_Attr;				\
  unsigned capacity;				\
  struct RefObject ** data;			\
  struct Class * type

O_CLASS(RefArray, RefObject);

#endif /* RefArray_H */
