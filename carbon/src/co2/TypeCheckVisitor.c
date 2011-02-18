#include "co2/TypeCheckVisitor.h"
  /* Declarations */
#include "co2/ArgumentDeclaration.h"
#include "co2/ClassDeclaration.h"
#include "co2/ConstructorDeclaration.h"
#include "co2/Declaration.h"
#include "co2/DestructorDeclaration.h"
#include "co2/FunctionDeclaration.h"
#include "co2/InterfaceDeclaration.h"
#include "co2/MacroDeclaration.h"
#include "co2/StructDeclaration.h"
#include "co2/TypeDeclaration.h"
#include "co2/VariableDeclaration.h"
  /* Statements */
#include "co2/CatchStatement.h"
#include "co2/CompoundStatement.h"
#include "co2/DeleteStatement.h"
#include "co2/DoStatement.h"
#include "co2/ExpressionStatement.h"
#include "co2/ForEachStatement.h"
#include "co2/ForStatement.h"
#include "co2/IfStatement.h"
#include "co2/ReturnStatement.h"
#include "co2/Statement.h"
#include "co2/TryStatement.h"
#include "co2/ThrowStatement.h"
#include "co2/WhileStatement.h"
  /* Expressions */
#include "co2/BinaryExpression.h"
#include "co2/CastExpression.h"
#include "co2/Expression.h"
#include "co2/FunctionCallExpression.h"
#include "co2/NestedExpression.h"
#include "co2/NewExpression.h"
#include "co2/NullExpression.h"
#include "co2/SizeExpression.h"
#include "co2/SuperExpression.h"
#include "co2/TokenExpression.h"
#include "co2/UnaryExpression.h"
#include "co2/VarArgExpression.h"
  /* Types */
#include "co2/ArrayType.h"
#include "co2/FunctionType.h"
#include "co2/ObjectType.h"
#include "co2/PrimitiveType.h"
#include "co2/Type.h"
  /* Support */
#include "grammar.h"
#include "co2/io.h"

void argument_error(struct FunctionCallExpression * self, struct FunctionType * function_type);

#define O_SUPER BaseCompileObjectVisitor()

void FunctionDeclaration_find_in_interface (void *_self, va_list * app);

O_IMPLEMENT(TypeCheckVisitor, void *, ctor, (void *_self, va_list *app))
{
  struct TypeCheckVisitor * self = O_CAST(_self, TypeCheckVisitor());
  self = O_SUPER->ctor(self, app);
  current_context = O_CALL_CLASS (Context (), new);
  O_BRANCH_CALL (current_context, retain);
  return self;
}

O_IMPLEMENT(TypeCheckVisitor, void *, dtor, (void *_self))
{
  struct TypeCheckVisitor *self = O_CAST(_self, TypeCheckVisitor());
  O_BRANCH_CALL (current_context, release);
  current_context = NULL;
  return O_SUPER->dtor(self);
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitArgumentDeclaration, (void *_self, void *_object), (_self, _object))
{
  /* nothing to do */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitClassDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct ClassDeclaration *self = O_CAST(_object, ClassDeclaration ());
  if (self->superclass)
    {
      struct Declaration *_super_class =
	O_CALL (self->scope, lookup, self->superclass);
      if (_super_class)
	{
	  /* TODO raise error when decl != ClassDeclaration () */
	  struct ClassDeclaration *super_class =
	    O_CAST (_super_class, ClassDeclaration ());
	  self->member_scope->parent = super_class->member_scope;
	}
    }
  /* TODO check whether:
   * 1. superclasses already implement interfaces, if so: fail
   * 2. foreach interface, methods already exists in other interface (including interfaces from superclasses), if so: fail
   * 3. class (including superclasses) implements all methods of all interfaces, if not: fail
   */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitConstructorDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct ConstructorDeclaration *self = O_CAST(_object, ConstructorDeclaration ());
  struct Declaration *class_decl =
    O_BRANCH_CALL (current_context, find, ClassDeclaration ());
  if (class_decl == NULL)
    {
      error (self->class_name,
	     "Constructors ('%s') are only allowed in class declarations\n",
	     self->class_name->name->data);
    }
  if (strcmp (class_decl->name->name->data, self->class_name->name->data) !=
      0)
    {
      error (self->class_name, "Constructor should be called '%s'\n",
	     class_decl->name->name->data);
    }

  self->type =
    O_CALL_CLASS (ObjectType (), new, self->class_name, class_decl);
  O_CALL (self->type, retain);
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitDestructorDeclaration, (void *_self, void *_object), (_self, _object))
{
  /* TODO check destructor name */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitFunctionDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct FunctionDeclaration *self = O_CAST(_object, FunctionDeclaration ());
  struct FunctionType *function_type = o_cast (self->type, FunctionType ());
  struct ClassDeclaration *class_decl =
    O_BRANCH_CALL (current_context, find, ClassDeclaration ());
  if (function_type->has_var_args)
    {
      if (class_decl == NULL && self->formal_arguments->length <= 1)
	{
	  error (self->name,
		 "variable argument list only supported on functions with at least one fixed argument.\n");
	}
    }
  if (class_decl && class_decl->interfaces)
    {
      O_CALL (class_decl->interfaces, map_args,
	      FunctionDeclaration_find_in_interface, self);
    }
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitInterfaceDeclaration, (void *_self, void *_object), (_self, _object))
{
  /* nothing to do */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitMacroDeclaration, (void *_self, void *_object), (_self, _object))
{
  /* TODO */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitStructDeclaration, (void *_self, void *_object), (_self, _object))
{
  /* nothing to do */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitTypeDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct TypeDeclaration *self = O_CAST(_object, TypeDeclaration ());
  if (self->is_struct)
    {
      struct PrimitiveType * type = O_CAST (self->type, PrimitiveType ());
      struct Declaration * decl = O_CALL (global_scope, lookup, type->token);
      if (decl)
	{
	  self->type = O_CALL_CLASS (ObjectType (), new, type->token, decl);
	  O_CALL (self->type, retain);
	  O_CALL (self->type, type_check);
	  O_CALL (type, release);
	}
    }
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitVariableDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct VariableDeclaration *self = O_CAST (_object, VariableDeclaration ());
  if (self->expr)
    {
      O_CALL (self->type, assert_compatible, self->expr->type);
    }

  if (self->scope->parent != NULL &&
      O_CALL (self->scope->parent, exists, self->name))
    {
      struct Declaration *first_decl =
	O_CALL (self->scope->parent, lookup, self->name);
      if (self->scope->type == first_decl->scope->type && first_decl->scope->type == CLASS_SCOPE)
	{
	  error (self->name, "'%s' already declared at %s:%d\n",
		 self->name->name->data, first_decl->name->file->name->data,
		 first_decl->name->line);
	}
    }
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitCatchStatement, (void *_self, void *_object), (_self, _object))
{
  struct CatchStatement *self = O_CAST (_object, CatchStatement ());
  if (!o_is_of (self->argument->type, ObjectType ()))
    {
      error (self->argument->name, "Expected class.\n");
    }
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitCompoundStatement, (void *_self, void *_object), (_self, _object))
{
  /* nothing to do */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitDeleteStatement, (void *_self, void *_object), (_self, _object))
{
  /* nothing to do */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitDoStatement, (void *_self, void *_object), (_self, _object))
{
  /* TODO, check whether condition is of type int (bool) */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitExpressionStatement, (void *_self, void *_object), (_self, _object))
{
  /* nothing to do */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitForEachStatement, (void *_self, void *_object), (_self, _object))
{
  /* TODO */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitForStatement, (void *_self, void *_object), (_self, _object))
{
  /* TODO, check whether condition is of type int (bool) */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitIfStatement, (void *_self, void *_object), (_self, _object))
{
  /* TODO, check whether condition is of type int (bool) */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitReturnStatement, (void *_self, void *_object), (_self, _object))
{
  struct ReturnStatement * self = O_CAST (_object, ReturnStatement ());
  self->function_context = O_BRANCH_CALL (current_context, find, FunctionDeclaration ());
  struct FunctionType *function_type = o_cast (self->function_context->type, FunctionType ());
  if (self->expr && self->expr->type)
    {
      O_CALL (function_type->return_type, assert_compatible, self->expr->type);
    }
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitThrowStatement, (void *_self, void *_object), (_self, _object))
{
  /* TODO check whether expression is a ClassDeclaration */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitTryStatement, (void *_self, void *_object), (_self, _object))
{
  /* nothing to do */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitWhileStatement, (void *_self, void *_object), (_self, _object))
{
  /* TODO, check whether condition is of type int (bool) */
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitBinaryExpression, (void *_self, void *_object), (_self, _object))
{
  struct BinaryExpression *self = O_CAST (_object, BinaryExpression ());
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitCastExpression, (void *_self, void *_object), (_self, _object))
{
  struct CastExpression *self = O_CAST(_object, CastExpression());
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitFunctionCallExpression, (void *_self, void *_object), (_self, _object))
{
  struct FunctionCallExpression *self = O_CAST (_object, FunctionCallExpression ());
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitNestedExpression, (void *_self, void *_object), (_self, _object))
{
  struct NestedExpression *self = O_CAST (_object, NestedExpression ());
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitNewExpression, (void *_self, void *_object), (_self, _object))
{
  struct NewExpression *self = O_CAST (_object, NewExpression ());
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitNullExpression, (void *_self, void *_object), (_self, _object))
{
  struct NullExpression *self = O_CAST (_object, NullExpression ());
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitSizeExpression, (void *_self, void *_object), (_self, _object))
{
  struct SizeExpression *self = O_CAST (_object, SizeExpression ());
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitSuperExpression, (void *_self, void *_object), (_self, _object))
{
  struct SuperExpression *self = O_CAST (_object, SuperExpression ());
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitTokenExpression, (void *_self, void *_object), (_self, _object))
{
  struct TokenExpression *self = O_CAST (_object, TokenExpression ());
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitUnaryExpression, (void *_self, void *_object), (_self, _object))
{
  struct UnaryExpression *self = O_CAST (_object, UnaryExpression ());
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitVarArgExpression, (void *_self, void *_object), (_self, _object))
{
  struct VarArgExpression *self = O_CAST (_object, VarArgExpression ());
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitArrayType, (void *_self, void *_object), (_self, _object))
{
  struct ArrayType *self = O_CAST (_object, ArrayType ());
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitFunctionType, (void *_self, void *_object), (_self, _object))
{
  struct FunctionType *self = O_CAST (_object, FunctionType ());
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitObjectType, (void *_self, void *_object), (_self, _object))
{
  struct ObjectType *self = O_CAST (_object, ObjectType ());
}

O_IMPLEMENT_IF(TypeCheckVisitor, void, visitPrimitiveType, (void *_self, void *_object), (_self, _object))
{
  struct PrimitiveType *self = O_CAST (_object, PrimitiveType ());
}

O_OBJECT(TypeCheckVisitor, BaseCompileObjectVisitor);
O_OBJECT_METHOD(TypeCheckVisitor, ctor);
O_OBJECT_METHOD(TypeCheckVisitor, dtor);

O_OBJECT_METHOD (TypeCheckVisitor, visitArgumentDeclaration);
O_OBJECT_METHOD (TypeCheckVisitor, visitClassDeclaration);
O_OBJECT_METHOD (TypeCheckVisitor, visitConstructorDeclaration);
O_OBJECT_METHOD (TypeCheckVisitor, visitDestructorDeclaration);
O_OBJECT_METHOD (TypeCheckVisitor, visitFunctionDeclaration);
O_OBJECT_METHOD (TypeCheckVisitor, visitInterfaceDeclaration);
O_OBJECT_METHOD (TypeCheckVisitor, visitMacroDeclaration);
O_OBJECT_METHOD (TypeCheckVisitor, visitStructDeclaration);
O_OBJECT_METHOD (TypeCheckVisitor, visitTypeDeclaration);
O_OBJECT_METHOD (TypeCheckVisitor, visitVariableDeclaration);

O_OBJECT_METHOD (TypeCheckVisitor, visitCatchStatement);
O_OBJECT_METHOD (TypeCheckVisitor, visitCompoundStatement);
O_OBJECT_METHOD (TypeCheckVisitor, visitDeleteStatement);
O_OBJECT_METHOD (TypeCheckVisitor, visitDoStatement);
O_OBJECT_METHOD (TypeCheckVisitor, visitExpressionStatement);
O_OBJECT_METHOD (TypeCheckVisitor, visitForEachStatement);
O_OBJECT_METHOD (TypeCheckVisitor, visitForStatement);
O_OBJECT_METHOD (TypeCheckVisitor, visitIfStatement);
O_OBJECT_METHOD (TypeCheckVisitor, visitReturnStatement);
O_OBJECT_METHOD (TypeCheckVisitor, visitThrowStatement);
O_OBJECT_METHOD (TypeCheckVisitor, visitTryStatement);
O_OBJECT_METHOD (TypeCheckVisitor, visitWhileStatement);

O_OBJECT_METHOD (TypeCheckVisitor, visitBinaryExpression);
O_OBJECT_METHOD (TypeCheckVisitor, visitCastExpression);
O_OBJECT_METHOD (TypeCheckVisitor, visitFunctionCallExpression);
O_OBJECT_METHOD (TypeCheckVisitor, visitNestedExpression);
O_OBJECT_METHOD (TypeCheckVisitor, visitNewExpression);
O_OBJECT_METHOD (TypeCheckVisitor, visitNullExpression);
O_OBJECT_METHOD (TypeCheckVisitor, visitSizeExpression);
O_OBJECT_METHOD (TypeCheckVisitor, visitSuperExpression);
O_OBJECT_METHOD (TypeCheckVisitor, visitTokenExpression);
O_OBJECT_METHOD (TypeCheckVisitor, visitUnaryExpression);
O_OBJECT_METHOD (TypeCheckVisitor, visitVarArgExpression);

O_OBJECT_METHOD (TypeCheckVisitor, visitArrayType);
O_OBJECT_METHOD (TypeCheckVisitor, visitFunctionType);
O_OBJECT_METHOD (TypeCheckVisitor, visitObjectType);
O_OBJECT_METHOD (TypeCheckVisitor, visitPrimitiveType);

O_OBJECT_IF(CompileObjectVisitor);
O_OBJECT_IF_METHOD(TypeCheckVisitor, visitClassDeclaration);
O_OBJECT_IF_END
O_END_OBJECT
