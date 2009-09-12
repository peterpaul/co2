#include "TokenExpression.h"
#include "grammar.tab.h"
#include "VarDeclaration.h"
#include "FunDeclaration.h"
#include "PrimitiveType.h"
#include "ArrayType.h"
#include "Token.h"

#define O_SUPER Expression()

O_IMPLEMENT(TokenExpression, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct TokenExpression * self = O_CAST(_self, TokenExpression());
  self = O_SUPER->ctor(self, app);
  self->token = O_CAST(va_arg(*app, struct Token *), Token());
  O_CALL(self->token, retain);
  return self;
}

O_IMPLEMENT(TokenExpression, void *, dtor, (void *_self), (_self))
{
  struct TokenExpression *self = O_CAST(_self, TokenExpression());
  O_CALL(self->token, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(TokenExpression, void, generate, (void *_self), (_self))
{
  struct TokenExpression *self = O_CAST(_self, TokenExpression());
  O_CALL(self->token, generate);
}

O_IMPLEMENT(TokenExpression, void, type_check, (void *_self), (_self))
{
  struct TokenExpression *self = O_CAST(_self, TokenExpression());
  switch (self->token->type)
    {
    case IDENTIFIER:
      if (o_is_a(self->decl, VarDeclaration()))
	{
	  struct VarDeclaration * var_decl = self->decl;
	  self->type = var_decl->type;
	}
      else if (o_is_a(self->decl, FunDeclaration()))
	{
	  struct FunDeclaration * fun_decl = self->decl;
	  self->type = fun_decl->type;
	}
      break;
    case INT_CONSTANT:
      {
	struct Token * token = O_CALL_CLASS(Token(), new, "int", INT, self->token->file, self->token->line);
	self->type = O_CALL_CLASS(PrimitiveType(), new, token);
      }
      break;
    case CHAR_CONSTANT:
    case FLOAT_CONSTANT:
    case STRING_CONSTANT:
      {
	struct Token * token = O_CALL_CLASS(Token(), new, "char", CHAR, self->token->file, self->token->line);
	struct Type * base_type = O_CALL_CLASS(PrimitiveType(), new, token);
	self->type = O_CALL_CLASS(ArrayType(), new, base_type);
      }
    default:
      break;
    }
  // check whether token type is an identifier or a constant.
  // lookup in the scope where this expression is located
  // when it is a var declaration, set self->type = decl->type
  // when it is a fun declaration, set self->type = decl->return_type
  // when it is a class declaration, set self->type = new PrimitiveType(self->token)
  // when it is an interface declaration, see class declaration
  // when it ia a macro declaration, ...?
}

O_OBJECT(TokenExpression, Expression);
O_OBJECT_METHOD(TokenExpression, ctor);
O_OBJECT_METHOD(TokenExpression, dtor);
O_OBJECT_METHOD(TokenExpression, generate);
O_OBJECT_METHOD(TokenExpression, type_check);
O_END_OBJECT
