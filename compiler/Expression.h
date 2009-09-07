#ifndef Expression_H
#define Expression_H

#include "CompileObject.h"

#define ExpressionClass_Attr			\
  CompileObjectClass_Attr

#define Expression_Attr				\
  CompileObject_Attr;				\
  bool is_method

O_CLASS(Expression, CompileObject);

#endif /* Expression_H */
