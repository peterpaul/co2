
#include "ArgumentDeclaration.h"
#include "ClassDeclaration.h"
#include "ConstructorDeclaration.h"
#include "Declaration.h"
#include "DestructorDeclaration.h"
#include "FunctionDeclaration.h"
#include "InterfaceDeclaration.h"
#include "MacroDeclaration.h"
#include "StructDeclaration.h"
#include "TypeDeclaration.h"
#include "VariableDeclaration.h"

#include "CatchStatement.h"
#include "CompoundStatement.h"
#include "DeleteStatement.h"
#include "DoStatement.h"
#include "ExpressionStatement.h"
#include "ForEachStatement.h"
#include "ForStatement.h"
#include "IfStatement.h"
#include "ReturnStatement.h"
#include "Statement.h"
#include "ThrowStatement.h"
#include "TryStatement.h"
#include "WhileStatement.h"

#include "BinaryExpression.h"
#include "CastExpression.h"
#include "Expression.h"
#include "FunctionCallExpression.h"
#include "NestedExpression.h"
#include "NewExpression.h"
#include "NullExpression.h"
#include "SizeExpression.h"
#include "SuperExpression.h"
#include "TokenExpression.h"
#include "UnaryExpression.h"
#include "VarArgExpression.h"

#include "ArrayType.h"
#include "FunctionType.h"
#include "ObjectType.h"
#include "PrimitiveType.h"
#include "Type.h"

#include "BaseCompileObjectVisitor.h"

#define O_SUPER RefObject()

O_IMPLEMENT(BaseCompileObjectVisitor, void *, ctor, (void *_self, va_list *app))
{
  struct BaseCompileObjectVisitor * self = O_CAST(_self, BaseCompileObjectVisitor());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT(BaseCompileObjectVisitor, void *, dtor, (void *_self))
{
  struct BaseCompileObjectVisitor *self = O_CAST(_self, BaseCompileObjectVisitor());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_IMPLEMENT_IF(BaseCompileObjectVisitor, void, visitDeclaration, (void *_self, void *decl), (_self, decl))
{
  struct BaseCompileObjectVisitor *self = O_CAST(_self, BaseCompileObjectVisitor());
  if (o_is_of (decl, ArgumentDeclaration ()))
    O_CALL (self, visitArgumentDeclaration, decl);
  else if (o_is_of (decl, ClassDeclaration ()))
    O_CALL (self, visitClassDeclaration, decl);
  else if (o_is_of (decl, ConstructorDeclaration ()))
    O_CALL (self, visitConstructorDeclaration, decl);
  else if (o_is_of (decl, DestructorDeclaration ()))
    O_CALL (self, visitDestructorDeclaration, decl);
  else if (o_is_of (decl, FunctionDeclaration ()))
    O_CALL (self, visitFunctionDeclaration, decl);
  else if (o_is_of (decl, InterfaceDeclaration ()))
    O_CALL (self, visitInterfaceDeclaration, decl);
  else if (o_is_of (decl, MacroDeclaration ()))
    O_CALL (self, visitMacroDeclaration, decl);
  else if (o_is_of (decl, StructDeclaration ()))
    O_CALL (self, visitStructDeclaration, decl);
  else if (o_is_of (decl, TypeDeclaration ()))
    O_CALL (self, visitTypeDeclaration, decl);
  else if (o_is_of (decl, VariableDeclaration ()))
    O_CALL (self, visitVariableDeclaration, decl);
  else
    {
    }
}

O_IMPLEMENT_IF(BaseCompileObjectVisitor, void, visitStatement, (void *_self, void *stat), (_self, stat))
{
  struct BaseCompileObjectVisitor *self = O_CAST(_self, BaseCompileObjectVisitor());
  if (o_is_of(stat, CatchStatement ()))
    O_CALL (self, visitCatchStatement, stat);
  else if (o_is_of(stat, CompoundStatement ()))
    O_CALL (self, visitCompoundStatement, stat);
  else if (o_is_of(stat, DeleteStatement ()))
    O_CALL (self, visitDeleteStatement, stat);
  else if (o_is_of(stat, DoStatement ()))
    O_CALL (self, visitDoStatement, stat);
  else if (o_is_of(stat, ExpressionStatement ()))
    O_CALL (self, visitExpressionStatement, stat);
  else if (o_is_of(stat, ForEachStatement ()))
    O_CALL (self, visitForEachStatement, stat);
  else if (o_is_of(stat, ForStatement ()))
    O_CALL (self, visitForStatement, stat);
  else if (o_is_of(stat, IfStatement ()))
    O_CALL (self, visitIfStatement, stat);
  else if (o_is_of(stat, ReturnStatement ()))
    O_CALL (self, visitReturnStatement, stat);
  else if (o_is_of(stat, ThrowStatement ()))
    O_CALL (self, visitThrowStatement, stat);
  else if (o_is_of(stat, TryStatement ()))
    O_CALL (self, visitTryStatement, stat);
  else if (o_is_of(stat, WhileStatement ()))
    O_CALL (self, visitWhileStatement, stat);
  else
    {
    }
}

O_IMPLEMENT_IF(BaseCompileObjectVisitor, void, visitExpression, (void *_self, void *expr), (_self, expr))
{
  struct BaseCompileObjectVisitor *self = O_CAST(_self, BaseCompileObjectVisitor());
  if (o_is_of (expr, BinaryExpression ()))
    O_CALL (self, visitBinaryExpression, expr);
  else if (o_is_of (expr, CastExpression ()))
    O_CALL (self, visitCastExpression, expr);
  else if (o_is_of (expr, FunctionCallExpression ()))
    O_CALL (self, visitFunctionCallExpression, expr);
  else if (o_is_of (expr, NestedExpression ()))
    O_CALL (self, visitNestedExpression, expr);
  else if (o_is_of (expr, NewExpression ()))
    O_CALL (self, visitNewExpression, expr);
  else if (o_is_of (expr, NullExpression ()))
    O_CALL (self, visitNullExpression, expr);
  else if (o_is_of (expr, SizeExpression ()))
    O_CALL (self, visitSizeExpression, expr);
  else if (o_is_of (expr, SuperExpression ()))
    O_CALL (self, visitSuperExpression, expr);
  else if (o_is_of (expr, TokenExpression ()))
    O_CALL (self, visitTokenExpression, expr);
  else if (o_is_of (expr, UnaryExpression ()))
    O_CALL (self, visitUnaryExpression, expr);
  else if (o_is_of (expr, VarArgExpression ()))
    O_CALL (self, visitVarArgExpression, expr);
  else
    {
    }
}

O_IMPLEMENT_IF(BaseCompileObjectVisitor, void, visitType, (void *_self, void *type), (_self, type))
{
  struct BaseCompileObjectVisitor *self = O_CAST(_self, BaseCompileObjectVisitor());
  if (o_is_of (type, ArrayType ()))
    O_CALL (self, visitArrayType, type);
  else if (o_is_of (type, FunctionType ()))
    O_CALL (self, visitFunctionType, type);
  else if (o_is_of (type, ObjectType ()))
    O_CALL (self, visitObjectType, type);
  else if (o_is_of (type, PrimitiveType ()))
    O_CALL (self, visitPrimitiveType, type);
  else
    {
    }
}

O_IMPLEMENT_IF(BaseCompileObjectVisitor, void, visit, (void *_self, void *object), (_self, object))
{
  struct BaseCompileObjectVisitor *self = O_CAST(_self, BaseCompileObjectVisitor());
  if (o_is_of (object, Declaration ()))
    O_CALL (self, visitDeclaration, object);
  else if (o_is_of (object, Statement ()))
    O_CALL (self, visitStatement, object);
  else if (o_is_of (object, Expression ()))
    O_CALL (self, visitExpression, object);
  else if (o_is_of (object, Type ()))
    O_CALL (self, visitType, object);
  else
    {
    }
}

O_OBJECT(BaseCompileObjectVisitor, RefObject);
O_OBJECT_METHOD(BaseCompileObjectVisitor, ctor);
O_OBJECT_METHOD(BaseCompileObjectVisitor, dtor);
O_OBJECT_METHOD(BaseCompileObjectVisitor, visitDeclaration);
O_OBJECT_METHOD(BaseCompileObjectVisitor, visitStatement);
O_OBJECT_METHOD(BaseCompileObjectVisitor, visitExpression);
O_OBJECT_METHOD(BaseCompileObjectVisitor, visitType);
O_OBJECT_METHOD(BaseCompileObjectVisitor, visit);
O_OBJECT_IF(CompileObjectVisitor);
O_OBJECT_IF_METHOD(BaseCompileObjectVisitor, visitDeclaration);
O_OBJECT_IF_METHOD(BaseCompileObjectVisitor, visitStatement);
O_OBJECT_IF_METHOD(BaseCompileObjectVisitor, visitExpression);
O_OBJECT_IF_METHOD(BaseCompileObjectVisitor, visitType);
O_OBJECT_IF_METHOD(BaseCompileObjectVisitor, visit);
O_OBJECT_IF_END
O_END_OBJECT
