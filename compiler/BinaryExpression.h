#ifndef BinaryExpression_H
#define BinaryExpression_H

#include "Expression.h"

#define BinaryExpressionClass_Attr		\
  ExpressionClass_Attr

#define BinaryExpression_Attr			\
  Expression_Attr;				\
  struct Expression * operand[2];		\
  struct Token * operator

O_CLASS (BinaryExpression, Expression);

#endif /* BinaryExpression_H */
