#ifndef Array_H
#define Array_H

#include "Object.h"

O_METHOD_DEF(Array, void *, resize, (void *_self, int size));
O_METHOD_DEF(Array, void *, set, (void *_self, int index, void *_item));
O_METHOD_DEF(Array, void *, get, (void *_self, int index));

#define ArrayClass_Attr				\
  ObjectClass_Attr;				\
  O_METHOD(Array, resize);			\
  O_METHOD(Array, set);				\
  O_METHOD(Array, get)

#define Array_Attr				\
  Object_Attr;					\
  int size;					\
  void ** data

O_CLASS(Array, Object);

#endif /* Array_H */
