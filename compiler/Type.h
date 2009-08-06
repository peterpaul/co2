#ifndef TYPE_H
#define TYPE_H

#include "CompileObject.h"
#include "Token.h"

#define TypeClass_Attr				\
  CompileObjectClass_Attr

#define Type_Attr				\
  CompileObject_Attr

O_CLASS(Type, CompileObject);

#endif /* TYPE_H */
