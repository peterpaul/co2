#include "BinaryExpression.h"
#include "Token.h"
#include "Type.h"
#include "ObjectType.h"
#include "Declaration.h"
#include "ArrayType.h"
#include "ObjectTypeDeclaration.h"
#include "io.h"
#include "PrimitiveType.h"
#include "grammar.tab.h"

#define O_SUPER Expression()

O_IMPLEMENT (BinaryExpression, void *, ctor, (void *_self, va_list * app))
{
  struct BinaryExpression *self = O_CAST (_self, BinaryExpression ());
  self = O_SUPER->ctor (self, app);
  self->operand[0] =
    O_BRANCH_CAST (va_arg (*app, struct Expression *), Expression ());
  self->operator = O_BRANCH_CAST (va_arg (*app, struct Token *), Token ());
  self->operand[1] =
    O_BRANCH_CAST (va_arg (*app, struct Expression *), Expression ());
  O_BRANCH_CALL (self->operand[0], retain);
  O_BRANCH_CALL (self->operand[1], retain);
  O_BRANCH_CALL (self->operator, retain);
  return self;
}

O_IMPLEMENT (BinaryExpression, void *, dtor, (void *_self))
{
  struct BinaryExpression *self = O_CAST (_self, BinaryExpression ());
  O_BRANCH_CALL (self->operand[0], release);
  O_BRANCH_CALL (self->operand[1], release);
  O_BRANCH_CALL (self->operator, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (BinaryExpression, void, generate, (void *_self))
{
  struct BinaryExpression *self = O_CAST (_self, BinaryExpression ());
  switch (self->operator->type)
    {
    case '.':
      O_CALL (self->operand[0], generate);
      fprintf (out, "->");
      O_CALL (self->operand[1], generate_left, false);
      break;
    case '[':
      O_CALL (self->operand[0], generate);
      O_CALL (self->operator, generate);
      O_CALL (self->operand[1], generate);
      fprintf (out, "]");
      break;
    case '%':
      {
	struct PrimitiveType * type = O_CAST (self->type, PrimitiveType ());
	switch (type->token->type)
	  {
	  case FLOAT:
	    fprintf (out, "fmodf (");
	    O_CALL (self->operand[0], generate);
	    fprintf (out, ", ");
	    O_CALL (self->operand[1], generate);
	    fprintf (out, ")");
	    break;
	  default:
	    O_CALL (self->operand[0], generate);
	    fprintf (out, " ");
	    O_CALL (self->operator, generate);
	    fprintf (out, " ");
	    O_CALL (self->operand[1], generate);
	    break;
	  }
      }
      break;
    case REMINDER:
      {
	struct PrimitiveType * type = O_CAST (self->type, PrimitiveType ());
	switch (type->token->type)
	  {
	  case FLOAT:
	    O_CALL (self->operand[0], generate);
	    fprintf (out, " = ");
	    fprintf (out, "fmodf (");
	    O_CALL (self->operand[0], generate);
	    fprintf (out, ", ");
	    O_CALL (self->operand[1], generate);
	    fprintf (out, ")");
	    break;
	  default:
	    O_CALL (self->operand[0], generate);
	    fprintf (out, " ");
	    O_CALL (self->operator, generate);
	    fprintf (out, " ");
	    O_CALL (self->operand[1], generate);
	    break;
	  }
      }
      break;
    case '^':
      {
	struct PrimitiveType * type = O_CAST (self->type, PrimitiveType ());
	switch (type->token->type)
	  {
	  case INT:
	    fprintf (out, "ipow (");
	  break;
	  case UNSIGNED:
	    fprintf (out, "upow (");
	    break;
	  case FLOAT:
	    fprintf (out, "powf (");
	    break;
	  }
	O_CALL (self->operand[0], generate);
	fprintf (out, ", ");
	O_CALL (self->operand[1], generate);
	fprintf (out, ")");
      }
      break;
    case POWER:
      {
	struct PrimitiveType * type = O_CAST (self->type, PrimitiveType ());
	O_CALL (self->operand[0], generate);
	fprintf (out, " = ");
	switch (type->token->type)
	  {
	  case INT:
	    fprintf (out, "ipow (");
	  break;
	  case UNSIGNED:
	    fprintf (out, "upow (");
	    break;
	  case FLOAT:
	    fprintf (out, "powf (");
	    break;
	  }
	O_CALL (self->operand[0], generate);
	fprintf (out, ", ");
	O_CALL (self->operand[1], generate);
	fprintf (out, ")");
      }
      break;
    default:
      O_CALL (self->operand[0], generate);
      fprintf (out, " ");
      O_CALL (self->operator, generate);
      fprintf (out, " ");
      O_CALL (self->operand[1], generate);
      break;
    }
}

O_IMPLEMENT (BinaryExpression, void, type_check, (void *_self))
{
  struct BinaryExpression *self = O_CAST (_self, BinaryExpression ());
  switch (self->operator-> type)
    {
    case '.':
      O_CALL (self->operand[0], type_check);
      struct ObjectType *object_type =
	o_cast (self->operand[0]->type, ObjectType ());
      struct ObjectTypeDeclaration *class_decl =
	o_cast (object_type->decl, ObjectTypeDeclaration ());
      O_CALL (self->operand[1], set_scope, class_decl->member_scope);
      O_CALL (self->operand[1], type_check);
      self->type = O_BRANCH_CALL (self->operand[1]->type, retain);
      break;
    case '[':
      O_CALL (self->operand[0], type_check);
      struct ArrayType *array_type =
	o_cast (self->operand[0]->type, ArrayType ());
      O_CALL (self->operand[1], type_check);
      self->type = O_CALL (array_type->base_type, retain);
      break;
    default:
      O_CALL (self->operand[0], type_check);
      O_CALL (self->operand[1], type_check);
      O_BRANCH_CALL (self->operand[0]->type, assert_compatible,
		     self->operand[1]->type);
      self->type = O_BRANCH_CALL (self->operand[0]->type, retain);
      break;
    }
}

O_IMPLEMENT (BinaryExpression, void, set_scope, (void *_self, void *_scope))
{
  struct BinaryExpression *self = O_CAST (_self, BinaryExpression ());
  O_CALL (self->operand[0], set_scope, _scope);
}

O_OBJECT (BinaryExpression, Expression);
O_OBJECT_METHOD (BinaryExpression, ctor);
O_OBJECT_METHOD (BinaryExpression, dtor);
O_OBJECT_METHOD (BinaryExpression, generate);
O_OBJECT_METHOD (BinaryExpression, type_check);
O_OBJECT_METHOD (BinaryExpression, set_scope);
O_END_OBJECT
