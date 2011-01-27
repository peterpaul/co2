#ifndef SizeExpression_H
#define SizeExpression_H

#include "co2/Expression.h"
#include "co2/Type.h"

#define SizeExpressionClass_Attr			\
  ExpressionClass_Attr

#define SizeExpression_Attr				\
  Expression_Attr;					\
  struct Type * param

O_CLASS(SizeExpression, Expression);

#endif /* SizeExpression_H */
