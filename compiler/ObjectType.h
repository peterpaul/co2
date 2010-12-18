#ifndef ObjectType_H
#define ObjectType_H

#include "Type.h"

#define ObjectTypeClass_Attr			\
  TypeClass_Attr

#define ObjectType_Attr				\
  Type_Attr;					\
  struct ObjectTypeDeclaration * decl;		\
  struct Token * token

O_CLASS (ObjectType, Type);

#endif /* ObjectType_H */
