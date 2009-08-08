#ifndef CLASSDECLARATION_H
#define CLASSDECLARATION_H

#include "Declaration.h"

#define ClassDeclarationClass_Attr		\
  DeclarationClass_Attr

#define ClassDeclaration_Attr			\
  Declaration_Attr;				\
  struct Token * superclass;			\
  struct RefList * interfaces;			\
  struct RefList * members

O_CLASS(ClassDeclaration, Declaration);

#endif /* CLASSDECLARATION_H */
