#ifndef NestedExpression_H
#define NestedExpression_H

#include "co2/Expression.h"

#define NestedExpressionClass_Attr			\
  ExpressionClass_Attr

#define NestedExpression_Attr				\
  Expression_Attr;					\
  struct Expression * expr

O_CLASS (NestedExpression, Expression);

#endif /* NestedExpression_H */
