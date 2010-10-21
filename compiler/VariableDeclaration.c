#include "VariableDeclaration.h"
#include "Type.h"
#include "RefList.h"
#include "Expression.h"
#include "io.h"

#define O_SUPER Declaration()

O_IMPLEMENT(VariableDeclaration, void *, ctor, (void *_self, va_list *app))
{
  struct VariableDeclaration * self = O_CAST(_self, VariableDeclaration());
  self = O_SUPER->ctor(self, app);
  self->expr = O_BRANCH_CALL(va_arg(*app, struct Expression *), retain);
  return self;
}

O_IMPLEMENT(VariableDeclaration, void, set_type, (void *_self, struct Type * type))
{
  struct VariableDeclaration * self = O_CAST(_self, VariableDeclaration());
  self->type = O_CALL(type, retain);
}

O_IMPLEMENT(VariableDeclaration, void, generate, (void *_self))
{
  struct VariableDeclaration * self = O_CAST(_self, VariableDeclaration());
  O_CALL(self->type, generate);
  fprintf(out, " ");
  O_CALL(self->name, generate);
  if (self->expr)
    {
      fprintf(out, "=");
      O_CALL(self->expr, generate);
    }
  fprintf(out, ";\n");
}

O_IMPLEMENT(VariableDeclaration, void, type_check, (void *_self))
{
  struct VariableDeclaration * self = O_CAST(_self, VariableDeclaration());
  if (self->expr)
    {
      O_CALL(self->expr, type_check);
      O_CALL(self->type, assert_compatible, self->expr->type);
    }
}

O_OBJECT(VariableDeclaration, Declaration);
O_OBJECT_METHOD(VariableDeclaration, ctor);
O_OBJECT_METHOD(VariableDeclaration, set_type);
O_OBJECT_METHOD(VariableDeclaration, generate);
O_OBJECT_METHOD(VariableDeclaration, type_check);
O_END_OBJECT
