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
  struct Type * type

O_CLASS(Expression, CompileObject);

void Expression_generate_actual_argument(void *_arg, va_list *app);

#endif /* Expression_H */
