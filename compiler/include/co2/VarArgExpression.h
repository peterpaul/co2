#ifndef VarArgExpression_H
#define VarArgExpression_H

#include "co2/Expression.h"
#include "co2/Type.h"

#define VarArgExpressionClass_Attr			\
  ExpressionClass_Attr

#define VarArgExpression_Attr				\
  Expression_Attr;					\
  struct Expression * va_arg_expr;			\
  struct Type * va_arg_type

O_CLASS (VarArgExpression, Expression);

#endif /* VarArgExpression_H */
