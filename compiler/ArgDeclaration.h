#ifndef ArgDeclaration_H
#define ArgDeclaration_H

#include "Declaration.h"

#define ArgDeclarationClass_Attr			\
  DeclarationClass_Attr

#define ArgDeclaration_Attr				\
  Declaration_Attr;					\
  struct Type * type

O_CLASS(ArgDeclaration, Declaration);

#endif /* ArgDeclaration_H */
