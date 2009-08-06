#ifndef VARDECLARATION_H
#define VARDECLARATION_H

#include "Declaration.h"
#include "Type.h"

O_METHOD_DEF(VarDeclaration, void, set_type, (void *_self, struct Type * type));

#define VarDeclarationClass_Attr		\
  DeclarationClass_Attr;			\
  O_METHOD(VarDeclaration, set_type)

#define VarDeclaration_Attr			\
  Declaration_Attr;				\
  struct Type * type;				\
  struct Token * token;				\
  struct Expression * expr

O_CLASS(VarDeclaration, Declaration);

#endif /* VARDECLARATION_H */
