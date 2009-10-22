#include "RefObject.h"

#define O_SUPER Object()

O_IMPLEMENT(RefObject, void *, ctor, (void *_self, va_list * app))
{
  struct RefObject *self = O_CAST(_self, RefObject());
  self = O_SUPER->ctor(self, app);
  self->ref_count = 0;
  /* Add self to auto release pool */
  O_CALL(current_release_pool, add, self);
  return self;
}

O_IMPLEMENT(RefObject, void *, dtor, (void *_self))
{
  struct RefObject *self = O_CAST(_self, RefObject());
  if (self->ref_count <= 0)
    {
      /* Remove self from auto release pool */
      O_CALL(current_release_pool, remove, self);
    }
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(RefObject, void *, retain, (void *_self))
{
  struct RefObject *self = O_CAST(_self, RefObject());
  if (self->ref_count <= 0)
    {
      /* Remove self from auto release pool */
      O_CALL(current_release_pool, remove, self);
      self->ref_count = 0;
    }
  self->ref_count++;
  return self;
}

O_IMPLEMENT(RefObject, void *, release, (void *_self))
{
  struct RefObject *self = O_CAST(_self, RefObject());
  self->ref_count--;
  if (self->ref_count == 0)
    {
      O_CALL(self, delete);
      self = NULL;
    }
  return self;
}

O_IMPLEMENT(RefObject, void *, auto_release, (void *_self))
{
  struct RefObject *self = O_CAST(_self, RefObject());
  self->ref_count--;
  if (self->ref_count == 0)
    {
      /* TODO add to auto release pool */
      O_CALL(current_release_pool, add, self);
    }
  return self;
}

O_OBJECT(RefObject, Object);
O_OBJECT_METHOD(RefObject, ctor);
O_OBJECT_METHOD(RefObject, dtor);
O_OBJECT_METHOD(RefObject, retain);
O_OBJECT_METHOD(RefObject, release);
O_OBJECT_METHOD(RefObject, auto_release);
O_END_OBJECT
