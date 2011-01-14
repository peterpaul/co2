#ifndef BaseCompileObjectVisitor_H
#define BaseCompileObjectVisitor_H

#include "RefObject.h"
#include "CompileObjectVisitor.h"

#define BaseCompileObjectVisitorClass_Attr			\
  RefObjectClass_Attr;						\
  O_METHOD(CompileObjectVisitor, visit);			\
  O_METHOD(CompileObjectVisitor, visitArgumentDeclaration);	\
  O_METHOD(CompileObjectVisitor, visitClassDeclaration);	\
  O_METHOD(CompileObjectVisitor, visitConstructorDeclaration);	\
  O_METHOD(CompileObjectVisitor, visitDeclaration);		\
  O_METHOD(CompileObjectVisitor, visitDestructorDeclaration);	\
  O_METHOD(CompileObjectVisitor, visitFunctionDeclaration);	\
  O_METHOD(CompileObjectVisitor, visitInterfaceDeclaration);	\
  O_METHOD(CompileObjectVisitor, visitMacroDeclaration);	\
  O_METHOD(CompileObjectVisitor, visitStructDeclaration);	\
  O_METHOD(CompileObjectVisitor, visitTypeDeclaration);		\
  O_METHOD(CompileObjectVisitor, visitVariableDeclaration);	\
  O_METHOD(CompileObjectVisitor, visitCatchStatement);		\
  O_METHOD(CompileObjectVisitor, visitCompoundStatement);	\
  O_METHOD(CompileObjectVisitor, visitDeleteStatement);		\
  O_METHOD(CompileObjectVisitor, visitDoStatement);		\
  O_METHOD(CompileObjectVisitor, visitExpressionStatement);	\
  O_METHOD(CompileObjectVisitor, visitForEachStatement);	\
  O_METHOD(CompileObjectVisitor, visitForStatement);		\
  O_METHOD(CompileObjectVisitor, visitIfStatement);		\
  O_METHOD(CompileObjectVisitor, visitReturnStatement);		\
  O_METHOD(CompileObjectVisitor, visitStatement);		\
  O_METHOD(CompileObjectVisitor, visitThrowStatement);		\
  O_METHOD(CompileObjectVisitor, visitTryStatement);		\
  O_METHOD(CompileObjectVisitor, visitWhileStatement);		\
  O_METHOD(CompileObjectVisitor, visitBinaryExpression);	\
  O_METHOD(CompileObjectVisitor, visitCastExpression);		\
  O_METHOD(CompileObjectVisitor, visitExpression);		\
  O_METHOD(CompileObjectVisitor, visitFunctionCallExpression);	\
  O_METHOD(CompileObjectVisitor, visitNestedExpression);	\
  O_METHOD(CompileObjectVisitor, visitNewExpression);		\
  O_METHOD(CompileObjectVisitor, visitNullExpression);		\
  O_METHOD(CompileObjectVisitor, visitSizeExpression);		\
  O_METHOD(CompileObjectVisitor, visitSuperExpression);		\
  O_METHOD(CompileObjectVisitor, visitTokenExpression);		\
  O_METHOD(CompileObjectVisitor, visitUnaryExpression);		\
  O_METHOD(CompileObjectVisitor, visitVarArgExpression);	\
  O_METHOD(CompileObjectVisitor, visitArrayType);		\
  O_METHOD(CompileObjectVisitor, visitFunctionType);		\
  O_METHOD(CompileObjectVisitor, visitObjectType);		\
  O_METHOD(CompileObjectVisitor, visitPrimitiveType);		\
  O_METHOD(CompileObjectVisitor, visitType);		\
  O_METHOD(CompileObjectVisitor, visitFile)

#define BaseCompileObjectVisitor_Attr		\
  RefObject_Attr

O_CLASS(BaseCompileObjectVisitor, RefObject);

#endif /* BaseCompileObjectVisitor_H */
