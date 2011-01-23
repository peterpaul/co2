#ifndef Array_H
#define Array_H

#include "co2/RefObject.h"

O_METHOD_DEF(Array, void *, resize, (void *_self, unsigned capacity));
O_METHOD_DEF(Array, void *, set, (void *_self, unsigned index, void *_item));
O_METHOD_DEF(Array, void *, get, (void *_self, unsigned index));

#define ArrayClass_Attr				\
  RefObjectClass_Attr;				\
  O_METHOD(Array, resize);			\
  O_METHOD(Array, set);				\
  O_METHOD(Array, get)

#define Array_Attr				\
  RefObject_Attr;				\
  unsigned capacity;				\
  void ** data;					\
  struct Class * type

O_CLASS(Array, RefObject);

#endif /* Array_H */
