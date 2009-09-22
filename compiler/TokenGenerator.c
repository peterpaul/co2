#include "TokenGenerator.h"
#include "grammar.tab.h"

#define O_SUPER Singleton()

O_IMPLEMENT(TokenGenerator, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct TokenGenerator * self = O_CAST(_self, TokenGenerator());
  self = O_SUPER->ctor(self, app);
  self->value = 0;
  return self;
}

O_IMPLEMENT(TokenGenerator, void *, dtor, (void *_self), (_self))
{
  struct TokenGenerator *self = O_CAST(_self, TokenGenerator());
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(TokenGenerator, struct Token *, create, (void *_self, struct Token * token), (_self, token))
{
  struct TokenGenerator *self = O_CAST(_self, TokenGenerator());
  struct String * name = O_CALL_CLASS(String(), new, "_%s_%d", token->name->data, self->value);
  struct Token * retval = O_CALL_CLASS(Token(), new, name->data, IDENTIFIER, token->file->data, token->line);
  self->value ++;
  O_CALL(name, delete);
  return retval;
}

O_OBJECT(TokenGenerator, Singleton);
O_OBJECT_METHOD(TokenGenerator, ctor);
O_OBJECT_METHOD(TokenGenerator, dtor);
O_OBJECT_METHOD(TokenGenerator, create);
O_END_OBJECT
