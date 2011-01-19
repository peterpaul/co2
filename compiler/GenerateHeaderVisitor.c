#include "GenerateHeaderVisitor.h"
#include "CompileObject.h"
#include "ClassDeclaration.h"
#include "VariableDeclaration.h"
#include "FunctionDeclaration.h"
#include "InterfaceDeclaration.h"
#include "ConstructorDeclaration.h"
#include "DestructorDeclaration.h"
#include "StructDeclaration.h"
#include "TypeDeclaration.h"
#include "FunctionType.h"
#include "ObjectType.h"
#include "io.h"

int
new_constructor_filter (void *_constructor);
void
ClassDeclaration_generate_method_definition (void *_method_decl,
					     va_list * app);
void
ClassDeclaration_generate_constructor_definition (void *_constructor_decl,
					     va_list * app);
void
ClassDeclaration_generate_constructor_registration (void *_constructor_decl,
						    va_list * app);
void
ClassDeclaration_generate_method_registration (void *_method_decl,
					       va_list * app);
void
ClassDeclaration_generate_attribute_registration (void *_method_decl,
						  va_list * app);

void
FunctionDeclaration_generate_formal_arg (void *_decl, va_list * ap);

void
InterfaceDeclaration_generate_method_definition (void *_method_decl,
						 va_list * app);
void
InterfaceDeclaration_generate_method_registration (void *_method_decl,
						   va_list * app);

struct FunctionType *
get_type (struct FunctionDeclaration *self);

void Declaration_list_generate (void *_self);

#define O_SUPER BaseCompileObjectVisitor()

O_IMPLEMENT(GenerateHeaderVisitor, void *, ctor, (void *_self, va_list *app))
{
  struct GenerateHeaderVisitor * self = O_CAST(_self, GenerateHeaderVisitor());
  self = O_SUPER->ctor(self, app);
  self->fp = va_arg (*app, FILE *);
  return self;
}

O_IMPLEMENT(GenerateHeaderVisitor, void *, dtor, (void *_self))
{
  struct GenerateHeaderVisitor *self = O_CAST(_self, GenerateHeaderVisitor());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

// O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visit, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitArgumentDeclaration, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitClassDeclaration, (void *_self, void *_object), (_self, _object))
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

  /* generate the class */
  O_CALL (new_constructors, map_args,
	  ClassDeclaration_generate_constructor_definition, self);
  fprintf (out, "\n");

  O_CALL (new_methods, map_args, ClassDeclaration_generate_method_definition,
	  self);
  fprintf (out, "\n");

  fprintf (out, "#define ");
  O_CALL (self->name, generate);
  fprintf (out, "Class_Attr\\\n ");
  generate_superclass (self);
  fprintf (out, "Class_Attr");
  O_CALL (new_constructors, map_args,
	  ClassDeclaration_generate_constructor_registration, self);
  O_CALL (new_methods, map_args,
	  ClassDeclaration_generate_method_registration, self);
  fprintf (out, "\n\n");

  fprintf (out, "#define ");
  O_CALL (self->name, generate);
  fprintf (out, "_Attr\\\n ");
  generate_superclass (self);
  fprintf (out, "_Attr");
  O_CALL (attributes, map_args,
	  ClassDeclaration_generate_attribute_registration, self);
  fprintf (out, "\n\n");

  fprintf (out, "O_CLASS (");
  O_CALL (self->name, generate);
  fprintf (out, ", ");
  generate_superclass (self);
  fprintf (out, ");\n\n");

  O_CALL (attributes, release);
  O_CALL (methods, release);
  O_CALL (new_methods, release);
  O_CALL (constructors, release);
  O_CALL (destructors, release);
}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitConstructorDeclaration, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateHeaderVisitor *visitor = O_CAST (_self, GenerateHeaderVisitor ());
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
      self->defined = true;
      self->declared = true;
    }
}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitDestructorDeclaration, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitFunctionDeclaration, (void *_self, void *_object), (_self, _object))
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
  fprintf (out, ");\n");
}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitInterfaceDeclaration, (void *_self, void *_object), (_self, _object))
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

  fprintf (out, "#include \"Interface.h\"\n");

  /* generate the class */
  O_CALL (new_methods, map_args,
	  InterfaceDeclaration_generate_method_definition, self);
  fprintf (out, "\n");

  fprintf (out, "#define ");
  O_CALL (self->name, generate);
  fprintf (out, "Class_Attr\\\n ");
  fprintf (out, "InterfaceClass_Attr");
  fprintf (out, "\n\n");

  fprintf (out, "#define ");
  O_CALL (self->name, generate);
  fprintf (out, "_Attr\\\n ");
  fprintf (out, "Interface_Attr");
  O_CALL (new_methods, map_args,
	  InterfaceDeclaration_generate_method_registration, self);
  fprintf (out, "\n\n");

  fprintf (out, "O_CLASS (");
  O_CALL (self->name, generate);
  fprintf (out, ", Interface);\n\n");

}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitMacroDeclaration, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitStructDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct StructDeclaration *self = O_CAST (_object, StructDeclaration ());
  fprintf (out, "struct ");
  O_CALL (self->name, generate);
  fprintf (out, " {\n");
  O_CALL (self->members, map, Declaration_list_generate);
  fprintf (out, "};\n\n");

}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitTypeDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct TypeDeclaration *self = O_CAST(_object, TypeDeclaration());
  fprintf (out, "typedef ");
  O_CALL (self->type, generate);
  fprintf (out, " ");
  O_CALL (self->name, generate);
  fprintf (out, ";\n");
}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitVariableDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct VariableDeclaration *self = O_CAST(_object, VariableDeclaration());

  if (!o_is_of (self->type, FunctionType ()))
    {
      fprintf (out, "extern ");
    }
  O_CALL (self->type, generate);
  fprintf (out, " ");
  O_CALL (self->name, generate);
  fprintf (out, ";\n");
}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitCatchStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitCompoundStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitDeleteStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitDoStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitExpressionStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitForEachStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitForStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitIfStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitReturnStatement, (void *_self, void *_object), (_self, _object)) {}
// O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitThrowStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitTryStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitWhileStatement, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitBinaryExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitCastExpression, (void *_self, void *_object), (_self, _object)) {}
// O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitFunctionCallExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitNestedExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitNewExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitNullExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitSizeExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitSuperExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitTokenExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitUnaryExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitVarArgExpression, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitArrayType, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitFunctionType, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitObjectType, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct ObjectType *self = O_CAST (_object, ObjectType ());

  if (self->decl && !self->decl->defined)
    {
      fprintf (out, "struct ");
      O_CALL (self->token, generate);
      fprintf (out, ";\n");
      self->decl->defined = true;
    }
}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitPrimitiveType, (void *_self, void *_object), (_self, _object)) {}
// O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitType, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitFile, (void *_self, void *_object), (_self, _object)) {}


O_OBJECT(GenerateHeaderVisitor, BaseCompileObjectVisitor);
O_OBJECT_METHOD(GenerateHeaderVisitor, ctor);
O_OBJECT_METHOD(GenerateHeaderVisitor, dtor);

// O_OBJECT_METHOD (GenerateHeaderVisitor, visit);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitArgumentDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitClassDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitConstructorDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitDestructorDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitFunctionDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitInterfaceDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitMacroDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitStructDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitTypeDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitVariableDeclaration);

O_OBJECT_METHOD (GenerateHeaderVisitor, visitCatchStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitCompoundStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitDeleteStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitDoStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitExpressionStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitForEachStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitForStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitIfStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitReturnStatement);
// O_OBJECT_METHOD (GenerateHeaderVisitor, visitStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitThrowStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitTryStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitWhileStatement);

O_OBJECT_METHOD (GenerateHeaderVisitor, visitBinaryExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitCastExpression);
// O_OBJECT_METHOD (GenerateHeaderVisitor, visitExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitFunctionCallExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitNestedExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitNewExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitNullExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitSizeExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitSuperExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitTokenExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitUnaryExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitVarArgExpression);

O_OBJECT_METHOD (GenerateHeaderVisitor, visitArrayType);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitFunctionType);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitObjectType);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitPrimitiveType);
// O_OBJECT_METHOD (GenerateHeaderVisitor, visitType);

O_OBJECT_METHOD (GenerateHeaderVisitor, visitFile);

O_OBJECT_IF(CompileObjectVisitor);
O_OBJECT_IF_METHOD(GenerateHeaderVisitor, visitClassDeclaration);
O_OBJECT_IF_END
O_END_OBJECT
