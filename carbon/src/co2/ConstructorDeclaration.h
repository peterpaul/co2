#ifndef CONSTRUCTORDECLARATION_H
#define CONSTRUCTORDECLARATION_H

#include "co2/Declaration.h"

O_METHOD_DEF (ConstructorDeclaration, bool, has_var_args, (void *_self));

#define ConstructorDeclarationClass_Attr		\
  DeclarationClass_Attr;				\
  O_METHOD (ConstructorDeclaration, has_var_args)

#define ConstructorDeclaration_Attr		\
  Declaration_Attr;				\
  struct Token * class_name;			\
  struct RefList * formal_arguments;		\
  struct Statement * body

O_CLASS (ConstructorDeclaration, Declaration);

#endif /* CONSTRUCTORDECLARATION_H */
