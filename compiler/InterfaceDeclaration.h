#ifndef InterfaceDeclaration_H
#define InterfaceDeclaration_H

#include "ObjectTypeDeclaration.h"

#define InterfaceDeclarationClass_Attr		\
  ObjectTypeDeclarationClass_Attr

#define InterfaceDeclaration_Attr		\
  ObjectTypeDeclaration_Attr;			\
  struct RefList * interfaces

O_CLASS(InterfaceDeclaration, ObjectTypeDeclaration);

#endif /* InterfaceDeclaration_H */
