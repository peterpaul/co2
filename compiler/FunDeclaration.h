#ifndef FUNDECLARATION_H
#define FUNDECLARATION_H

#include "Declaration.h"

#define FunDeclarationClass_Attr		\
  DeclarationClass_Attr

#define FunDeclaration_Attr			\
  Declaration_Attr;				\
  struct Type * return_type;			\
  struct FunctionType * type;			\
  struct RefList * formal_arguments;		\
  struct Statement * body

O_CLASS(FunDeclaration, Declaration);

#endif /* FUNDECLARATION_H */
