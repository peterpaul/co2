#include "Array.h"

#define O_SUPER Object()

O_IMPLEMENT(Array, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct Array * self = O_CAST(_self, Array());
  self = O_SUPER->ctor(self, app);
  self->size = va_arg(*app, int);
  self->data = calloc(self->size, sizeof (void *));
  return self;
}

O_IMPLEMENT(Array, void *, dtor, (void *_self), (_self))
{
  struct Array *self = O_CAST(_self, Array());
  self->size = 0;
  free(self->data);
  self->data = NULL;
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(Array, void *, resize, (void *_self, int size), (_self, size))
{
  struct Array *self = O_CAST(_self, Array());
  // TODO check function description, and function result
  self->data = realloc(self->data, size);
  // TODO check function description
  memset(&(self->data[self->size]), 0, (size - self->size) * sizeof (void *));
  self->size = size;
  return self;
}

O_IMPLEMENT(Array, void *, get, (void *_self, int index), (_self, index))
{
  struct Array *self = O_CAST(_self, Array());
  if (index >= self->size)
    {
      return NULL;
    }
  return self->data[index];
}

O_IMPLEMENT(Array, void *, set, (void *_self, int index, void *_item), (_self, index, _item))
{
  struct Array *self = O_CAST(_self, Array());
  if (index >= self->size)
    {
      O_CALL(self, resize, index + 1);
    }
  self->data[index] = _item;
  return _item;
}

O_OBJECT(Array, Object);
O_OBJECT_METHOD(Array, ctor);
O_OBJECT_METHOD(Array, dtor);
O_OBJECT_METHOD(Array, resize);
O_OBJECT_METHOD(Array, set);
O_OBJECT_METHOD(Array, get);
O_END_OBJECT
