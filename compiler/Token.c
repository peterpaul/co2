#include "Token.h"
#include "io.h"

#define O_SUPER CompileObject()

O_IMPLEMENT (Token, void *, ctor, (void *_self, va_list * app))
{
  struct Token *self = O_CAST (_self, Token ());
  self = O_SUPER->ctor (self, app);
  self->name = O_CALL_CLASS (String (), new, "%s", va_arg (*app, char *));
  O_CALL (self->name, retain);
  self->type = va_arg (*app, int);
  self->file = O_CALL_CLASS (String (), new, "%s", va_arg (*app, char *));
  O_CALL (self->file, retain);
  self->line = va_arg (*app, int);
  return self;
}

O_IMPLEMENT (Token, void *, ctor_from_token, (void *_self, va_list * app))
{
  struct Token *self = O_CAST (_self, Token ());
  self = O_SUPER->ctor (self, app);
  struct Token *base = O_CAST (va_arg (*app, struct Token *), Token ());
  self->name = O_CALL_CLASS (String (), new, "%s", va_arg (*app, char *));
  O_CALL (self->name, retain);
  self->type = va_arg (*app, int);
  self->file = O_CALL_CLASS (String (), new, "%s", base->file->data);
  O_CALL (self->file, retain);
  /* TODO: should eventually be:
   *
   * self->file = O_CALL (base->file, retain);
   */
  self->line = base->line;
  return self;
}

O_IMPLEMENT (Token, void *, dtor, (void *_self))
{
  struct Token *self = O_CAST (_self, Token ());
  O_CALL (self->name, release);
  O_CALL (self->file, release);
}

O_IMPLEMENT (Token, void, generate, (void *_self))
{
  struct Token *self = O_CAST (_self, Token ());
  fprintf (out, "%s", self->name->data);
}

O_OBJECT (Token, CompileObject);
O_OBJECT_METHOD (Token, ctor);
O_OBJECT_METHOD (Token, ctor_from_token);
O_OBJECT_METHOD (Token, dtor);
O_OBJECT_METHOD (Token, generate);
O_END_OBJECT
