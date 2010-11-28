#include "Context.h"
#include "CompileObject.h"

#define O_SUPER RefObject()

struct Context *current_context;

O_IMPLEMENT (Context, void *, ctor, (void *_self, va_list * app))
{
  struct Context *self = O_CAST (_self, Context ());
  self = O_SUPER->ctor (self, app);
  self->stack = O_CALL_CLASS (RefList (), new, 8, CompileObject ());
  O_CALL (self->stack, retain);
  return self;
}

O_IMPLEMENT (Context, void *, dtor, (void *_self))
{
  struct Context *self = O_CAST (_self, Context ());
  O_CALL (self->stack, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (Context, void, add, (void *_self, void *_item))
{
  struct Context *self = O_CAST (_self, Context ());
  O_CALL (self->stack, append, _item);
}

O_IMPLEMENT (Context, void *, find, (void *_self, void *_class))
{
  struct Context *self = O_CAST (_self, Context ());
  int i;
  for (i = self->stack->length - 1; i >= 0; i--)
    {
      void *item = O_CALL (self->stack, get, i);
      if (o_is_of (item, _class))
	{
	  return item;
	}
    }
  return NULL;
}

O_IMPLEMENT (Context, void, remove_last, (void *_self))
{
  struct Context *self = O_CAST (_self, Context ());
  O_CALL (self->stack, remove);
}

O_OBJECT (Context, RefObject);
O_OBJECT_METHOD (Context, ctor);
O_OBJECT_METHOD (Context, dtor);
O_OBJECT_METHOD (Context, add);
O_OBJECT_METHOD (Context, find);
O_OBJECT_METHOD (Context, remove_last);
O_END_OBJECT
