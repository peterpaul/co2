#ifndef CompileObject_H
#define CompileObject_H

#include "RefObject.h"

O_METHOD_DEF(CompileObject, void, type_check, (void *_self));
O_METHOD_DEF(CompileObject, void, optimize, (void *_self));
O_METHOD_DEF(CompileObject, void, generate, (void *_self));
O_METHOD_DEF(CompileObject, struct Token *, get_token, (void *_self));

#define CompileObjectClass_Attr			\
  RefObjectClass_Attr;				\
  O_METHOD(CompileObject, generate);		\
  O_METHOD(CompileObject, optimize);		\
  O_METHOD(CompileObject, type_check);		\
  O_METHOD(CompileObject, get_token)

#define CompileObject_Attr			\
  RefObject_Attr

O_CLASS(CompileObject, RefObject);

#endif /* CompileObject_H */
