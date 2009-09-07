#ifndef NewExpression_H
#define NewExpression_H

#include "Expression.h"

#define NewExpressionClass_Attr			\
  ExpressionClass_Attr

#define NewExpression_Attr			\
  Expression_Attr;				\
  struct Type * type;				\
  struct Expression * array_size;		\
  struct RefList * ctor_arguments

O_CLASS(NewExpression, Expression);

#endif /* NewExpression_H */
