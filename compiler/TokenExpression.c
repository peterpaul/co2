#include "Declaration.h"
#include "Scope.h"
#include "TokenExpression.h"
#include "grammar.tab.h"
#include "PrimitiveType.h"
#include "ArrayType.h"
#include "FunctionType.h"
#include "ObjectType.h"
#include "ClassDeclaration.h"
#include "Token.h"
#include "grammar.tab.h"
#include "io.h"

#define O_SUPER Expression()

O_IMPLEMENT (TokenExpression, void *, ctor, (void *_self, va_list * app))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  self = O_SUPER->ctor (self, app);
  self->token = O_RETAIN_ARG (Token);
  if (self->token->type == IDENTIFIER)
    {
      self->scope = current_scope;
    }
  self->check_global_scope = true;
  return self;
}

O_IMPLEMENT (TokenExpression, void *, dtor, (void *_self))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  O_CALL (self->token, release);
  O_BRANCH_CALL (self->decl, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (TokenExpression, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  O_CALL (self->token, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (TokenExpression, void, generate_left, (void *_self, bool left))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  if (self->token->type == VA_ARG)
    {
      fprintf (out, "ap");
      return;
    }
  if (left && self->decl && self->decl->class_decl)
    {
      fprintf (out, "self->");
    }
  O_CALL (self->token, generate);
}

O_IMPLEMENT (TokenExpression, void, set_scope, (void *_self, void *_scope))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  self->scope = O_CAST (_scope, Scope ());
  self->check_global_scope = false;
}

O_IMPLEMENT (TokenExpression, void, type_check, (void *_self))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  switch (self->token->type)
    {
    case IDENTIFIER:
      O_CALL (self, lookup);
      if (!self->decl)
	{
	  return;
	}
      self->type = O_CALL (O_CALL (self->decl->type, get_declared_type), retain);
      break;
    case SELF:
      {
	struct ClassDeclaration *class_decl =
	  O_CALL (current_context, find, ClassDeclaration ());
	self->type =
	  O_CALL_CLASS (ObjectType (), new, class_decl->name, class_decl);
	O_CALL (self->type, retain);
      }
      break;
    case INT_CONSTANT:
      {
	struct Token *token =
	  O_CALL_CLASS (Token (), new_ctor, _Token_ctor_from_token, self->token, "int", INT);
	self->type = O_CALL_CLASS (PrimitiveType (), new, token);
	O_CALL (self->type, retain);
      }
      break;
    case STRING_CONSTANT:
      {
	struct Token *token =
	  O_CALL_CLASS (Token (), new_ctor, _Token_ctor_from_token, self->token, "char", CHAR);
	struct Type *base_type = O_CALL_CLASS (PrimitiveType (), new, token);
	self->type = O_CALL_CLASS (ArrayType (), new, base_type);
	O_CALL (self->type, retain);
      }
      break;
    case CHAR_CONSTANT:
      {
	struct Token *token =
	  O_CALL_CLASS (Token (), new_ctor, _Token_ctor_from_token, self->token, "char", CHAR);
	self->type = O_CALL_CLASS (PrimitiveType (), new, token);
	O_CALL (self->type, retain);
      }
      break;
    case FLOAT_CONSTANT:
      {
	struct Token *token =
	  O_CALL_CLASS (Token (), new_ctor, _Token_ctor_from_token, self->token, "float", FLOAT);
	self->type = O_CALL_CLASS (PrimitiveType (), new, token);
	O_CALL (self->type, retain);
      }
      break;
    case VA_ARG:
      {
	struct Token *token =
	  O_CALL_CLASS (Token (), new_ctor, _Token_ctor_from_token, self->token, "va_list", VA_LIST);
	self->type = O_CALL_CLASS (PrimitiveType (), new, token);
	O_CALL (self->type, retain);
      }
      break;
    default:
      error (self->token, "Unhandled TokenExpression: '%s'\n",
	     self->token->name->data);
      break;
    }
  O_CALL (self->type, type_check);
}

O_IMPLEMENT (TokenExpression, void, lookup, (void *_self))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  if (self->check_global_scope)
    {
      if (O_CALL (self->scope, exists, self->token))
	{
	  self->decl = O_CALL (self->scope, lookup, self->token);
	}
      else
	{
	  self->decl = O_CALL (global_scope, lookup, self->token);
	}
    }
  else
    {
      self->decl = O_CALL (self->scope, lookup, self->token);
    }
  O_BRANCH_CALL (self->decl, retain);
}

O_IMPLEMENT(TokenExpression, struct Token *, get_token, (void *_self))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  return self->token;
}

O_OBJECT (TokenExpression, Expression);
O_OBJECT_METHOD (TokenExpression, ctor);
O_OBJECT_METHOD (TokenExpression, dtor);
O_OBJECT_METHOD (TokenExpression, accept);
O_OBJECT_METHOD (TokenExpression, generate_left);
O_OBJECT_METHOD (TokenExpression, type_check);
O_OBJECT_METHOD (TokenExpression, set_scope);
O_OBJECT_METHOD (TokenExpression, lookup);
O_OBJECT_METHOD (TokenExpression, get_token);
O_END_OBJECT
