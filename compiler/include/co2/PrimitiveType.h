#ifndef PrimitiveType_H
#define PrimitiveType_H

#include "co2/Type.h"

#define PrimitiveTypeClass_Attr			\
  TypeClass_Attr

#define PrimitiveType_Attr		\
  Type_Attr;				\
  struct Token * token

O_CLASS (PrimitiveType, Type);

#endif /* PrimitiveType_H */
