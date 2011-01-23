#ifndef FunctionCallExpression_H
#define FunctionCallExpression_H

#include "co2/Expression.h"
#include "co2/FunctionType.h"

O_METHOD_DEF (FunctionCallExpression, void, type_check_arguments, (void *_self, struct FunctionType *function_type));

#define FunctionCallExpressionClass_Attr			\
  ExpressionClass_Attr;						\
  O_METHOD(FunctionCallExpression, type_check_arguments)

#define FunctionCallExpression_Attr				\
  Expression_Attr;						\
  struct Expression * function;					\
  struct RefList * actual_arguments

O_CLASS (FunctionCallExpression, Expression);

#endif /* FunctionCallExpression_H */
