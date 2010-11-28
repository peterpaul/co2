#include "Token.h"
#include "String.h"
#include "io.h"

#define O_SUPER CompileObject()

O_IMPLEMENT (Token, void *, ctor, (void *_self, va_list * app))
{
  struct Token *self = O_CAST (_self, Token ());
  self = O_SUPER->ctor (self, app);
  self->name = O_CALL_CLASS (String (), new, "%s", va_arg (*app, char *));
  self->type = va_arg (*app, int);
  self->file = O_CALL_CLASS (String (), new, "%s", va_arg (*app, char *));
  self->line = va_arg (*app, int);
  return self;
}

O_IMPLEMENT (Token, void, generate, (void *_self))
{
  struct Token *self = O_CAST (_self, Token ());
  fprintf (out, "%s", self->name->data);
}

O_OBJECT (Token, CompileObject);
O_OBJECT_METHOD (Token, ctor);
O_OBJECT_METHOD (Token, generate);
O_END_OBJECT
