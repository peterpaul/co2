#ifndef Scope_H
#define Scope_H

#include "Hash.h"
#include "Declaration.h"
#include "Token.h"

O_METHOD_DEF(Scope, struct Declaration *, declare, (void *_self, struct Declaration * decl));
O_METHOD_DEF(Scope, void, leave, (void *_self));
O_METHOD_DEF(Scope, struct Declaration *, lookup_in_this_scope, (void *_self, struct Token * token));
O_METHOD_DEF(Scope, struct Declaration *, lookup, (void *_self, struct Token * token));
O_METHOD_DEF(Scope, void, error_not_found, (void *_self, struct Token * token));

#define ScopeClass_Attr				\
  HashClass_Attr;				\
  O_METHOD(Scope, declare);			\
  O_METHOD(Scope, leave);			\
  O_METHOD(Scope, lookup_in_this_scope);	\
  O_METHOD(Scope, lookup);			\
  O_METHOD(Scope, error_not_found)

#define Scope_Attr				\
  Hash_Attr;					\
  struct Scope * parent

O_CLASS(Scope, Hash);

extern struct Scope * current_scope;

#endif /* Scope_H */
