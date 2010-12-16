#ifndef SizeExpression_H
#define SizeExpression_H

#include "Expression.h"
#include "Type.h"

#define SizeExpressionClass_Attr			\
  ExpressionClass_Attr

#define SizeExpression_Attr				\
  Expression_Attr;					\
  struct Type * param

O_CLASS(SizeExpression, Expression);

#endif /* SizeExpression_H */
