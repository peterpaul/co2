#include "Declaration.h"

#define O_SUPER RefObject()

O_IMPLEMENT(Declaration, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct Declaration * self = O_CAST(_self, Declaration());
  self = O_SUPER->ctor(self, app);
  self->token = va_arg(*app, struct Token *);
  return self;
}

O_OBJECT(Declaration, RefObject);
O_OBJECT_METHOD(Declaration, ctor);
O_END_OBJECT
