#ifndef ArrayType_H
#define ArrayType_H

#include "co2/Type.h"

#define ArrayTypeClass_Attr			\
  TypeClass_Attr

#define ArrayType_Attr				\
  Type_Attr;					\
  struct Type * base_type

O_CLASS (ArrayType, Type);

#endif /* ArrayType_H */