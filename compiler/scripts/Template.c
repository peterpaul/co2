#include "$1.h"

#define O_SUPER $2()

O_IMPLEMENT($1, void *, ctor, (void *_self, va_list *app))
{
  struct $1 * self = O_CAST(_self, $1());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT($1, void *, dtor, (void *_self))
{
  struct $1 *self = O_CAST(_self, $1());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_OBJECT($1, $2);
O_OBJECT_METHOD($1, ctor);
O_OBJECT_METHOD($1, dtor);
O_END_OBJECT
