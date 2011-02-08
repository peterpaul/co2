#include "co2/ReleasePoolItem.h"
#include "co2/RefObject.h"

#define O_SUPER Object()

O_IMPLEMENT(ReleasePoolItem, void *, ctor, (void *_self, va_list * app))
{
  struct ReleasePoolItem *self = O_CAST(_self, ReleasePoolItem());
  self = O_SUPER->ctor(self, app);
  self->item = o_cast(va_arg(*app, struct RefObject *), RefObject());
  self->next = NULL;
  return self;
}

O_IMPLEMENT(ReleasePoolItem, void *, add, (void *_self, void *_item))
{
  struct ReleasePoolItem *self = O_CAST(_self, ReleasePoolItem());
  struct ReleasePoolItem *item = o_cast(_item, ReleasePoolItem());
  item->next = self;
  return item;
}

O_IMPLEMENT(ReleasePoolItem, void *, remove, (void *_self, void *_item))
{
  struct ReleasePoolItem *self = O_CAST(_self, ReleasePoolItem());
  struct RefObject *item = O_CAST(_item, RefObject());
  if (self->item == item)
    {
      struct ReleasePoolItem *result = self->next;
      O_CALL(self, delete);;
      return result;
    }
  else
    {
      // TODO fix problems in compiler
      //      assertTrue(self->next != NULL, "ReleasePoolItem not found in list.");
      if (self->next != NULL)
	self->next = O_CALL(self->next, remove, item);
      return self;
    }
}

O_IMPLEMENT(ReleasePoolItem, void, clear_list, (void *_self))
{
  struct ReleasePoolItem *self = O_CAST(_self, ReleasePoolItem());
  O_BRANCH_CALL(self->next, clear_list);
  O_CALL(self->item, delete);
  O_CALL(self, delete);;
}

O_OBJECT(ReleasePoolItem, Object);
O_OBJECT_METHOD(ReleasePoolItem, ctor);
O_OBJECT_METHOD(ReleasePoolItem, add);
O_OBJECT_METHOD(ReleasePoolItem, remove);
O_OBJECT_METHOD(ReleasePoolItem, clear_list);
O_END_OBJECT