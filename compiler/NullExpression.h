#ifndef NullExpression_H
#define NullExpression_H

#include "TokenExpression.h"

#define NullExpressionClass_Attr			\
  TokenExpressionClass_Attr

#define NullExpression_Attr				\
  TokenExpression_Attr

O_CLASS(NullExpression, TokenExpression);

#endif /* NullExpression_H */
