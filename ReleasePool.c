#include "ReleasePool.h"
#include "RefObject.h"
#include "ReleasePoolItem.h"

#define O_SUPER Object()

struct ReleasePool * current_release_pool;
#define RELEASEPOOL_HASH_SIZE 1223

O_IMPLEMENT(ReleasePool, void *, ctor, (void *_self, va_list * app), (_self, app))
{
  struct ReleasePool *self = O_CAST(_self, ReleasePool());
  self = O_SUPER->ctor(self, app);
  self->parent = current_release_pool;
  current_release_pool = self;
  self->hashmap = calloc(sizeof(struct ReleasePoolItem *), RELEASEPOOL_HASH_SIZE);
  return self;
}

O_IMPLEMENT(ReleasePool, void *, dtor, (void *_self), (_self))
{
  struct ReleasePool *self = O_CAST(_self, ReleasePool());
  int i;
  for (i=0; i<RELEASEPOOL_HASH_SIZE; i++)
    {
      O_CALL(self->hashmap[i], clear_list);
      self->hashmap[i] = NULL;
    }
  free(self->hashmap);
  current_release_pool = self->parent;

  self->hashmap = NULL;
  self->parent = NULL;
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(ReleasePool, void *, add, (void *_self, void *_item), (_self, _item))
{
  struct ReleasePool *self = O_CAST(_self, ReleasePool());
  size_t index = ((size_t)_item) % RELEASEPOOL_HASH_SIZE;
  struct ReleasePoolItem *item = O_CALL_CLASS(ReleasePoolItem(), new, _item);
  if (self->hashmap[index] == NULL)
    self->hashmap[index] = item;
  else
    self->hashmap[index] = O_CALL(self->hashmap[index], add, item);
  return _item;
}

O_IMPLEMENT(ReleasePool, void *, remove, (void *_self, void *_item), (_self, _item))
{
  struct ReleasePool *self = O_CAST(_self, ReleasePool());
  size_t index = ((size_t)_item) % RELEASEPOOL_HASH_SIZE;
  self->hashmap[index] = O_CALL(self->hashmap[index], remove, _item);
  return _item;
}

O_OBJECT(ReleasePool, Object);
O_OBJECT_METHOD(ReleasePool, ctor);
O_OBJECT_METHOD(ReleasePool, dtor);
O_OBJECT_METHOD(ReleasePool, add);
O_OBJECT_METHOD(ReleasePool, remove);
O_END_OBJECT
