#ifndef CONSTRUCTORDECLARATION_H
#define CONSTRUCTORDECLARATION_H

#include "Declaration.h"

O_METHOD_DEF(ConstructorDeclaration, void, set_class_name, (void *_self, struct Token * name));

#define ConstructorDeclarationClass_Attr		\
  DeclarationClass_Attr;				\
  O_METHOD(ConstructorDeclaration, set_class_name)

#define ConstructorDeclaration_Attr		\
  Declaration_Attr;				\
  struct Token * class_name;			\
  struct RefList * formal_arguments;		\
  struct Statement * body

O_CLASS(ConstructorDeclaration, Declaration);

#endif /* CONSTRUCTORDECLARATION_H */
