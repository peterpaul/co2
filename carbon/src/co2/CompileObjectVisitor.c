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
#include "co2/CompileObjectVisitor.h"

#define O_SUPER Interface()

//O_METHOD_IF(CompileObjectVisitor, void *, getNext, (void *_self), (_self));
//O_METHOD_IF(CompileObjectVisitor, bool, hasNext, (void *_self), (_self));

O_METHOD_IF(CompileObjectVisitor, void, visit, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitArgumentDeclaration, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitClassDeclaration, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitConstructorDeclaration, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitDeclaration, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitDestructorDeclaration, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitFunctionDeclaration, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitInterfaceDeclaration, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitMacroDeclaration, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitStructDeclaration, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitTypeDeclaration, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitVariableDeclaration, (void *_self, void *object), (_self, object));

O_METHOD_IF(CompileObjectVisitor, void, visitCatchStatement, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitCompoundStatement, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitDeleteStatement, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitDoStatement, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitExpressionStatement, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitForEachStatement, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitForStatement, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitIfStatement, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitReturnStatement, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitStatement, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitThrowStatement, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitTryStatement, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitWhileStatement, (void *_self, void *object), (_self, object));

O_METHOD_IF(CompileObjectVisitor, void, visitBinaryExpression, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitCastExpression, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitExpression, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitFunctionCallExpression, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitNestedExpression, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitNewExpression, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitNullExpression, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitSizeExpression, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitSuperExpression, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitTokenExpression, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitUnaryExpression, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitVarArgExpression, (void *_self, void *object), (_self, object));

O_METHOD_IF(CompileObjectVisitor, void, visitArrayType, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitFunctionType, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitObjectType, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitPrimitiveType, (void *_self, void *object), (_self, object));
O_METHOD_IF(CompileObjectVisitor, void, visitType, (void *_self, void *object), (_self, object));

O_METHOD_IF(CompileObjectVisitor, void, visitFile, (void *_self, void *object), (_self, object));

O_OBJECT(CompileObjectVisitor, Interface);
O_END_OBJECT
