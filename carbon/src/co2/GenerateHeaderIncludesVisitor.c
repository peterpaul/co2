#include "co2/GenerateHeaderIncludesVisitor.h"
#include "co2/CompileObject.h"
#include "co2/ClassDeclaration.h"
#include "co2/VariableDeclaration.h"
#include "co2/FunctionDeclaration.h"
#include "co2/InterfaceDeclaration.h"
#include "co2/ConstructorDeclaration.h"
#include "co2/DestructorDeclaration.h"
#include "co2/StructDeclaration.h"
#include "co2/TypeDeclaration.h"
#include "co2/FunctionType.h"
#include "co2/ObjectType.h"
#include "co2/io.h"
#include "co2/Hash.h"
#include "co2/Compiler.h"

#define O_SUPER BaseCompileObjectVisitor()

O_IMPLEMENT(GenerateHeaderIncludesVisitor, void *, ctor, (void *_self, va_list *app))
{
  struct GenerateHeaderIncludesVisitor * self = O_CAST(_self, GenerateHeaderIncludesVisitor());
  self = O_SUPER->ctor(self, app);
  self->fp = va_arg (*app, FILE *);
  current_context = O_CALL_CLASS (Context (), new);
  O_BRANCH_CALL (current_context, retain);

  self->map = O_CALL_CLASS (Hash(), new);
  return self;
}

O_IMPLEMENT(GenerateHeaderIncludesVisitor, void *, dtor, (void *_self))
{
  struct GenerateHeaderIncludesVisitor *self = O_CAST(_self, GenerateHeaderIncludesVisitor());
  O_BRANCH_CALL (current_context, release);
  current_context = NULL;
  O_CALL (self->map, delete);
  return O_SUPER->dtor(self);
}

// O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visit, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitArgumentDeclaration, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitClassDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateHeaderIncludesVisitor *visitor = O_CAST(_self, GenerateHeaderIncludesVisitor());
  struct ClassDeclaration *self = O_CAST(_object, ClassDeclaration ());
  
  if (self->superclass) 
    {
      printf ("----------------> %s: %s\n", self->name->name->data, self->superclass->name->data);
      struct Declaration *super = O_CALL (global_scope, lookup, self->superclass);
      if (super->file && super->file != main_file)
	{
	  struct String * filename = O_CALL_CLASS (String (), new, "\"%s.h\"", super->file->name->data);
	  O_CALL (filename, retain);
	  O_CALL (visitor->map, set, filename->data, filename);
	}
    }
  else
    {
      printf ("----------------> %s\n", self->name->name->data);
    }
}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitConstructorDeclaration, (void *_self, void *_object), (_self, _object))
{}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateHeaderIncludesVisitor *visitor = O_CAST (_self, GenerateHeaderIncludesVisitor ());
  struct Declaration *self = O_CAST (_object, Declaration ());
  if (self->include_file)
    {
      O_CALL(visitor->map, set, self->include_file->name->data, self->include_file->name);
      return;
    }
  if (self->scope && self->scope->type != GLOBAL_SCOPE)
    {
      // only generate global declarations
      return;
    }
  O_SUPER->visitDeclaration(visitor, self);
}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitDestructorDeclaration, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitFunctionDeclaration, (void *_self, void *_object), (_self, _object))
{}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitInterfaceDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateHeaderIncludesVisitor *visitor = O_CAST(_self, GenerateHeaderIncludesVisitor());
  struct InterfaceDeclaration *self = O_CAST (_object, InterfaceDeclaration ());
}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitMacroDeclaration, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitStructDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateHeaderIncludesVisitor *visitor = O_CAST(_self, GenerateHeaderIncludesVisitor());
  struct StructDeclaration *self = O_CAST (_object, StructDeclaration ());
}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitTypeDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateHeaderIncludesVisitor *visitor = O_CAST(_self, GenerateHeaderIncludesVisitor());
  struct TypeDeclaration *self = O_CAST(_object, TypeDeclaration());
}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitVariableDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateHeaderIncludesVisitor *visitor = O_CAST(_self, GenerateHeaderIncludesVisitor());
  struct VariableDeclaration *self = O_CAST(_object, VariableDeclaration());
}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitCatchStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitCompoundStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitDeleteStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitDoStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitExpressionStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitForEachStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitForStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitIfStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitReturnStatement, (void *_self, void *_object), (_self, _object)) {}
// O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitThrowStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitTryStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitWhileStatement, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitBinaryExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitCastExpression, (void *_self, void *_object), (_self, _object)) {}
// O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitFunctionCallExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitNestedExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitNewExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitNullExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitSizeExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitSuperExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitTokenExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitUnaryExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitVarArgExpression, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitArrayType, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitFunctionType, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitObjectType, (void *_self, void *_object), (_self, _object))
{
  struct GenerateHeaderIncludesVisitor *visitor = O_CAST(_self, GenerateHeaderIncludesVisitor());
  struct ObjectType *self = O_CAST (_object, ObjectType ());
}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitPrimitiveType, (void *_self, void *_object), (_self, _object)) {}
// O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitType, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitFile, (void *_self, void *_object), (_self, _object))
{
  struct GenerateHeaderIncludesVisitor *visitor = O_CAST(_self, GenerateHeaderIncludesVisitor());
  struct File *self = O_CAST (_object, File ());

  struct Hash *map = visitor->map;
  int i;
  for (i = 0; i < HASH_SIZE; i++)
    {
      struct Tuple *tuple = map->map[i];
      while (tuple)
	{
	  struct String *value = O_CAST (tuple->value, String ());
	  fprintf (out, "#include %s\n", value->data);
	  tuple = tuple->next;
	}
    }
}

O_OBJECT(GenerateHeaderIncludesVisitor, BaseCompileObjectVisitor);
O_OBJECT_METHOD(GenerateHeaderIncludesVisitor, ctor);
O_OBJECT_METHOD(GenerateHeaderIncludesVisitor, dtor);

// O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visit);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitArgumentDeclaration);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitClassDeclaration);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitConstructorDeclaration);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitDeclaration);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitDestructorDeclaration);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitFunctionDeclaration);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitInterfaceDeclaration);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitMacroDeclaration);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitStructDeclaration);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitTypeDeclaration);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitVariableDeclaration);

O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitCatchStatement);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitCompoundStatement);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitDeleteStatement);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitDoStatement);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitExpressionStatement);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitForEachStatement);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitForStatement);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitIfStatement);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitReturnStatement);
// O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitStatement);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitThrowStatement);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitTryStatement);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitWhileStatement);

O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitBinaryExpression);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitCastExpression);
// O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitExpression);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitFunctionCallExpression);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitNestedExpression);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitNewExpression);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitNullExpression);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitSizeExpression);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitSuperExpression);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitTokenExpression);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitUnaryExpression);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitVarArgExpression);

O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitArrayType);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitFunctionType);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitObjectType);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitPrimitiveType);
// O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitType);

O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitFile);

O_OBJECT_IF(CompileObjectVisitor);
// O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visit);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitArgumentDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitClassDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitConstructorDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitDestructorDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitFunctionDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitInterfaceDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitMacroDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitStructDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitTypeDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitVariableDeclaration);

O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitCatchStatement);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitCompoundStatement);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitDeleteStatement);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitDoStatement);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitExpressionStatement);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitForEachStatement);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitForStatement);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitIfStatement);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitReturnStatement);
// O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitStatement);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitThrowStatement);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitTryStatement);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitWhileStatement);

O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitBinaryExpression);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitCastExpression);
// O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitExpression);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitFunctionCallExpression);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitNestedExpression);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitNewExpression);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitNullExpression);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitSizeExpression);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitSuperExpression);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitTokenExpression);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitUnaryExpression);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitVarArgExpression);

O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitArrayType);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitFunctionType);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitObjectType);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitPrimitiveType);
// O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitType);

O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitFile);
O_OBJECT_IF_END
O_END_OBJECT
