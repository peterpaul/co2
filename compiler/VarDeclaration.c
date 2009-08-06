#include "VarDeclaration.h"
#include "Type.h"
#include "RefList.h"
#include "Expression.h"

#define O_SUPER Declaration()

O_IMPLEMENT(VarDeclaration, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct VarDeclaration * self = O_CAST(_self, VarDeclaration());
  self = O_SUPER->ctor(self, app);
  self->token = O_CALL(va_arg(*app, struct Token *), retain);
  self->expr = O_BRANCH_CALL(va_arg(*app, struct Expression *), retain);
  return self;
}

O_IMPLEMENT(VarDeclaration, void, set_type, (void *_self, struct Type * type), (_self, type))
{
  struct VarDeclaration * self = O_CAST(_self, VarDeclaration());
  self->type = O_CALL(type, retain);
}

O_OBJECT(VarDeclaration, Declaration);
O_OBJECT_METHOD(VarDeclaration, ctor);
O_OBJECT_METHOD(VarDeclaration, set_type);
O_END_OBJECT
