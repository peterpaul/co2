/*
 * carbon Compiler and programming language for libco2
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "co2/ArgumentDeclaration.h"
#include "co2/ClassDeclaration.h"
#include "co2/ConstructorDeclaration.h"
#include "co2/Declaration.h"
#include "co2/DestructorDeclaration.h"
#include "co2/FunctionDeclaration.h"
#include "co2/InterfaceDeclaration.h"
#include "co2/StructDeclaration.h"
#include "co2/TypeDeclaration.h"
#include "co2/VariableDeclaration.h"

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
#include "co2/ThrowStatement.h"
#include "co2/TryStatement.h"
#include "co2/WhileStatement.h"

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

#include "co2/ArrayType.h"
#include "co2/FunctionType.h"
#include "co2/ObjectType.h"
#include "co2/PrimitiveType.h"
#include "co2/Type.h"

#include "co2/File.h"

#include "co2/BaseCompileObjectVisitor.h"

#define O_SUPER RefObject ()

O_IMPLEMENT (BaseCompileObjectVisitor, void *, ctor, (void *_self, va_list *app))
{
  struct BaseCompileObjectVisitor * self = O_CAST (_self, BaseCompileObjectVisitor ());
  self = O_SUPER->ctor (self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT (BaseCompileObjectVisitor, void *, dtor, (void *_self))
{
  struct BaseCompileObjectVisitor *self = O_CAST (_self, BaseCompileObjectVisitor ());
  /* TODO cleanup */
  return O_SUPER->dtor (self);
}

O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitDeclaration, (void *_self, void *decl), (_self, decl))
{
  struct BaseCompileObjectVisitor *self = O_CAST (_self, BaseCompileObjectVisitor ());
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

O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitStatement, (void *_self, void *stat), (_self, stat))
{
  struct BaseCompileObjectVisitor *self = O_CAST (_self, BaseCompileObjectVisitor ());
  if (o_is_of (stat, CatchStatement ()))
    O_CALL (self, visitCatchStatement, stat);
  else if (o_is_of (stat, CompoundStatement ()))
    O_CALL (self, visitCompoundStatement, stat);
  else if (o_is_of (stat, DeleteStatement ()))
    O_CALL (self, visitDeleteStatement, stat);
  else if (o_is_of (stat, DoStatement ()))
    O_CALL (self, visitDoStatement, stat);
  else if (o_is_of (stat, ExpressionStatement ()))
    O_CALL (self, visitExpressionStatement, stat);
  else if (o_is_of (stat, ForEachStatement ()))
    O_CALL (self, visitForEachStatement, stat);
  else if (o_is_of (stat, ForStatement ()))
    O_CALL (self, visitForStatement, stat);
  else if (o_is_of (stat, IfStatement ()))
    O_CALL (self, visitIfStatement, stat);
  else if (o_is_of (stat, ReturnStatement ()))
    O_CALL (self, visitReturnStatement, stat);
  else if (o_is_of (stat, ThrowStatement ()))
    O_CALL (self, visitThrowStatement, stat);
  else if (o_is_of (stat, TryStatement ()))
    O_CALL (self, visitTryStatement, stat);
  else if (o_is_of (stat, WhileStatement ()))
    O_CALL (self, visitWhileStatement, stat);
  else
    {
    }
}

O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitExpression, (void *_self, void *expr), (_self, expr))
{
  struct BaseCompileObjectVisitor *self = O_CAST (_self, BaseCompileObjectVisitor ());
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

O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitType, (void *_self, void *type), (_self, type))
{
  struct BaseCompileObjectVisitor *self = O_CAST (_self, BaseCompileObjectVisitor ());
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

O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visit, (void *_self, void *object), (_self, object))
{
  struct BaseCompileObjectVisitor *self = O_CAST (_self, BaseCompileObjectVisitor ());
  if (o_is_of (object, Declaration ()))
    O_CALL (self, visitDeclaration, object);
  else if (o_is_of (object, Statement ()))
    O_CALL (self, visitStatement, object);
  else if (o_is_of (object, Expression ()))
    O_CALL (self, visitExpression, object);
  else if (o_is_of (object, Type ()))
    O_CALL (self, visitType, object);
  else if (o_is_of (object, File ()))
    O_CALL (self, visitFile, object);
  else
    {
    }
}

O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitArgumentDeclaration, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitClassDeclaration, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitConstructorDeclaration, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitDestructorDeclaration, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitFunctionDeclaration, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitInterfaceDeclaration, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitStructDeclaration, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitTypeDeclaration, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitVariableDeclaration, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitCatchStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitCompoundStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitDeleteStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitDoStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitExpressionStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitForEachStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitForStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitIfStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitReturnStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitThrowStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitTryStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitWhileStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitBinaryExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitCastExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitFunctionCallExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitNestedExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitNewExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitNullExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitSizeExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitSuperExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitTokenExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitUnaryExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitVarArgExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitArrayType, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitFunctionType, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitObjectType, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitPrimitiveType, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF (BaseCompileObjectVisitor, void, visitFile, (void *_self, void *file), (_self, file)) {}

O_OBJECT (BaseCompileObjectVisitor, RefObject);
O_OBJECT_METHOD (BaseCompileObjectVisitor, ctor);
O_OBJECT_METHOD (BaseCompileObjectVisitor, dtor);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitDeclaration);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitStatement);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitExpression);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitType);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visit);

O_OBJECT_METHOD (BaseCompileObjectVisitor, visitArgumentDeclaration);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitClassDeclaration);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitConstructorDeclaration);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitDestructorDeclaration);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitFunctionDeclaration);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitInterfaceDeclaration);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitStructDeclaration);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitTypeDeclaration);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitVariableDeclaration);

O_OBJECT_METHOD (BaseCompileObjectVisitor, visitCatchStatement);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitCompoundStatement);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitDeleteStatement);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitDoStatement);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitExpressionStatement);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitForEachStatement);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitForStatement);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitIfStatement);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitReturnStatement);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitThrowStatement);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitTryStatement);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitWhileStatement);

O_OBJECT_METHOD (BaseCompileObjectVisitor, visitBinaryExpression);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitCastExpression);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitFunctionCallExpression);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitNestedExpression);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitNewExpression);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitNullExpression);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitSizeExpression);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitSuperExpression);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitTokenExpression);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitUnaryExpression);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitVarArgExpression);

O_OBJECT_METHOD (BaseCompileObjectVisitor, visitArrayType);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitFunctionType);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitObjectType);
O_OBJECT_METHOD (BaseCompileObjectVisitor, visitPrimitiveType);

O_OBJECT_METHOD (BaseCompileObjectVisitor, visitFile);
O_OBJECT_IF (CompileObjectVisitor);
O_OBJECT_IF_METHOD (BaseCompileObjectVisitor, visitDeclaration);
O_OBJECT_IF_METHOD (BaseCompileObjectVisitor, visitStatement);
O_OBJECT_IF_METHOD (BaseCompileObjectVisitor, visitExpression);
O_OBJECT_IF_METHOD (BaseCompileObjectVisitor, visitType);
O_OBJECT_IF_METHOD (BaseCompileObjectVisitor, visit);
O_OBJECT_IF_END
O_END_OBJECT
