#include "ArrayList.h"

#define O_SUPER Array()

O_IMPLEMENT(ArrayList, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct ArrayList * self = O_CAST(_self, ArrayList());
  self = O_SUPER->ctor(self, app);
  self->length = 0;
  return self;
}

O_IMPLEMENT(ArrayList, void *, dtor, (void *_self), (_self))
{
  struct ArrayList *self = O_CAST(_self, ArrayList());
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(ArrayList, void *, append, (void *_self, void *item), (_self, item))
{
  struct ArrayList *self = O_CAST(_self, ArrayList());
  if (self->length == self->capacity)
    {
      O_CALL(self, resize, self->capacity * 2);
    }
  O_CALL(self, set, self->length, item);
  self->length ++;
}

O_IMPLEMENT(ArrayList, void *, append_list, (void *_self, void *_list), (_self, _list));
O_IMPLEMENT(ArrayList, void *, merge, (void *_self, void *_other), (_self, _other));
O_IMPLEMENT(ArrayList, void *, map, (void *_self, void (*fun) (void *)), (_self, fun))
{
  struct ArrayList *self = O_CAST(_self, ArrayList());
  int i;
  const unsigned length = self->length;
  for (i = 0; i < length; i++)
    {
      fun(self->data[i]);
    }
  return self;
}

O_IMPLEMENT(ArrayList, void *, map_args,
	    (void *_self, void (*fun) (void *, va_list *), ...), (_self, fun))
{
  struct ArrayList *self = O_CAST(_self, ArrayList());
  int i;
  va_list ap;
  va_start(ap, fun);
  const unsigned length = self->length;
  for (i = 0; i < length; i++)
    {
      va_list aq;
      va_copy(aq, ap);
      fun(self->data[i], &aq);
      va_end(aq);
    }
  va_end(ap);
  return self;
}

O_IMPLEMENT(ArrayList, void *, filter, (void *_self, int (*filter) (void *)), (_self, filter))
{
  struct ArrayList *self = O_CAST(_self, ArrayList());
  int i;
  const unsigned length = self->length;
  struct ArrayList *result = O_CALL_CLASS(ArrayList(), new, self->length / 2, self->type);
  for (i = 0; i < length; i++)
    {
      if (filter(self->data[i]))
	O_CALL(result, append, self->data[i]);
    }
  return result;
}

O_IMPLEMENT(ArrayList, void *, filter_args,
	    (void *_self, int (*filter) (void *, va_list *), ...), (_self, filter))
{
  struct ArrayList *self = O_CAST(_self, ArrayList());
  int i;
  va_list ap;
  va_start(ap, filter);
  const unsigned length = self->length;
  struct ArrayList *result = O_CALL_CLASS(ArrayList(), new, self->length / 2, self->type);
  for (i = 0; i < length; i++)
    {
      va_list aq;
      va_copy(aq, ap);
      if (filter(self->data[i], &aq))
	O_CALL(result, append, self->data[i]);
      va_end(aq);
    }
  va_end(ap);
  return result;
}

O_IMPLEMENT(ArrayList, void *, getIterator, (void *_self), (_self));

O_OBJECT(ArrayList, Array);
O_OBJECT_METHOD(ArrayList, ctor);
O_OBJECT_METHOD(ArrayList, dtor);
O_END_OBJECT
