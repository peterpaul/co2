#ifndef TokenExpression_H
#define TokenExpression_H

#include "co2/Expression.h"
#include "co2/Token.h"

O_METHOD_DEF (TokenExpression, void, lookup, (void *_self));

#define TokenExpressionClass_Attr			\
  ExpressionClass_Attr;					\
  O_METHOD(TokenExpression, lookup)

#define TokenExpression_Attr				\
  Expression_Attr;					\
  struct Token * token;					\
  struct Scope * scope;					\
  struct Declaration * decl;				\
  bool check_global_scope

O_CLASS (TokenExpression, Expression);

#endif /* TokenExpression_H */
