#!/bin/bash
set -x

LIBSOURCES="ArgumentDeclaration.c
ArrayType.c
BaseCompileObjectVisitor.c
BinaryExpression.c
BreakStatement.c
CastExpression.c
CatchStatement.c
ClassDeclaration.c
CompileObject.c
CompileObjectVisitor.c
Compiler.c
CompoundStatement.c
ConditionalExpression.c
ConditionalBinaryExpression.c
ConstructorDeclaration.c
Context.c
ContinueStatement.c
Declaration.c
DeleteStatement.c
DestructorDeclaration.c
DoStatement.c
error.c
Expression.c
ExpressionStatement.c
File.c
FinallyStatement.c
ForEachStatement.c
ForStatement.c
FunctionCallExpression.c
FunctionDeclaration.c
FunctionType.c
GenerateHeaderIncludesVisitor.c
GenerateHeaderVisitor.c
GenerateSourceIncludesVisitor.c
GenerateSourceVisitor.c
Hash.c
HelloWorldVisitor.c
IfStatement.c
IncludeStack.c
InterfaceDeclaration.c
io.c
IsOfExpression.c
MacroDeclaration.c
NestedExpression.c
NewExpression.c
NullExpression.c
ObjectType.c
ObjectTypeDeclaration.c
Path.c
PrimitiveType.c
RefList.c
ReturnStatement.c
Scope.c
Set.c
SizeExpression.c
Statement.c
StructDeclaration.c
SuperExpression.c
ThrowStatement.c
Token.c
TokenExpression.c
TokenGenerator.c
TryStatement.c
TypeCheckVisitor.c
Type.c
TypeDeclaration.c
UnaryExpression.c
VarArgExpression.c
VariableDeclaration.c
WhileStatement.c"

if [[ ($# > 0) && ("$1" == "clean") ]]
then
    rm *.[ch]
    rm a.out
    make clean
else
    make -k
    make -k
    rm Compiler.bin
    gcc -O0 -g3 -I/home/peterpaul/local/co2-0.0.2/include/co2-1.0 -L/home/peterpaul/local/co2-0.0.2/lib -lco2-1.0 $LIBSOURCES Compiler.c -o Compiler.bin
fi
