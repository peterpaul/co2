#include "CastExpression.h"
#include "Type.h"
#include "io.h"
#include "ObjectType.h"
#include "Token.h"
#include "ClassDeclaration.h"
#include "InterfaceDeclaration.h"

#define O_SUPER Expression()

O_IMPLEMENT(CastExpression, void *, ctor, (void *_self, va_list *app))
{
  struct CastExpression * self = O_CAST(_self, CastExpression());
  self = O_SUPER->ctor(self, app);
  self->cast_type = O_RETAIN_ARG (Type);
  self->expression = O_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT(CastExpression, void *, dtor, (void *_self))
{
  struct CastExpression *self = O_CAST(_self, CastExpression());
  O_CALL(self->cast_type, retain);
  O_CALL(self->expression, retain);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(CastExpression, void, type_check, (void *_self))
{
  struct CastExpression *self = O_CAST(_self, CastExpression());
  O_CALL (self->expression, type_check);
  O_CALL (self->cast_type, type_check);
  if (o_is_of (self->cast_type, ObjectType ()) && o_is_of (self->expression->type, ObjectType()))
    {
      O_CALL (self->cast_type, assert_compatible, self->expression->type);
    }
  else
    {
      if (!O_CALL (self->cast_type, is_compatible, self->expression->type))
	{
	  struct Token *expr_token = O_CALL(self->expression->type, get_token);
	  struct String *expr_string = O_CALL (self->expression->type, to_string);
	  struct String *type_string = O_CALL (self->cast_type, to_string);
	  warning (expr_token, "casting possibly incompatible types %s to %s\n", expr_string->data, type_string->data);
	  O_CALL (expr_string, delete);
	  O_CALL (type_string, delete);
	}
    }
  self->type = O_CALL(self->cast_type, retain);
}

O_IMPLEMENT(CastExpression, void, generate, (void *_self))
{
  struct CastExpression *self = O_CAST(_self, CastExpression());
  if (o_is_of(self->cast_type, ObjectType())) 
    {
      struct ObjectType * object_type = O_CAST(self->cast_type, ObjectType());
      if (o_is_of(object_type->decl, ClassDeclaration()))
	{
	  fprintf(out, "O_CAST (");
	  O_CALL(self->expression, generate);
	  fprintf(out, ", ");
	  O_CALL(object_type->token, generate);
	  fprintf(out, " ())");
	}
      else if (o_is_of(object_type->decl, InterfaceDeclaration()))
	{
	  fprintf(out, "o_cast_interface (");
	  O_CALL(self->expression, generate);
	  fprintf(out, ", ");
	  O_CALL(object_type->token, generate);
	  fprintf(out, " ())");
	}
      else
	{
	  error (object_type->token, "Unhandled ObjectType: %s: %s", object_type->token->name->data, object_type->decl->name->name->data);
	}
    }
  else
    {
      fprintf(out, "(");
      fprintf(out, "(");
      O_CALL(self->cast_type, generate);
      fprintf(out, ") ");
      O_CALL(self->expression, generate);
      fprintf(out, ")");
    }
}

O_OBJECT(CastExpression, Expression);
O_OBJECT_METHOD(CastExpression, ctor);
O_OBJECT_METHOD(CastExpression, dtor);
O_OBJECT_METHOD(CastExpression, type_check);
O_OBJECT_METHOD(CastExpression, generate);
O_END_OBJECT
