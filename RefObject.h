#ifndef REFOBJECT_H
#define REFOBJECT_H

#include "Object.h"
#include "ReleasePool.h"

O_METHOD_DEF(RefObject, void *, retain, (void *_self));
O_METHOD_DEF(RefObject, void *, release, (void *_self));
O_METHOD_DEF(RefObject, void *, auto_release, (void *_self));

#define RefObjectClass_Attr						\
  ObjectClass_Attr;							\
  O_METHOD (RefObject, retain);						\
  O_METHOD (RefObject, release);					\
  O_METHOD (RefObject, auto_release)

#define RefObject_Attr							\
  Object_Attr;								\
  int ref_count

O_CLASS(RefObject, Object);

#endif				/* not REFOBJECT_H */
