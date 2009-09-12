#include "PrimitiveType.h"
#include "Token.h"
#include "io.h"
#include "grammar.tab.h"

#define O_SUPER Type()

O_IMPLEMENT(PrimitiveType, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct PrimitiveType * self = O_CAST(_self, PrimitiveType());
  self = O_SUPER->ctor(self, app);
  self->token = O_CAST(va_arg(*app, struct Token *), Token());
  O_CALL(self->token, retain);
  return self;
}

O_IMPLEMENT(PrimitiveType, void *, dtor, (void *_self), (_self))
{
  struct PrimitiveType *self = O_CAST(_self, PrimitiveType());
  O_CALL(self->token, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(PrimitiveType, void, generate, (void *_self), (_self))
{
  struct PrimitiveType *self = O_CAST(_self, PrimitiveType());
  if (self->token->type == TYPE_IDENTIFIER) 
    {
      fprintf(out, "struct ");
      O_CALL(self->token, generate);
      fprintf(out, "*");
    }
  else
    {
      O_CALL(self->token, generate);
    }
}

O_IMPLEMENT(PrimitiveType, struct Token *, get_token, (void *_self), (_self))
{
  struct PrimitiveType *self = O_CAST(_self, PrimitiveType());
  return self->token;
}

O_IMPLEMENT(PrimitiveType, struct String *, to_string, (void *_self), (_self))
{
  struct PrimitiveType *self = O_CAST(_self, PrimitiveType());
  return O_CALL_CLASS(String(), new, "%s", self->token->name->data);;
}

O_OBJECT(PrimitiveType, Type);
O_OBJECT_METHOD(PrimitiveType, ctor);
O_OBJECT_METHOD(PrimitiveType, dtor);
O_OBJECT_METHOD(PrimitiveType, generate);
O_OBJECT_METHOD(PrimitiveType, get_token);
O_OBJECT_METHOD(PrimitiveType, to_string);
O_END_OBJECT
