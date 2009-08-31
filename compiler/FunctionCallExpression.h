#ifndef FunctionCallExpression_H
#define FunctionCallExpression_H

#include "Expression.h"

#define FunctionCallExpressionClass_Attr			\
  ExpressionClass_Attr

#define FunctionCallExpression_Attr				\
  Expression_Attr;						\
  struct Expression * function;					\
  struct RefList * actual_arguments

O_CLASS(FunctionCallExpression, Expression);

#endif /* FunctionCallExpression_H */
