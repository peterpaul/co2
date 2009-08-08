#ifndef Scope_H
#define Scope_H

#include "Hash.h"
#include "Declaration.h"

O_METHOD_DEF(Scope, struct Declaration *, declare, (void *_self, struct Declaration * decl));
O_METHOD_DEF(Scope, void, leave, (void *_self));

#define ScopeClass_Attr				\
  HashClass_Attr;				\
  O_METHOD(Scope, declare);			\
  O_METHOD(Scope, leave)

#define Scope_Attr				\
  Hash_Attr;					\
  struct Scope * parent

O_CLASS(Scope, Hash);

extern struct Scope * current_scope;

#endif /* Scope_H */
