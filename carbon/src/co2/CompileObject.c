#include "co2/CompileObject.h"

#define O_SUPER RefObject()

O_IMPLEMENT (CompileObject, void *, ctor, (void *_self, va_list * app))
{
  struct CompileObject *self = O_CAST (_self, CompileObject ());
  self = O_SUPER->ctor (self, app);
  return self;
}

O_IMPLEMENT (CompileObject, void *, dtor, (void *_self))
{
  struct CompileObject *self = O_CAST (_self, CompileObject ());
  return O_SUPER->dtor (self);
}

/* filter methods */
int
type_filter (void *_member, va_list * app)
{
  return o_is_of (_member, va_arg (*app, void *));
}

int
not_type_filter (void *_member, va_list * app)
{
  return !type_filter (_member, app);;
}

void
accept (void *_object, va_list *app)
{
  struct CompileObject *object = O_CAST (_object, CompileObject ());
  struct BaseCompileObjectVisitor *visitor = O_GET_ARG (BaseCompileObjectVisitor);
  O_CALL (object, accept, visitor);
}

O_IMPLEMENT (CompileObject, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct CompileObject *self = O_CAST (_self, CompileObject ());
  O_CALL (visitor, visit, self);
}

O_OBJECT (CompileObject, RefObject);
O_OBJECT_METHOD (CompileObject, ctor);
O_OBJECT_METHOD (CompileObject, dtor);
O_OBJECT_METHOD (CompileObject, accept);
O_END_OBJECT
