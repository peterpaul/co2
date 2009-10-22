#include "MacroDeclaration.h"

#define O_SUPER Declaration()

O_IMPLEMENT(MacroDeclaration, void *, ctor, (void *_self, va_list *app))
{
  struct MacroDeclaration * self = O_CAST(_self, MacroDeclaration());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT(MacroDeclaration, void *, dtor, (void *_self))
{
  struct MacroDeclaration *self = O_CAST(_self, MacroDeclaration());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_OBJECT(MacroDeclaration, Declaration);
O_OBJECT_METHOD(MacroDeclaration, ctor);
O_OBJECT_METHOD(MacroDeclaration, dtor);
O_END_OBJECT
