#include "Exception.h"

#define O_SUPER Object()

void
throw_ex (struct Exception *_ex)
{
  throw (_ex->class->id, _ex);
}

O_IMPLEMENT (Exception, void *, ctor, (void *_self, va_list * app))
{
  struct Exception *self = O_CAST (_self, Exception ());
  self = O_SUPER->ctor (self, app);
  if (self->class->id == 0)
    {
      self->class->id = hash_function (self->class->name);
    }
  self->file = strdup (va_arg (*app, const char *));
  self->line = va_arg (*app, unsigned int);
  self->func = strdup (va_arg (*app, const char *));
  self->message = va_arg (*app, const char *);
  if (self->message)
    {
      self->message = strdup (self->message);
    }
  return self;
}

O_IMPLEMENT (Exception, void *, dtor, (void *_self))
{
  struct Exception *self = O_CAST (_self, Exception ());
  free ((char *) self->file);
  free ((char *) self->func);
  if (self->message)
    {
      free ((char *) self->message);
    }
  return O_SUPER->dtor (self);
}

O_OBJECT (Exception, Object);
O_OBJECT_METHOD (Exception, ctor);
O_OBJECT_METHOD (Exception, dtor);
O_END_OBJECT
