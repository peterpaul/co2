#ifndef Expression_H
#define Expression_H

#include "CompileObject.h"

O_METHOD_DEF(Expression, void, set_scope, (void *_self, void *_scope));

#define ExpressionClass_Attr			\
  CompileObjectClass_Attr;			\
  O_METHOD(Expression, set_scope)

#define Expression_Attr				\
  CompileObject_Attr;				\
  struct Type * type;				\
  bool is_method

O_CLASS(Expression, CompileObject);

#endif /* Expression_H */
