#ifndef REFOBJECT_H
#define REFOBJECT_H

#include "co2/Object.h"
#include "co2/ReleasePool.h"

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

#define O_RETAIN_ARG(type)				\
  ({ struct type * __tmp__ = O_GET_ARG(type);		\
    O_CALL(__tmp__, retain);})

#define O_BRANCH_RETAIN_ARG(type)			\
  ({ struct type * __tmp__ = O_BRANCH_GET_ARG(type);	\
    O_BRANCH_CALL(__tmp__, retain);})

#endif				/* not REFOBJECT_H */
