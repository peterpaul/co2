#ifndef NewExpression_H
#define NewExpression_H

#include "co2/TokenExpression.h"

O_METHOD_DEF (NewExpression, void, set_ctor_name,
	      (void *_self, struct TokenExpression * ctor_name));

#define NewExpressionClass_Attr			\
  ExpressionClass_Attr;				\
  O_METHOD(NewExpression, set_ctor_name)

#define NewExpression_Attr			\
  Expression_Attr;				\
  struct Type * new_type;			\
  struct TokenExpression * ctor_name;		\
  struct Expression * array_size;		\
  struct RefList * ctor_arguments

O_CLASS (NewExpression, Expression);

#endif /* NewExpression_H */
