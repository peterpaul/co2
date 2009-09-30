#ifndef Expression_H
#define Expression_H

#include "CompileObject.h"

O_METHOD_DEF(Expression, void, set_scope, (void *_self, void *_scope));
O_METHOD_DEF(Expression, void, generate_left, (void *_self, bool left));

#define ExpressionClass_Attr			\
  CompileObjectClass_Attr;			\
  O_METHOD(Expression, set_scope);		\
  O_METHOD(Expression, generate_left)

#define Expression_Attr				\
  CompileObject_Attr;				\
  struct Type * type;				\
  bool is_method

O_CLASS(Expression, CompileObject);

#endif /* Expression_H */
