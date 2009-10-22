#include "TokenExpression.h"
#include "grammar.tab.h"
#include "VarDeclaration.h"
#include "FunDeclaration.h"
#include "ArgDeclaration.h"
#include "PrimitiveType.h"
#include "ArrayType.h"
#include "FunctionType.h"
#include "Token.h"
#include "grammar.tab.h"
#include "io.h"

#define O_SUPER Expression()

O_IMPLEMENT(TokenExpression, void *, ctor, (void *_self, va_list *app))
{
  struct TokenExpression * self = O_CAST(_self, TokenExpression());
  self = O_SUPER->ctor(self, app);
  self->token = O_CAST(va_arg(*app, struct Token *), Token());
  O_CALL(self->token, retain);
  if (self->token->type == IDENTIFIER)
    {
      self->scope = current_scope;
    }
  self->check_global_scope = true;
  return self;
}

O_IMPLEMENT(TokenExpression, void *, dtor, (void *_self))
{
  struct TokenExpression *self = O_CAST(_self, TokenExpression());
  O_CALL(self->token, release);
  O_BRANCH_CALL(self->decl, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(TokenExpression, void, generate_left, (void *_self, bool left))
{
  struct TokenExpression *self = O_CAST(_self, TokenExpression());
  if (left && self->decl && self->decl->class_decl)
    {
      fprintf(out, "self->");
    }
  O_CALL(self->token, generate);
}

O_IMPLEMENT(TokenExpression, void, set_scope, (void *_self, void *_scope))
{
  struct TokenExpression *self = O_CAST(_self, TokenExpression());
  self->scope = O_CAST(_scope, Scope());
  self->check_global_scope = false;
}

O_IMPLEMENT(TokenExpression, void, type_check, (void *_self))
{
  struct TokenExpression *self = O_CAST(_self, TokenExpression());
  switch (self->token->type)
    {
    case IDENTIFIER:
      O_CALL(self, lookup);
      if (!self->decl) 
	{
	  return;
	}
      self->type = O_CALL(self->decl->type, retain);
      break;
    case INT_CONSTANT:
      {
	struct Token * token = O_CALL_CLASS(Token(), new, "int", INT, self->token->file, self->token->line);
	self->type = O_CALL_CLASS(PrimitiveType(), new, token);
	O_CALL(self->type, retain);
      }
      break;
    case STRING_CONSTANT:
      {
	struct Token * token = O_CALL_CLASS(Token(), new, "char", CHAR, self->token->file, self->token->line);
	struct Type * base_type = O_CALL_CLASS(PrimitiveType(), new, token);
	self->type = O_CALL_CLASS(ArrayType(), new, base_type);
	O_CALL(self->type, retain);
      }
      break;
    case CHAR_CONSTANT:
      {
	struct Token * token = O_CALL_CLASS(Token(), new, "char", CHAR, self->token->file, self->token->line);
	self->type = O_CALL_CLASS(PrimitiveType(), new, token);
	O_CALL(self->type, retain);
      }
      break;
    case FLOAT_CONSTANT:
      {
	struct Token * token = O_CALL_CLASS(Token(), new, "float", FLOAT, self->token->file, self->token->line);
	self->type = O_CALL_CLASS(PrimitiveType(), new, token);
	O_CALL(self->type, retain);
      }
      break;
    default:
      error(self->token, "Unhandled TokenExpression: %s\n", self->token->name->data);
      break;
    }
}

O_IMPLEMENT(TokenExpression, void, lookup, (void *_self))
{
 struct TokenExpression *self = O_CAST(_self, TokenExpression());
 if (self->check_global_scope) 
   {
     if (O_CALL(self->scope, exists, self->token))
       {
	 self->decl = O_CALL(self->scope, lookup, self->token);
       }
     else
       {
	 self->decl = O_CALL(global_scope, lookup, self->token);
       }
   }
 else
   {
     self->decl = O_CALL(self->scope, lookup, self->token);
   }
 O_BRANCH_CALL(self->decl, retain);
}

O_OBJECT(TokenExpression, Expression);
O_OBJECT_METHOD(TokenExpression, ctor);
O_OBJECT_METHOD(TokenExpression, dtor);
O_OBJECT_METHOD(TokenExpression, generate_left);
O_OBJECT_METHOD(TokenExpression, type_check);
O_OBJECT_METHOD(TokenExpression, set_scope);
O_OBJECT_METHOD(TokenExpression, lookup);
O_END_OBJECT
