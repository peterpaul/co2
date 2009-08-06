#include "Token.h"
#include "String.h"

#define O_SUPER RefObject()

O_IMPLEMENT(Token, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct Token *self = O_CAST(_self, Token());
  self = O_SUPER->ctor(self, app);
  self->name = O_CALL_CLASS(String(), new, "%s", va_arg(*app, char *));
  self->type = va_arg(*app, int);
  self->file = O_CALL_CLASS(String(), new, "%s", va_arg(*app, char *));
  self->line = va_arg(*app, int);

  printf("Created token: \"%s\".\n", self->name->data);

  return self;
}

O_OBJECT(Token, RefObject);
O_OBJECT_METHOD(Token, ctor);
O_END_OBJECT
