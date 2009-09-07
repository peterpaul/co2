#include "File.h"
#include "Path.h"
#include "RefList.h"

#define O_SUPER CompileObject()

O_IMPLEMENT(File, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct File * self = O_CAST(_self, File());
  self = O_SUPER->ctor(self, app);
  self->package = O_CAST(va_arg(*app, struct Path *), Path());
  O_CALL(self->package, retain);
  self->imports = O_CAST(va_arg(*app, struct RefList *), RefList());
  O_CALL(self->imports, retain);
  self->declarations = O_CAST(va_arg(*app, struct RefList *), RefList());
  O_CALL(self->declarations, retain);
  return self;
}

O_IMPLEMENT(File, void *, dtor, (void *_self), (_self))
{
  struct File *self = O_CAST(_self, File());
  O_CALL(self->package, release);
  O_CALL(self->imports, release);
  O_CALL(self->declarations, release);
  return O_SUPER->dtor(self);
}

O_OBJECT(File, CompileObject);
O_OBJECT_METHOD(File, ctor);
O_OBJECT_METHOD(File, dtor);
O_END_OBJECT
