AM_YFLAGS = -d

lib_LTLIBRARIES = libcarbon-1.0.la
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
libcarbon_1_0_la_CPPFLAGS = -I$(top_srcdir)/include

bin_PROGRAMS = carbon
carbon_SOURCES = co2/Compiler.c
carbon_CPPFLAGS = -I$(top_srcdir)/include
carbon_LDFLAGS = $(CO2_LIBS) -lcarbon-1.0