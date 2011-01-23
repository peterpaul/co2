#ifndef Scope_H
#define Scope_H

#include "co2/Hash.h"
#include "co2/Declaration.h"
#include "co2/Token.h"

typedef enum
{
  GLOBAL_SCOPE,
  VARIABLE_SCOPE,
  CLASS_SCOPE,
  ARGUMENT_SCOPE,
  COMPOUND_SCOPE,
  INTERFACE_SCOPE,
  STRUCT_SCOPE,
  CATCH_SCOPE
} ScopeType;

O_METHOD_DEF (Scope, struct Declaration *, declare,
	      (void *_self, struct Declaration * decl));
O_METHOD_DEF (Scope, void, leave, (void *_self));
O_METHOD_DEF (Scope, struct Declaration *, lookup_in_this_scope,
	      (void *_self, struct Token * token));
O_METHOD_DEF (Scope, struct Declaration *, lookup,
	      (void *_self, struct Token * token));
O_METHOD_DEF (Scope, struct Declaration *, lookup_type_in_this_scope,
	      (void *_self, struct Token * token, void *_type));
O_METHOD_DEF (Scope, struct Declaration *, lookup_type,
	      (void *_self, struct Token * token, void *_type));
O_METHOD_DEF (Scope, bool, exists_in_this_scope,
	      (void *_self, struct Token * token));
O_METHOD_DEF (Scope, bool, exists, (void *_self, struct Token * token));
O_METHOD_DEF (Scope, void, error_not_found,
	      (void *_self, struct Token * token));

#define ScopeClass_Attr				\
  HashClass_Attr;				\
  O_METHOD(Scope, declare);			\
  O_METHOD(Scope, leave);			\
  O_METHOD(Scope, lookup_in_this_scope);	\
  O_METHOD(Scope, lookup);			\
  O_METHOD(Scope, lookup_type_in_this_scope);	\
  O_METHOD(Scope, lookup_type);			\
  O_METHOD(Scope, exists_in_this_scope);	\
  O_METHOD(Scope, exists);			\
  O_METHOD(Scope, error_not_found)

#define Scope_Attr				\
  Hash_Attr;					\
  ScopeType type;				\
  struct Token * name;				\
  struct Scope * parent

O_CLASS (Scope, Hash);

extern struct Scope *current_scope;
extern struct Scope *global_scope;

#endif /* Scope_H */
