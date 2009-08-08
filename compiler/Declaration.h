#ifndef Declaration_H
#define Declaration_H

#include "CompileObject.h"
#include "Scope.h"

O_METHOD_DEF(Declaration, void, set_scope, (void *_self, void * scope));

#define DeclarationClass_Attr			\
  CompileObjectClass_Attr;			\
  O_METHOD(Declaration, set_scope)


#define Declaration_Attr			\
  CompileObject_Attr;				\
  struct Token * name;				\
  struct Scope * scope

O_CLASS(Declaration, CompileObject);

#endif /* Declaration_H */
