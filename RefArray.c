#include "RefArray.h"

#define O_SUPER RefObject()

O_IMPLEMENT(RefArray, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct RefArray * self = O_CAST(_self, RefArray());
  self = O_SUPER->ctor(self, app);
  self->capacity = va_arg(*app, unsigned);
  self->type = o_branch_cast(va_arg(*app, void *), Class());
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

static void RefArray_release_range(struct RefArray * self, int start, int end)
{
  int i;
  for (i = start; i < end; i++)
    {
      O_CALL(self->data[i], release);
    }
}

O_IMPLEMENT(RefArray, void *, dtor, (void *_self), (_self))
{
  struct RefArray *self = O_CAST(_self, RefArray());
  self->capacity = 0;
  if (self->data)
    {
      RefArray_release_range(self, 0, self->capacity);
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
O_IMPLEMENT(RefArray, void *, resize, (void *_self, unsigned size), (_self, size))
{
  struct RefArray *self = O_CAST(_self, RefArray());
  if (size == 0)
    {
      if (self->data)
	{
	  RefArray_release_range(self, 0, self->capacity);
	  free(self->data);
	  self->data = NULL;
	}
    }
  else
    {
      if (self->data)
	{
	  if (size < self->capacity)
	    {
	      RefArray_release_range(self, size, self->capacity);
	    }
	  self->data = realloc(self->data, size);
	  if (size > self->capacity)
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

O_IMPLEMENT(RefArray, void *, get, (void *_self, unsigned index), (_self, index))
{
  struct RefArray *self = O_CAST(_self, RefArray());
  assertTrue(index < self->capacity, "array index out of bounds");
  if (index >= self->capacity)
    {
      return NULL;
    }
  return self->data[index];
}

O_IMPLEMENT(RefArray, void *, set, (void *_self, unsigned index, void *_item), (_self, index, _item))
{
  struct RefArray *self = O_CAST(_self, RefArray());
  struct RefObject *item = self->type ? o_branch_cast(_item, self->type) : _item;
  assertTrue(index < self->capacity, "array index out of bounds");
  if (index >= self->capacity)
    {
      return NULL;
    }
  struct RefObject *old_item = self->data[index];
  self->data[index] = O_CALL(item, retain);
  /* Release the old_item after retaining the new item, to prevent releasing the item when both are the same. */
  O_BRANCH_CALL(old_item, release);
  return _item;
}

O_OBJECT(RefArray, RefObject);
O_OBJECT_METHOD(RefArray, ctor);
O_OBJECT_METHOD(RefArray, dtor);
O_OBJECT_METHOD(RefArray, resize);
O_OBJECT_METHOD(RefArray, set);
O_OBJECT_METHOD(RefArray, get);
O_END_OBJECT
