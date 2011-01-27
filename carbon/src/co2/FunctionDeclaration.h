#ifndef FUNDECLARATION_H
#define FUNDECLARATION_H

#include "co2/Declaration.h"

#define FunctionDeclarationClass_Attr		\
  DeclarationClass_Attr

#define FunctionDeclaration_Attr		\
  Declaration_Attr;				\
  struct RefList * formal_arguments;		\
  struct InterfaceDeclaration * interface_decl;	\
  struct Statement * body

O_CLASS (FunctionDeclaration, Declaration);

#endif /* FUNDECLARATION_H */
