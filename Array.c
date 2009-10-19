#include "Array.h"

#define O_SUPER RefObject()

O_IMPLEMENT(Array, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct Array * self = O_CAST(_self, Array());
  self = O_SUPER->ctor(self, app);
  self->capacity = va_arg(*app, unsigned);
  self->type = o_cast(va_arg(*app, void *), Class());
  if (self->capacity > 0)
    {
      self->data = calloc(self->capacity, sizeof (void *));
    }
  else
    {
      self->data = NULL;
    }
  return self;
}

O_IMPLEMENT(Array, void *, dtor, (void *_self), (_self))
{
  struct Array *self = O_CAST(_self, Array());
  self->capacity = 0;
  if (self->data)
    {
      free(self->data);
      self->data = NULL;
    }
  return O_SUPER->dtor(self);
}

/**
 * "resize" can handle the following situations:
 * - from nothing to nothing
 * - from nothing to anything
 * - from anything to nothing
 * - make smaller
 * - make larger
 */
O_IMPLEMENT(Array, void *, resize, (void *_self, unsigned size), (_self, size))
{
  struct Array *self = O_CAST(_self, Array());
  if (size == 0)
    {
      if (self->data)
	{
	  free(self->data);
	  self->data = NULL;
	}
    }
  else
    {
      if (self->data)
	{
	  self->data = realloc(self->data, size);
	  if ((size - self->capacity) > 0)
	    {
	      memset(&(self->data[self->capacity]), 0, (size - self->capacity) * sizeof (void *));
	    }
	}
      else
	{
	  self->data = calloc(self->capacity, sizeof (void *));
	}
    }
  self->capacity = size;
  return self;
}

O_IMPLEMENT(Array, void *, get, (void *_self, unsigned index), (_self, index))
{
  struct Array *self = O_CAST(_self, Array());
  assertTrue(index < self->capacity, "array index out of bounds");
  if (index >= self->capacity)
    {
      return NULL;
    }
  return self->data[index];
}

O_IMPLEMENT(Array, void *, set, (void *_self, unsigned index, void *_item), (_self, index, _item))
{
  struct Array *self = O_CAST(_self, Array());
  assertTrue(index < self->capacity, "array index out of bounds");
  if (index >= self->capacity)
    {
      return NULL;
    }
  self->data[index] = o_cast(_item, self->type);
  return _item;
}

O_OBJECT(Array, RefObject);
O_OBJECT_METHOD(Array, ctor);
O_OBJECT_METHOD(Array, dtor);
O_OBJECT_METHOD(Array, resize);
O_OBJECT_METHOD(Array, set);
O_OBJECT_METHOD(Array, get);
O_END_OBJECT
