#include "co2/GenerateSourceIncludesVisitor.h"
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
#include "co2/TokenExpression.h"

#define O_SUPER BaseCompileObjectVisitor()

O_IMPLEMENT(GenerateSourceIncludesVisitor, void *, ctor, (void *_self, va_list *app))
{
  struct GenerateSourceIncludesVisitor * self = O_CAST(_self, GenerateSourceIncludesVisitor());
  self = O_SUPER->ctor(self, app);
  self->fp = va_arg (*app, FILE *);
  current_context = O_CALL_CLASS (Context (), new);
  O_BRANCH_CALL (current_context, retain);

  self->map = O_CALL_CLASS (Hash(), new);
  return self;
}

O_IMPLEMENT(GenerateSourceIncludesVisitor, void *, dtor, (void *_self))
{
  struct GenerateSourceIncludesVisitor *self = O_CAST(_self, GenerateSourceIncludesVisitor());
  O_BRANCH_CALL (current_context, release);
  current_context = NULL;
  O_CALL (self->map, delete);
  return O_SUPER->dtor(self);
}

// O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visit, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitArgumentDeclaration, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitClassDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceIncludesVisitor *visitor = O_CAST(_self, GenerateSourceIncludesVisitor());
  struct ClassDeclaration *self = O_CAST(_object, ClassDeclaration ());
}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitConstructorDeclaration, (void *_self, void *_object), (_self, _object))
{}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceIncludesVisitor *visitor = O_CAST (_self, GenerateSourceIncludesVisitor ());
  struct Declaration *self = O_CAST (_object, Declaration ());
  if (self->include_file)
    {
      O_CALL(visitor->map, set, self->include_file->name->data, self->include_file->name);
      return;
    }
  if (self->file && self->file != main_file)
    {
      struct String * filename = O_CALL_CLASS (String (), new, "\"%s.h\"", self->file->name->data);
      O_CALL (filename, retain);
      O_CALL (visitor->map, set, filename->data, filename);
    }
}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitDestructorDeclaration, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitFunctionDeclaration, (void *_self, void *_object), (_self, _object))
{}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitInterfaceDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceIncludesVisitor *visitor = O_CAST(_self, GenerateSourceIncludesVisitor());
  struct InterfaceDeclaration *self = O_CAST (_object, InterfaceDeclaration ());
}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitMacroDeclaration, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitStructDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceIncludesVisitor *visitor = O_CAST(_self, GenerateSourceIncludesVisitor());
  struct StructDeclaration *self = O_CAST (_object, StructDeclaration ());
}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitTypeDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceIncludesVisitor *visitor = O_CAST(_self, GenerateSourceIncludesVisitor());
  struct TypeDeclaration *self = O_CAST(_object, TypeDeclaration());
}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitVariableDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceIncludesVisitor *visitor = O_CAST(_self, GenerateSourceIncludesVisitor());
  struct VariableDeclaration *self = O_CAST(_object, VariableDeclaration());
}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitCatchStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitCompoundStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitDeleteStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitDoStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitExpressionStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitForEachStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitForStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitIfStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitReturnStatement, (void *_self, void *_object), (_self, _object)) {}
// O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitThrowStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitTryStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitWhileStatement, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitBinaryExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitCastExpression, (void *_self, void *_object), (_self, _object)) {}
// O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitFunctionCallExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitNestedExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitNewExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitNullExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitSizeExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitSuperExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitTokenExpression, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitUnaryExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitVarArgExpression, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitArrayType, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitFunctionType, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitObjectType, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceIncludesVisitor *visitor = O_CAST(_self, GenerateSourceIncludesVisitor());
  struct ObjectType *self = O_CAST (_object, ObjectType ());

  if (self->decl && self->decl->file && self->decl->file != main_file)
    {
      struct String * filename = O_CALL_CLASS (String (), new, "\"%s.h\"", self->decl->file->name->data);
      O_CALL (filename, retain);
      O_CALL (visitor->map, set, filename->data, filename);
    }
}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitPrimitiveType, (void *_self, void *_object), (_self, _object)) {}
// O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitType, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitFile, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceIncludesVisitor *visitor = O_CAST(_self, GenerateSourceIncludesVisitor());
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

O_OBJECT(GenerateSourceIncludesVisitor, BaseCompileObjectVisitor);
O_OBJECT_METHOD(GenerateSourceIncludesVisitor, ctor);
O_OBJECT_METHOD(GenerateSourceIncludesVisitor, dtor);

// O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visit);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitArgumentDeclaration);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitClassDeclaration);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitConstructorDeclaration);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitDeclaration);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitDestructorDeclaration);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitFunctionDeclaration);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitInterfaceDeclaration);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitMacroDeclaration);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitStructDeclaration);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitTypeDeclaration);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitVariableDeclaration);

O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitCatchStatement);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitCompoundStatement);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitDeleteStatement);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitDoStatement);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitExpressionStatement);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitForEachStatement);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitForStatement);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitIfStatement);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitReturnStatement);
// O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitStatement);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitThrowStatement);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitTryStatement);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitWhileStatement);

O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitBinaryExpression);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitCastExpression);
// O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitExpression);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitFunctionCallExpression);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitNestedExpression);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitNewExpression);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitNullExpression);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitSizeExpression);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitSuperExpression);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitTokenExpression);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitUnaryExpression);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitVarArgExpression);

O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitArrayType);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitFunctionType);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitObjectType);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitPrimitiveType);
// O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitType);

O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitFile);

O_OBJECT_IF(CompileObjectVisitor);
// O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visit);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitArgumentDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitClassDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitConstructorDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitDestructorDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitFunctionDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitInterfaceDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitMacroDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitStructDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitTypeDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitVariableDeclaration);

O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitCatchStatement);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitCompoundStatement);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitDeleteStatement);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitDoStatement);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitExpressionStatement);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitForEachStatement);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitForStatement);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitIfStatement);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitReturnStatement);
// O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitStatement);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitThrowStatement);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitTryStatement);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitWhileStatement);

O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitBinaryExpression);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitCastExpression);
// O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitExpression);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitFunctionCallExpression);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitNestedExpression);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitNewExpression);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitNullExpression);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitSizeExpression);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitSuperExpression);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitTokenExpression);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitUnaryExpression);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitVarArgExpression);

O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitArrayType);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitFunctionType);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitObjectType);
O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitPrimitiveType);
// O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitType);

O_OBJECT_IF_METHOD (GenerateSourceIncludesVisitor, visitFile);
O_OBJECT_IF_END
O_END_OBJECT
