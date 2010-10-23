#ifndef SuperExpression_H
#define SuperExpression_H

#include "Expression.h"
#include "RefList.h"

#define SuperExpressionClass_Attr			\
  ExpressionClass_Attr
#define SuperExpression_Attr				\
  Expression_Attr;					\
  struct Token * super;					\
  struct Token * ctor_name;				\
  struct ClassDeclaration * class_context;		\
  struct Declaration * method_context;			\
  struct RefList * actual_arguments

O_CLASS(SuperExpression, Expression);

#endif /* SuperExpression_H */
