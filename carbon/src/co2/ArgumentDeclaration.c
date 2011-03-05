#include "co2/ArgumentDeclaration.h"
#include "co2/Type.h"
#include "co2/io.h"
#include "co2/FunctionType.h"
#include "grammar.h"

#define O_SUPER Declaration()

O_IMPLEMENT (ArgumentDeclaration, void *, ctor, (void *_self, va_list * app))
{
  struct ArgumentDeclaration *self = O_CAST (_self, ArgumentDeclaration ());
  self = O_SUPER->ctor (self, app);
  self->type = O_RETAIN_ARG (Type);
  return self;
}

O_IMPLEMENT (ArgumentDeclaration, void *, dtor, (void *_self))
{
  struct ArgumentDeclaration *self = O_CAST (_self, ArgumentDeclaration ());
  O_CALL(self->type, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (ArgumentDeclaration, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct ArgumentDeclaration *self = O_CAST (_self, ArgumentDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->type, accept, visitor);
  O_CALL (visitor, visit, self);
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT (ArgumentDeclaration, void, generate, (void *_self))
{
  struct ArgumentDeclaration *self = O_CAST (_self, ArgumentDeclaration ());
  if (self->name->type == VA_ARG)
    {
      O_CALL (self->name, generate);
    }
  else if (o_is_of (self->type, FunctionType ()))
    {
      struct FunctionType * function_type = O_CAST (self->type, FunctionType ());
      O_CALL (function_type, generate_named, self->name);
    }
  else
    {
      O_CALL (self->type, generate);
      fprintf (out, " ");
      O_CALL (self->name, generate);
    }
}

O_IMPLEMENT (ArgumentDeclaration, void, type_check, (void *_self))
{
  struct ArgumentDeclaration *self = O_CAST (_self, ArgumentDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->type, type_check);
  O_BRANCH_CALL (current_context, remove_last);
}

O_OBJECT (ArgumentDeclaration, Declaration);
O_OBJECT_METHOD (ArgumentDeclaration, ctor);
O_OBJECT_METHOD (ArgumentDeclaration, dtor);
O_OBJECT_METHOD (ArgumentDeclaration, accept);
O_OBJECT_METHOD (ArgumentDeclaration, generate);
O_OBJECT_METHOD (ArgumentDeclaration, type_check);
O_END_OBJECT
