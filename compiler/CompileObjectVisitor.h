#ifndef CompileObjectVisitor_H
#define CompileObjectVisitor_H

#include "Interface.h"

O_METHOD_DEF(CompileObjectVisitor, void, visit, (void *_self, void *object));

O_METHOD_DEF(CompileObjectVisitor, void, visitArgumentDeclaration, (void *_self, void *decl));
O_METHOD_DEF(CompileObjectVisitor, void, visitClassDeclaration, (void *_self, void *decl));
O_METHOD_DEF(CompileObjectVisitor, void, visitConstructorDeclaration, (void *_self, void *decl));
O_METHOD_DEF(CompileObjectVisitor, void, visitDeclaration, (void *_self, void *decl));
O_METHOD_DEF(CompileObjectVisitor, void, visitDestructorDeclaration, (void *_self, void *decl));
O_METHOD_DEF(CompileObjectVisitor, void, visitFunctionDeclaration, (void *_self, void *decl));
O_METHOD_DEF(CompileObjectVisitor, void, visitInterfaceDeclaration, (void *_self, void *decl));
O_METHOD_DEF(CompileObjectVisitor, void, visitMacroDeclaration, (void *_self, void *decl));
O_METHOD_DEF(CompileObjectVisitor, void, visitStructDeclaration, (void *_self, void *decl));
O_METHOD_DEF(CompileObjectVisitor, void, visitTypeDeclaration, (void *_self, void *decl));
O_METHOD_DEF(CompileObjectVisitor, void, visitVariableDeclaration, (void *_self, void *decl));

O_METHOD_DEF(CompileObjectVisitor, void, visitCatchStatement, (void *_self, void *stat));
O_METHOD_DEF(CompileObjectVisitor, void, visitCompoundStatement, (void *_self, void *stat));
O_METHOD_DEF(CompileObjectVisitor, void, visitDeleteStatement, (void *_self, void *stat));
O_METHOD_DEF(CompileObjectVisitor, void, visitDoStatement, (void *_self, void *stat));
O_METHOD_DEF(CompileObjectVisitor, void, visitExpressionStatement, (void *_self, void *stat));
O_METHOD_DEF(CompileObjectVisitor, void, visitForEachStatement, (void *_self, void *stat));
O_METHOD_DEF(CompileObjectVisitor, void, visitForStatement, (void *_self, void *stat));
O_METHOD_DEF(CompileObjectVisitor, void, visitIfStatement, (void *_self, void *stat));
O_METHOD_DEF(CompileObjectVisitor, void, visitReturnStatement, (void *_self, void *stat));
O_METHOD_DEF(CompileObjectVisitor, void, visitStatement, (void *_self, void *stat));
O_METHOD_DEF(CompileObjectVisitor, void, visitThrowStatement, (void *_self, void *stat));
O_METHOD_DEF(CompileObjectVisitor, void, visitTryStatement, (void *_self, void *stat));
O_METHOD_DEF(CompileObjectVisitor, void, visitWhileStatement, (void *_self, void *stat));

O_METHOD_DEF(CompileObjectVisitor, void, visitBinaryExpression, (void *_self, void *expr));
O_METHOD_DEF(CompileObjectVisitor, void, visitCastExpression, (void *_self, void *expr));
O_METHOD_DEF(CompileObjectVisitor, void, visitExpression, (void *_self, void *expr));
O_METHOD_DEF(CompileObjectVisitor, void, visitFunctionCallExpression, (void *_self, void *expr));
O_METHOD_DEF(CompileObjectVisitor, void, visitNestedExpression, (void *_self, void *expr));
O_METHOD_DEF(CompileObjectVisitor, void, visitNewExpression, (void *_self, void *expr));
O_METHOD_DEF(CompileObjectVisitor, void, visitNullExpression, (void *_self, void *expr));
O_METHOD_DEF(CompileObjectVisitor, void, visitSizeExpression, (void *_self, void *expr));
O_METHOD_DEF(CompileObjectVisitor, void, visitSuperExpression, (void *_self, void *expr));
O_METHOD_DEF(CompileObjectVisitor, void, visitTokenExpression, (void *_self, void *expr));
O_METHOD_DEF(CompileObjectVisitor, void, visitUnaryExpression, (void *_self, void *expr));
O_METHOD_DEF(CompileObjectVisitor, void, visitVarArgExpression, (void *_self, void *expr));

O_METHOD_DEF(CompileObjectVisitor, void, visitArrayType, (void *_self, void *type));
O_METHOD_DEF(CompileObjectVisitor, void, visitFunctionType, (void *_self, void *type));
O_METHOD_DEF(CompileObjectVisitor, void, visitObjectType, (void *_self, void *type));
O_METHOD_DEF(CompileObjectVisitor, void, visitPrimitiveType, (void *_self, void *type));
O_METHOD_DEF(CompileObjectVisitor, void, visitType, (void *_self, void *type));


#define CompileObjectVisitorClass_Attr		\
  InterfaceClass_Attr

#define CompileObjectVisitor_Attr				\
  Interface_Attr;						\
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
  O_METHOD(CompileObjectVisitor, visitType)



O_CLASS(CompileObjectVisitor, Interface);

#endif /* CompileObjectVisitor_H */
