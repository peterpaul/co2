#ifndef ConditionalExpression_H
#define ConditionalExpression_H

#include "co2/Expression.h"

#define ConditionalExpressionClass_Attr		\
  ExpressionClass_Attr

#define ConditionalExpression_Attr		\
  Expression_Attr;				\
  struct Expression * condition;		\
  struct Expression * then_expr;		\
  struct Expression * else_expr

O_CLASS(ConditionalExpression, Expression);

#endif /* ConditionalExpression_H */
