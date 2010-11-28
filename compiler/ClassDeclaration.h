#ifndef CLASSDECLARATION_H
#define CLASSDECLARATION_H

#include "ObjectTypeDeclaration.h"

#define ClassDeclarationClass_Attr		\
  ObjectTypeDeclarationClass_Attr

#define ClassDeclaration_Attr			\
  ObjectTypeDeclaration_Attr;			\
  struct Token * superclass;			\
  struct RefList * interfaces

O_CLASS (ClassDeclaration, ObjectTypeDeclaration);

#endif /* CLASSDECLARATION_H */
