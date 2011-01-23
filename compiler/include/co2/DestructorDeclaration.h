#ifndef DESTRUCTORDECLARATION_H
#define DESTRUCTORDECLARATION_H

#include "co2/Declaration.h"

#define DestructorDeclarationClass_Attr		\
  DeclarationClass_Attr

#define DestructorDeclaration_Attr		\
  Declaration_Attr;				\
  struct Token * class_name;			\
  struct Statement * body

O_CLASS (DestructorDeclaration, Declaration);

#endif /* DESTRUCTORDECLARATION_H */
