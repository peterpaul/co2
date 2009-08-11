#include "Scope.h"
#include "Token.h"

#define O_SUPER Hash()

struct Scope * current_scope = NULL;

O_IMPLEMENT(Scope, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct Scope * self = O_CAST(_self, Scope());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  self->parent = current_scope;
  current_scope = self;
  return self;
}

O_IMPLEMENT(Scope, void *, dtor, (void *_self), (_self))
{
  struct Scope *self = O_CAST(_self, Scope());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(Scope, struct Declaration *, declare, (void *_self, struct Declaration * decl), (_self, decl))
{
  struct Scope *self = O_CAST(_self, Scope());
  O_CALL(self, add, decl->name->name->data, decl);
  O_CALL(decl, set_scope, self);
  return decl;
}

O_IMPLEMENT(Scope, void, leave, (void *_self), (_self))
{
  struct Scope *self = O_CAST(_self, Scope());
  current_scope = self->parent;
}

O_IMPLEMENT(Scope, void, error_already_declared, (void *_self, const char * key, const void *_item), (_self, key, _item))
{
  struct Scope *self = O_CAST(_self, Scope());
  struct Declaration *decl = o_cast(_item, Declaration());
  error(decl->name, "'%s' already declared.\n", key);
}

O_IMPLEMENT(Scope, struct Declaration *, lookup_in_this_scope, (void *_self, struct Token * token), (_self, token))
{
  struct Scope *self = O_CAST(_self, Scope());
  return O_CALL(self, get, token->name->data);
}

O_IMPLEMENT(Scope, struct Declaration *, lookup, (void *_self, struct Token * token), (_self, token))
{
  struct Scope *self = O_CAST(_self, Scope());
  struct Declaration * result = O_CALL(self, lookup_in_this_scope, token);
  if (result == NULL && self->parent != NULL)
    {
      result = O_CALL(self->parent, lookup, token);
    }
  return result;
}

O_OBJECT(Scope, Hash);
O_OBJECT_METHOD(Scope, ctor);
O_OBJECT_METHOD(Scope, dtor);
O_OBJECT_METHOD(Scope, declare);
O_OBJECT_METHOD(Scope, leave);
O_OBJECT_METHOD(Scope, error_already_declared);
O_OBJECT_METHOD(Scope, lookup_in_this_scope);
O_OBJECT_METHOD(Scope, lookup);
O_END_OBJECT
