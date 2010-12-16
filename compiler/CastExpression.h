#ifndef CastExpression_H
#define CastExpression_H

#include "Expression.h"

#define CastExpressionClass_Attr			\
  ExpressionClass_Attr

#define CastExpression_Attr				\
  Expression_Attr;					\
  struct Type * cast_type;				\
  struct Expression * expression

O_CLASS(CastExpression, Expression);

#endif /* CastExpression_H */
