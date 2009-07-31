#include "Token.h"

#define O_SUPER RefObject()

O_IMPLEMENT(Token, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct Token *self = O_CAST(_self, Token());
  self = O_SUPER->ctor(self, app);
  self->name = va_arg(*app, struct String *);
  self->type = va_arg(*app, int);
  self->file = va_arg(*app, struct String *);
  self->line = va_arg(*app, int);
  return self;
}



O_OBJECT(Token, Object);
O_OBJECT_METHOD(Token, ctor);
O_END_OBJECT
