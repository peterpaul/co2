#ifndef VARDECLARATION_H
#define VARDECLARATION_H

#include "Declaration.h"
#include "Token.h"

#define VarDeclarationClass_Attr			\
  DeclarationClass_Attr

#define VarDeclaration_Attr			\
  Declaration_Attr;				\
  struct Type * type;				\
  struct List * variables

O_CLASS(VarDeclaration, Declaration);

#endif /* VARDECLARATION_H */
