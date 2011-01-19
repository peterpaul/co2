#include "GenerateSourceVisitor.h"
#include "CompileObject.h"
#include "ArgumentDeclaration.h"
#include "ClassDeclaration.h"
#include "VariableDeclaration.h"
#include "FunctionDeclaration.h"
#include "InterfaceDeclaration.h"
#include "ConstructorDeclaration.h"
#include "DestructorDeclaration.h"
#include "StructDeclaration.h"
#include "TypeDeclaration.h"
#include "FunctionType.h"
#include "Statement.h"
#include "PrimitiveType.h"
#include "grammar.tab.h"
#include "Expression.h"
#include "FunctionType.h"
#include "Type.h"
#include "io.h"

int new_constructor_filter (void *_constructor);
void ClassDeclaration_generate_constructor_implementation (void *_constructor_decl, va_list * app);
void ClassDeclaration_generate_destructor_implementation (void *_destructor_decl, va_list * app);
void ClassDeclaration_generate_method_implementation (void *_method_decl, va_list * app);
void ClassDeclaration_generate_constructor_registration_2 (void *_constructor_decl, va_list * app);
void ClassDeclaration_generate_destructor_registration_2 (void *_destructor_decl, va_list * app);
void ClassDeclaration_generate_method_implementation_2 (void *_interface_name, va_list * app);

void FunctionDeclaration_generate_formal_arg (void *_decl, va_list * ap);

void InterfaceDeclaration_generate_method_implementation (void *_method_decl, va_list * app);

struct FunctionType *get_type (struct DestructorDeclaration *self);

#define O_SUPER BaseCompileObjectVisitor()

O_IMPLEMENT(GenerateSourceVisitor, void *, ctor, (void *_self, va_list *app))
{
  struct GenerateSourceVisitor * self = O_CAST(_self, GenerateSourceVisitor());
  self = O_SUPER->ctor(self, app);
  self->fp = va_arg (*app, FILE *);
  return self;
}

O_IMPLEMENT(GenerateSourceVisitor, void *, dtor, (void *_self))
{
  struct GenerateSourceVisitor *self = O_CAST(_self, GenerateSourceVisitor());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

// O_IMPLEMENT_IF(GenerateSourceVisitor, void, visit, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitArgumentDeclaration, (void *_self, void *object), (_self, object)) {}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitClassDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct ClassDeclaration *self = O_CAST(_object, ClassDeclaration ());

  /* filter the members */
  struct RefList *attributes =
    O_CALL (self->members, filter_args, type_filter, VariableDeclaration ());
  O_CALL (attributes, retain);
  struct RefList *methods =
    O_CALL (self->members, filter_args, type_filter, FunctionDeclaration ());
  O_CALL (methods, retain);
  struct RefList *constructors =
    O_CALL (self->members, filter_args, type_filter,
	    ConstructorDeclaration ());
  O_CALL (constructors, retain);
  struct RefList *destructors =
    O_CALL (self->members, filter_args, type_filter,
	    DestructorDeclaration ());
  O_CALL (destructors, retain);

  struct RefList *new_methods =
    O_CALL (methods, filter_args, Declaration_new_member_filter,
	    FunctionDeclaration ());
  O_CALL (new_methods, retain);

  struct RefList *new_constructors =
    O_CALL (constructors, filter, new_constructor_filter);
  O_CALL (new_constructors, retain);

  fprintf (out, "#define O_SUPER ");
  generate_superclass (self);
  fprintf (out, " ()\n\n");

  O_CALL (constructors, map_args,
	  ClassDeclaration_generate_constructor_implementation, self);
  O_CALL (destructors, map_args,
	  ClassDeclaration_generate_destructor_implementation, self);
  O_CALL (methods, map_args, ClassDeclaration_generate_method_implementation,
	  self);

  fprintf (out, "O_OBJECT (");
  O_CALL (self->name, generate);
  fprintf (out, ", ");
  generate_superclass (self);
  fprintf (out, ");\n");

  O_CALL (constructors, map_args,
	  ClassDeclaration_generate_constructor_registration_2, self);
  O_CALL (destructors, map_args,
	  ClassDeclaration_generate_destructor_registration_2, self);
  O_CALL (methods, map_args,
	  ObjectTypeDeclaration_generate_method_registration_2, self);

  if (self->interfaces)
    {
      O_CALL (self->interfaces, map_args,
	      ClassDeclaration_generate_method_implementation_2, self->name);
    }

  fprintf (out, "O_END_OBJECT\n\n");

  fprintf (out, "#undef O_SUPER\n\n");

  O_CALL (attributes, release);
  O_CALL (methods, release);
  O_CALL (new_methods, release);
  O_CALL (constructors, release);
  O_CALL (destructors, release);
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitConstructorDeclaration, (void *_self, void *object), (_self, object)) {}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceVisitor *visitor = O_CAST (_self, GenerateSourceVisitor ());
  struct Declaration *self = O_CAST (_object, Declaration ());
  if (self->scope && self->scope->type != GLOBAL_SCOPE)
    {
      // only generate global declarations
      return;
    }
  else if (self->include_file)
    {
      // don't generate if external definition
      fprintf (out, "#include ");
      O_CALL (self->include_file, generate);
      fprintf (out, "\n");
      return;
    }
  else
    {
      O_SUPER->visitDeclaration(visitor, self);
    }
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitDestructorDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceVisitor *visitor = O_CAST (_self, GenerateSourceVisitor ());
  struct DestructorDeclaration *self = O_CAST (_object, DestructorDeclaration ());

  O_CALL (get_type (self)->return_type, generate);
  fprintf (out, " ");
  O_CALL (self->name, generate);
  fprintf (out, "(");
  fprintf (out, ")\n");
  fprintf (out, "{\n");
  O_CALL (self->body, generate);
  fprintf (out, "}\n");
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitFunctionDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct FunctionDeclaration *self = O_CAST(_object, FunctionDeclaration ());

  bool first_formal_arg = true;
  struct FunctionType *function_type = get_type (self);
  O_CALL (function_type->return_type, generate);
  fprintf (out, " ");
  O_CALL (self->name, generate);
  fprintf (out, " (");
  O_CALL (self->formal_arguments, map_args,
	  FunctionDeclaration_generate_formal_arg, &first_formal_arg);
  fprintf (out, ")\n");
  fprintf (out, "{\n");

  if (function_type->has_var_args)
    {
      fprintf (out, "va_list ap;\n");
      fprintf (out, "va_start (ap, ");
      struct ArgumentDeclaration *arg_decl =
	O_CALL (self->formal_arguments, get,
		self->formal_arguments->length - 2);
      O_CALL (arg_decl->name, generate);
      fprintf (out, ");\n");
    }
  O_BRANCH_CALL (self->body, generate);

  if (function_type->has_var_args && 
      (o_is_of (function_type->return_type, PrimitiveType ())
       && ((struct PrimitiveType *) function_type->return_type)->token->type == VOID))
    {
      fprintf (out, "va_end (ap);\n");
    }

  fprintf (out, "}\n\n");
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitInterfaceDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct InterfaceDeclaration *self = O_CAST (_object, InterfaceDeclaration ());

  /* filter the members */
  struct RefList *methods =
    O_CALL (self->members, filter_args, type_filter, FunctionDeclaration ());
  O_CALL (methods, retain);
  struct RefList *new_methods =
    O_CALL (methods, filter_args, Declaration_new_member_filter,
	    FunctionDeclaration ());
  O_CALL (new_methods, retain);

  fprintf (out, "#define O_SUPER Interface ()\n\n");

  O_CALL (methods, map_args,
	  InterfaceDeclaration_generate_method_implementation, self);
  fprintf (out, "\n");

  fprintf (out, "O_OBJECT (");
  O_CALL (self->name, generate);
  fprintf (out, ", Interface);\n");
  fprintf (out, "O_END_OBJECT\n\n");

  fprintf (out, "#undef O_SUPER\n\n");

  O_CALL (methods, release);
  O_CALL (new_methods, release);
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitMacroDeclaration, (void *_self, void *object), (_self, object)) {}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitStructDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct StructDeclaration *self = O_CAST (_object, StructDeclaration ());
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitTypeDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct TypeDeclaration *self = O_CAST(_object, TypeDeclaration());
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitVariableDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct VariableDeclaration *self = O_CAST(_object, VariableDeclaration());

  O_CALL (self->type, generate);
  fprintf (out, " ");
  O_CALL (self->name, generate);
  if (self->expr)
    {
      fprintf (out, " = ");
      O_CALL (self->expr, generate);
    }
  fprintf (out, ";\n");
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitCatchStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitCompoundStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitDeleteStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitDoStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitExpressionStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitForEachStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitForStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitIfStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitReturnStatement, (void *_self, void *object), (_self, object)) {}
// O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitThrowStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitTryStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitWhileStatement, (void *_self, void *object), (_self, object)) {}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitBinaryExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitCastExpression, (void *_self, void *object), (_self, object)) {}
// O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitFunctionCallExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitNestedExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitNewExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitNullExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitSizeExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitSuperExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitTokenExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitUnaryExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitVarArgExpression, (void *_self, void *object), (_self, object)) {}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitArrayType, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitFunctionType, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitObjectType, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitPrimitiveType, (void *_self, void *object), (_self, object)) {}
// O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitType, (void *_self, void *object), (_self, object)) {}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitFile, (void *_self, void *object), (_self, object)) {}


O_OBJECT(GenerateSourceVisitor, BaseCompileObjectVisitor);
O_OBJECT_METHOD(GenerateSourceVisitor, ctor);
O_OBJECT_METHOD(GenerateSourceVisitor, dtor);

// O_OBJECT_METHOD (GenerateSourceVisitor, visit);
O_OBJECT_METHOD (GenerateSourceVisitor, visitArgumentDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitClassDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitConstructorDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitDestructorDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitFunctionDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitInterfaceDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitMacroDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitStructDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitTypeDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitVariableDeclaration);

O_OBJECT_METHOD (GenerateSourceVisitor, visitCatchStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitCompoundStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitDeleteStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitDoStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitExpressionStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitForEachStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitForStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitIfStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitReturnStatement);
// O_OBJECT_METHOD (GenerateSourceVisitor, visitStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitThrowStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitTryStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitWhileStatement);

O_OBJECT_METHOD (GenerateSourceVisitor, visitBinaryExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitCastExpression);
// O_OBJECT_METHOD (GenerateSourceVisitor, visitExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitFunctionCallExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitNestedExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitNewExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitNullExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitSizeExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitSuperExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitTokenExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitUnaryExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitVarArgExpression);

O_OBJECT_METHOD (GenerateSourceVisitor, visitArrayType);
O_OBJECT_METHOD (GenerateSourceVisitor, visitFunctionType);
O_OBJECT_METHOD (GenerateSourceVisitor, visitObjectType);
O_OBJECT_METHOD (GenerateSourceVisitor, visitPrimitiveType);
// O_OBJECT_METHOD (GenerateSourceVisitor, visitType);

O_OBJECT_METHOD (GenerateSourceVisitor, visitFile);

O_OBJECT_IF(CompileObjectVisitor);
O_OBJECT_IF_METHOD(GenerateSourceVisitor, visitClassDeclaration);
O_OBJECT_IF_END
O_END_OBJECT
