#ifndef UnaryExpression_H
#define UnaryExpression_H

#include "co2/Expression.h"

#define UnaryExpressionClass_Attr			\
  ExpressionClass_Attr

#define UnaryExpression_Attr				\
  Expression_Attr;					\
  struct Token * operator;				\
  struct Expression * operand

O_CLASS (UnaryExpression, Expression);

#endif /* UnaryExpression_H */
