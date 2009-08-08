#ifndef Expression_H
#define Expression_H

#include "CompileObject.h"

#define ExpressionClass_Attr			\
  CompileObjectClass_Attr

#define Expression_Attr				\
  CompileObject_Attr;				\
  struct Expression * operand[2];		\
  struct Token * operator

O_CLASS(Expression, CompileObject);

#endif /* Expression_H */
