#ifndef Type_H
#define Type_H

#include "co2/CompileObject.h"

O_METHOD_DEF (Type, bool, is_compatible, (void *_self, void *_other));
O_METHOD_DEF (Type, void, assert_compatible, (void *_self, void *_other));
O_METHOD_DEF (Type, void, assume_compatible, (void *_self, void *_other));
O_METHOD_DEF (Type, bool, is_void_ptr, (void *_self));
O_METHOD_DEF (Type, struct Type *, get_declared_type, (void *_self));

#define TypeClass_Attr				\
  CompileObjectClass_Attr;			\
  O_METHOD(Type, is_compatible);		\
  O_METHOD(Type, is_void_ptr);			\
  O_METHOD(Type, assert_compatible);		\
  O_METHOD(Type, assume_compatible);		\
  O_METHOD(Type, get_declared_type)

#define Type_Attr				\
  CompileObject_Attr

O_CLASS (Type, CompileObject);

#endif /* Type_H */
