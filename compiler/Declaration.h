#ifndef Declaration_H
#define Declaration_H

#include "CompileObject.h"
#include "Scope.h"

O_METHOD_DEF(Declaration, void, set_scope, (void *_self, void * scope));
O_METHOD_DEF(Declaration, void, set_class_decl, (void *_self, void *_class_decl));

#define DeclarationClass_Attr			\
  CompileObjectClass_Attr;			\
  O_METHOD(Declaration, set_scope);		\
  O_METHOD(Declaration, set_class_decl)

#define Declaration_Attr			\
  CompileObject_Attr;				\
  struct Token * name;				\
  struct Scope * scope;				\
  struct Type * type;				\
  struct ClassDeclaration * class_decl

O_CLASS(Declaration, CompileObject);

#endif /* Declaration_H */
