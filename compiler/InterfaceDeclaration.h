#ifndef InterfaceDeclaration_H
#define InterfaceDeclaration_H

#include "Declaration.h"

#define InterfaceDeclarationClass_Attr		\
  DeclarationClass_Attr

#define InterfaceDeclaration_Attr		\
  Declaration_Attr;				\
  struct Scope * member_scope;			\
  struct RefList * members;			\
  struct RefList * interfaces

O_CLASS(InterfaceDeclaration, Declaration);

#endif /* InterfaceDeclaration_H */
