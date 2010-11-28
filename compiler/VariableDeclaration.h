#ifndef VARDECLARATION_H
#define VARDECLARATION_H

#include "Declaration.h"
#include "Type.h"

O_METHOD_DEF (VariableDeclaration, void, set_type,
	      (void *_self, struct Type * type));

#define VariableDeclarationClass_Attr		\
  DeclarationClass_Attr;			\
  O_METHOD(VariableDeclaration, set_type)

#define VariableDeclaration_Attr			\
  Declaration_Attr;				\
  struct Expression * expr

O_CLASS (VariableDeclaration, Declaration);

#endif /* VARDECLARATION_H */
