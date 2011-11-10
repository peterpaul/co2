MAINTAINERCLEANFILES=Makefile.in
ACLOCAL_AMFLAGS=-I m4

AM_YFLAGS=-d

libcarbon_SOURCES=\
		co2/ArgumentDeclaration.c\
		co2/ArrayType.c\
		co2/BaseCompileObjectVisitor.c\
		co2/BreakStatement.c\
		co2/BinaryExpression.c\
		co2/CaseStatement.c\
		co2/CastExpression.c\
		co2/CatchStatement.c\
		co2/ClassDeclaration.c\
		co2/CompileObject.c\
		co2/CompileObjectVisitor.c\
		co2/CompoundStatement.c\
		co2/CompositeScope.c\
		co2/ConditionalBinaryExpression.c\
		co2/ConditionalExpression.c\
		co2/ConstructorDeclaration.c\
		co2/Context.c\
		co2/ContinueStatement.c\
		co2/Declaration.c\
		co2/DeleteStatement.c\
		co2/DestructorDeclaration.c\
		co2/DoStatement.c\
		co2/error.c\
		co2/Expression.c\
		co2/ExpressionStatement.c\
		co2/FinallyStatement.c\
		co2/FixScopeVisitor.c\
		co2/GenerateHeaderIncludesVisitor.c\
		co2/GenerateHeaderVisitor.c\
		co2/GenerateSourceIncludesVisitor.c\
		co2/GenerateSourceVisitor.c\
		co2/File.c\
		co2/ForEachStatement.c\
		co2/ForStatement.c\
		co2/FunctionType.c\
		co2/FunctionDeclaration.c\
		co2/FunctionCallExpression.c\
		co2/Hash.c\
		co2/IfStatement.c\
		co2/InterfaceMethodDefinition.c\
		co2/IncludeStack.c\
		co2/InterfaceDeclaration.c\
		co2/io.c\
		co2/IScope.c\
		co2/IsOfExpression.c\
		co2/NewExpression.c\
		co2/NestedExpression.c\
		co2/NullExpression.c\
		co2/ObjectType.c\
		co2/ObjectTypeDeclaration.c\
		co2/Path.c\
		co2/PrimitiveType.c\
		co2/RefList.c\
		co2/ReturnStatement.c\
		co2/Scope.c\
		co2/SizeExpression.c\
		co2/Statement.c\
		co2/StructDeclaration.c\
		co2/SuperExpression.c\
		co2/SwitchStatement.c\
		co2/ThrowStatement.c\
		co2/Token.c\
		co2/TokenExpression.c\
		co2/TokenGenerator.c\
		co2/TryStatement.c\
		co2/Type.c\
		co2/TypeDeclaration.c\
		co2/UnaryExpression.c\
		co2/VarArgExpression.c\
		co2/VariableDeclaration.c\
		co2/WhileStatement.c

lib_LTLIBRARIES=libcarbon-1.0.la
libcarbon_1_0_ladir=$(includedir)/carbon-1.0/co2
libcarbon_1_0_la_SOURCES= co2/grammar.y co2/lex.l $(libcarbon_SOURCES)
libcarbon_1_0_la_HEADERS=$(libcarbon_SOURCES:.c=.h)

libcarbon_1_0_la_CPPFLAGS=-DO_DEBUG

grammar.h: grammar.c

bin_PROGRAMS=carbon
carbon_SOURCES=co2/Compiler.c co2/Compiler.h
carbon_LDADD=libcarbon-1.0.la
carbon_LDFLAGS=$(CO2_LIBS)

distclean-local:
	-rm -r ${bin_PROGRAMS}

clean-local:
	-rm grammar.c grammar.h lex.c
