#include "Path.h"
#include "RefList.h"

#define O_SUPER CompileObject()

O_IMPLEMENT(Path, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct Path * self = O_CAST(_self, Path());
  self = O_SUPER->ctor(self, app);
  self->path_name = O_CAST(va_arg(*app, struct RefList *), RefList());
  O_CALL(self->path_name, retain);
  return self;
}

O_IMPLEMENT(Path, void *, dtor, (void *_self), (_self))
{
  struct Path *self = O_CAST(_self, Path());
  O_CALL(self->path_name, release);
  return O_SUPER->dtor(self);
}

O_OBJECT(Path, CompileObject);
O_OBJECT_METHOD(Path, ctor);
O_OBJECT_METHOD(Path, dtor);
O_END_OBJECT
