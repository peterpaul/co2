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
#ifndef BaseCompileObjectVisitor_H
#define BaseCompileObjectVisitor_H

#include "co2/RefObject.h"
#include "co2/CompileObjectVisitor.h"

O_METHOD_DEF (BaseCompileObjectVisitor, void, visitDeclaration, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitExpression, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitType, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitArgumentDeclaration, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitClassDeclaration, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitConstructorDeclaration, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitDestructorDeclaration, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitFunctionDeclaration, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitInterfaceDeclaration, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitStructDeclaration, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitTypeDeclaration, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitVariableDeclaration, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitBreakStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitCaseStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitCatchStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitCompoundStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitDeleteStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitDoStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitExpressionStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitFinallyStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitForEachStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitForStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitIfStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitReturnStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitSwitchStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitThrowStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitTryStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitWhileStatement, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitBinaryExpression, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitCastExpression, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitConditionalBinaryExpression, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitConditionalExpression, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitFunctionCallExpression, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitIsOfExpression, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitNestedExpression, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitNewExpression, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitNullExpression, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitSizeExpression, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitSuperExpression, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitTokenExpression, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitUnaryExpression, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitVarArgExpression, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitArrayType, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitFunctionType, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitObjectType, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitPrimitiveType, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitFile, (void *_self, void *_object));
O_METHOD_DEF (BaseCompileObjectVisitor, void, visitToken, (void *_self, void *_object));

#define BaseCompileObjectVisitorClass_Attr				\
  RefObjectClass_Attr;							\
  O_METHOD(CompileObjectVisitor, visit);				\
  O_METHOD(BaseCompileObjectVisitor, visitArgumentDeclaration);		\
  O_METHOD(BaseCompileObjectVisitor, visitClassDeclaration);		\
  O_METHOD(BaseCompileObjectVisitor, visitConstructorDeclaration);	\
  O_METHOD(BaseCompileObjectVisitor, visitDeclaration);			\
  O_METHOD(BaseCompileObjectVisitor, visitDestructorDeclaration);	\
  O_METHOD(BaseCompileObjectVisitor, visitFunctionDeclaration);		\
  O_METHOD(BaseCompileObjectVisitor, visitInterfaceDeclaration);	\
  O_METHOD(BaseCompileObjectVisitor, visitStructDeclaration);		\
  O_METHOD(BaseCompileObjectVisitor, visitTypeDeclaration);		\
  O_METHOD(BaseCompileObjectVisitor, visitVariableDeclaration);		\
  O_METHOD(BaseCompileObjectVisitor, visitBreakStatement);		\
  O_METHOD(BaseCompileObjectVisitor, visitCaseStatement);		\
  O_METHOD(BaseCompileObjectVisitor, visitCatchStatement);		\
  O_METHOD(BaseCompileObjectVisitor, visitCompoundStatement);		\
  O_METHOD(BaseCompileObjectVisitor, visitDeleteStatement);		\
  O_METHOD(BaseCompileObjectVisitor, visitDoStatement);			\
  O_METHOD(BaseCompileObjectVisitor, visitExpressionStatement);		\
  O_METHOD(BaseCompileObjectVisitor, visitFinallyStatement);		\
  O_METHOD(BaseCompileObjectVisitor, visitForEachStatement);		\
  O_METHOD(BaseCompileObjectVisitor, visitForStatement);		\
  O_METHOD(BaseCompileObjectVisitor, visitIfStatement);			\
  O_METHOD(BaseCompileObjectVisitor, visitReturnStatement);		\
  O_METHOD(BaseCompileObjectVisitor, visitStatement);			\
  O_METHOD(BaseCompileObjectVisitor, visitSwitchStatement);		\
  O_METHOD(BaseCompileObjectVisitor, visitThrowStatement);		\
  O_METHOD(BaseCompileObjectVisitor, visitTryStatement);		\
  O_METHOD(BaseCompileObjectVisitor, visitWhileStatement);		\
  O_METHOD(BaseCompileObjectVisitor, visitBinaryExpression);		\
  O_METHOD(BaseCompileObjectVisitor, visitCastExpression);		\
  O_METHOD(BaseCompileObjectVisitor, visitConditionalBinaryExpression);	\
  O_METHOD(BaseCompileObjectVisitor, visitConditionalExpression);	\
  O_METHOD(BaseCompileObjectVisitor, visitExpression);			\
  O_METHOD(BaseCompileObjectVisitor, visitFunctionCallExpression);	\
  O_METHOD(BaseCompileObjectVisitor, visitIsOfExpression);		\
  O_METHOD(BaseCompileObjectVisitor, visitNestedExpression);		\
  O_METHOD(BaseCompileObjectVisitor, visitNewExpression);		\
  O_METHOD(BaseCompileObjectVisitor, visitNullExpression);		\
  O_METHOD(BaseCompileObjectVisitor, visitSizeExpression);		\
  O_METHOD(BaseCompileObjectVisitor, visitSuperExpression);		\
  O_METHOD(BaseCompileObjectVisitor, visitTokenExpression);		\
  O_METHOD(BaseCompileObjectVisitor, visitUnaryExpression);		\
  O_METHOD(BaseCompileObjectVisitor, visitVarArgExpression);		\
  O_METHOD(BaseCompileObjectVisitor, visitArrayType);			\
  O_METHOD(BaseCompileObjectVisitor, visitFunctionType);		\
  O_METHOD(BaseCompileObjectVisitor, visitObjectType);			\
  O_METHOD(BaseCompileObjectVisitor, visitPrimitiveType);		\
  O_METHOD(BaseCompileObjectVisitor, visitType);			\
  O_METHOD(BaseCompileObjectVisitor, visitFile);			\
  O_METHOD(BaseCompileObjectVisitor, visitToken)

#define BaseCompileObjectVisitor_Attr		\
  RefObject_Attr

O_CLASS(BaseCompileObjectVisitor, RefObject);

#endif /* BaseCompileObjectVisitor_H */
