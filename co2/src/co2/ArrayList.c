#include "co2/ArrayList.h"

#define O_SUPER Array()

O_IMPLEMENT(ArrayList, void *, ctor, (void *_self, va_list *app))
{
  struct ArrayList * self = O_CAST(_self, ArrayList());
  self = O_SUPER->ctor(self, app);
  self->length = 0;
  return self;
}

O_IMPLEMENT(ArrayList, void *, dtor, (void *_self))
{
  struct ArrayList *self = O_CAST(_self, ArrayList());
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(ArrayList, void *, prepend, (void *_self, void *item))
{
  struct ArrayList *self = O_CAST(_self, ArrayList());
  if (self->length == self->capacity)
    {
      O_CALL(self, resize, self->capacity * 2);
    }
  memmove(&self->data[1], &self->data[0], sizeof(void *[self->length]));
  O_CALL(self, set, 0, item);
  self->length ++;
}

O_IMPLEMENT(ArrayList, void *, append, (void *_self, void *item))
{
  struct ArrayList *self = O_CAST(_self, ArrayList());
  if (self->length == self->capacity)
    {
      O_CALL(self, resize, self->capacity * 2);
    }
  O_CALL(self, set, self->length, item);
  self->length ++;
}

static void ArrayList_append_item(void *_item, va_list * ap)
{
  struct ArrayList *list = o_cast(va_arg(*ap, struct ArrayList *), ArrayList());
  O_CALL(list, append, _item);
}

O_IMPLEMENT(ArrayList, void *, append_list, (void *_self, void *_list))
{
  struct ArrayList *self = O_CAST(_self, ArrayList());
  struct ArrayList *list = o_cast(_list, ArrayList());
  O_CALL(list, map_args, ArrayList_append_item, self);
  return self;
}

O_IMPLEMENT(ArrayList, void *, merge, (void *_self, void *_other))
{
  struct ArrayList *self = O_CAST(_self, ArrayList());
  struct ArrayList *other = O_CAST(_other, ArrayList());
  O_CALL(self, append_list, other);
  /* delete other list */
  other->length = 0;
  O_CALL(other, delete);
  /* return result */
  return self;
}

O_IMPLEMENT(ArrayList, void *, map, (void *_self, void (*fun) (void *)))
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
	    (void *_self, void (*fun) (void *, va_list *), ...))
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

O_IMPLEMENT(ArrayList, void *, filter, (void *_self, int (*filter) (void *)))
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
	    (void *_self, int (*filter) (void *, va_list *), ...))
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

O_IMPLEMENT(ArrayList, void *, getIterator, (void *_self));

O_OBJECT(ArrayList, Array);
O_OBJECT_METHOD(ArrayList, ctor);
O_OBJECT_METHOD(ArrayList, dtor);
O_END_OBJECT
