#ifndef CLASSDECLARATION_H
#define CLASSDECLARATION_H

#include "Declaration.h"

O_METHOD_DEF(ClassDeclaration, bool, is_compatible, (void *_self, void *_other));

#define ClassDeclarationClass_Attr		\
  DeclarationClass_Attr;			\
  O_METHOD(ClassDeclaration, is_compatible)

#define ClassDeclaration_Attr			\
  Declaration_Attr;				\
  struct Token * superclass;			\
  struct RefList * interfaces;			\
  struct RefList * members;			\
  struct Scope * member_scope

O_CLASS(ClassDeclaration, Declaration);

#endif /* CLASSDECLARATION_H */
