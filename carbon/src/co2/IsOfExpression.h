#ifndef IsOfExpression_H
#define IsOfExpression_H

#include "co2/Expression.h"

#define IsOfExpressionClass_Attr		\
  ExpressionClass_Attr

#define IsOfExpression_Attr			\
  Expression_Attr;				\
  struct Expression * expr_to_check;		\
  struct Token * class_token

O_CLASS(IsOfExpression, Expression);

#endif /* IsOfExpression_H */
