#ifndef CompileObject_H
#define CompileObject_H

#include "RefObject.h"

O_METHOD_DEF(CompileObject, void, generate, (void *_self));

#define CompileObjectClass_Attr			\
  RefObjectClass_Attr;				\
  O_METHOD(CompileObject, generate)

#define CompileObject_Attr			\
  RefObject_Attr

O_CLASS(CompileObject, RefObject);

#endif /* CompileObject_H */
