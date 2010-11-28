#ifndef CONSTRUCTORDECLARATION_H
#define CONSTRUCTORDECLARATION_H

#include "Declaration.h"

#define ConstructorDeclarationClass_Attr		\
  DeclarationClass_Attr

#define ConstructorDeclaration_Attr		\
  Declaration_Attr;				\
  struct Token * class_name;			\
  struct RefList * formal_arguments;		\
  struct Statement * body

O_CLASS (ConstructorDeclaration, Declaration);

#endif /* CONSTRUCTORDECLARATION_H */
