#include "VarDeclaration.h"
#include "Type.h"
#include "RefList.h"
#include "Expression.h"
#include "io.h"

#define O_SUPER Declaration()

O_IMPLEMENT(VarDeclaration, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct VarDeclaration * self = O_CAST(_self, VarDeclaration());
  self = O_SUPER->ctor(self, app);
  self->expr = O_BRANCH_CALL(va_arg(*app, struct Expression *), retain);
  return self;
}

O_IMPLEMENT(VarDeclaration, void, set_type, (void *_self, struct Type * type), (_self, type))
{
  struct VarDeclaration * self = O_CAST(_self, VarDeclaration());
  self->type = O_CALL(type, retain);
}

O_IMPLEMENT(VarDeclaration, void, generate, (void *_self), (_self))
{
  struct VarDeclaration * self = O_CAST(_self, VarDeclaration());
  O_CALL(self->type, generate);
  fprintf(out, " %s", self->name->name->data);
  if (self->expr)
    {
      fprintf(out, " = ");
      O_CALL(self->expr, generate);
    }
  fprintf(out, ";\n");
}

O_OBJECT(VarDeclaration, Declaration);
O_OBJECT_METHOD(VarDeclaration, ctor);
O_OBJECT_METHOD(VarDeclaration, set_type);
O_OBJECT_METHOD(VarDeclaration, generate);
O_END_OBJECT
