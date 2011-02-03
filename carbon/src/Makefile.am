MAINTAINERCLEANFILES = Makefile.in
ACLOCAL_AMFLAGS = -I m4
ACLOCAL_AMFLAGS = -I m4

AM_YFLAGS = -d

lib_LTLIBRARIES = libcarbon-1.0.la
libcarbon_1_0_ladir=$(includedir)/carbon-1.0/co2
libcarbon_1_0_la_SOURCES =  co2/grammar.y co2/lex.l co2/ArgumentDeclaration.c co2/ArrayType.c co2/BaseCompileObjectVisitor.c\
		co2/BinaryExpression.c\
		co2/CastExpression.c co2/CatchStatement.c co2/ClassDeclaration.c\
		co2/CompileObject.c co2/CompileObjectVisitor.c\
		co2/CompoundStatement.c\
		co2/ConstructorDeclaration.c co2/Context.c co2/Declaration.c co2/DeleteStatement.c\
		co2/DestructorDeclaration.c co2/DoStatement.c co2/error.c co2/Expression.c\
		co2/ExpressionStatement.c co2/GenerateHeaderVisitor.c\
		co2/GenerateSourceVisitor.c co2/File.c\
		co2/ForEachStatement.c co2/ForStatement.c\
		co2/FunctionType.c co2/FunctionDeclaration.c co2/FunctionCallExpression.c\
		co2/Hash.c co2/HelloWorldVisitor.c\
		co2/IfStatement.c co2/IncludeStack.c co2/InterfaceDeclaration.c co2/io.c\
		co2/MacroDeclaration.c co2/NewExpression.c co2/NestedExpression.c\
		co2/NullExpression.c co2/ObjectType.c co2/ObjectTypeDeclaration.c co2/Path.c\
		co2/PrimitiveType.c co2/RefList.c co2/ReturnStatement.c co2/Scope.c\
		co2/SizeExpression.c co2/Statement.c co2/StructDeclaration.c co2/SuperExpression.c\
		co2/ThrowStatement.c co2/Token.c co2/TokenExpression.c co2/TokenGenerator.c\
		co2/TryStatement.c co2/Type.c co2/TypeCheckVisitor.c co2/TypeDeclaration.c\
		co2/UnaryExpression.c\
		co2/VarArgExpression.c co2/VariableDeclaration.c co2/WhileStatement.c
libcarbon_1_0_la_HEADERS = co2/ArgumentDeclaration.h co2/ArrayType.h co2/BaseCompileObjectVisitor.h\
		co2/BinaryExpression.h\
		co2/CastExpression.h co2/CatchStatement.h co2/ClassDeclaration.h\
		co2/CompileObject.h co2/CompileObjectVisitor.h co2/Compiler.h\
		co2/CompoundStatement.h\
		co2/ConstructorDeclaration.h co2/Context.h co2/Declaration.h co2/DeleteStatement.h\
		co2/DestructorDeclaration.h co2/DoStatement.h co2/error.h co2/Expression.h\
		co2/ExpressionStatement.h co2/GenerateHeaderVisitor.h\
		co2/GenerateSourceVisitor.h co2/File.h\
		co2/ForEachStatement.h co2/ForStatement.h\
		co2/FunctionType.h co2/FunctionDeclaration.h co2/FunctionCallExpression.h\
		co2/Hash.h co2/HelloWorldVisitor.h\
		co2/IfStatement.h co2/IncludeStack.h co2/InterfaceDeclaration.h co2/io.h\
		co2/MacroDeclaration.h co2/NewExpression.h co2/NestedExpression.h\
		co2/NullExpression.h co2/ObjectType.h co2/ObjectTypeDeclaration.h co2/Path.h\
		co2/PrimitiveType.h co2/RefList.h co2/ReturnStatement.h co2/Scope.h\
		co2/SizeExpression.h co2/Statement.h co2/StructDeclaration.h co2/SuperExpression.h\
		co2/ThrowStatement.h co2/Token.h co2/TokenExpression.h co2/TokenGenerator.h\
		co2/TryStatement.h co2/Type.h co2/TypeCheckVisitor.h co2/TypeDeclaration.h\
		co2/UnaryExpression.h\
		co2/VarArgExpression.h co2/VariableDeclaration.h co2/WhileStatement.h

libcarbon_1_0_la_CPPFLAGS = -I$(top_srcdir)/include

grammar.h: grammar.c

bin_PROGRAMS = carbon
carbon_SOURCES = co2/Compiler.c
carbon_CPPFLAGS = -I$(top_srcdir)/include
carbon_LDFLAGS = $(CO2_LIBS) $(CO2BASE_LIBS) -lcarbon-1.0

distclean-local:
	-rm -r ${bin_PROGRAMS}
