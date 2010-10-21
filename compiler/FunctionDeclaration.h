#ifndef FUNDECLARATION_H
#define FUNDECLARATION_H

#include "Declaration.h"

#define FunctionDeclarationClass_Attr		\
  DeclarationClass_Attr

#define FunctionDeclaration_Attr			\
  Declaration_Attr;				\
  struct RefList * formal_arguments;		\
  struct Statement * body

O_CLASS(FunctionDeclaration, Declaration);

#endif /* FUNDECLARATION_H */
