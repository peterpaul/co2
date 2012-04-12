MAINTAINERCLEANFILES=Makefile.in
ACLOCAL_AMFLAGS=-I m4

AM_YFLAGS=-d

lib_LTLIBRARIES=libcarbon-1.0.la
libcarbon_1_0_ladir=$(includedir)/carbon-1.0/co2

libcarbon_1_0_la_CO2SOURCES=\
co2/ArgumentDeclaration.co2\
co2/ArrayType.co2\
co2/BaseCompileObjectVisitor.co2\
co2/BinaryExpression.co2\
co2/BreakStatement.co2\
co2/CaseStatement.co2\
co2/CastExpression.co2\
co2/CatchStatement.co2\
co2/ClassDeclaration.co2\
co2/CompileObject.co2\
co2/CompileObjectVisitor.co2\
co2/CompoundStatement.co2\
co2/CompositeScope.co2\
co2/ConditionalBinaryExpression.co2\
co2/ConditionalExpression.co2\
co2/ConstructorDeclaration.co2\
co2/Context.co2\
co2/ContinueStatement.co2\
co2/Declaration.co2\
co2/DeleteStatement.co2\
co2/DestructorDeclaration.co2\
co2/DoStatement.co2\
co2/Error.co2\
co2/Expression.co2\
co2/ExpressionStatement.co2\
co2/File.co2\
co2/FinallyStatement.co2\
co2/ForEachStatement.co2\
co2/ForStatement.co2\
co2/FunctionCallExpression.co2\
co2/FunctionDeclaration.co2\
co2/FunctionType.co2\
co2/Grammar.co2\
co2/Hash.co2\
co2/HelloWorldVisitor.co2\
co2/IfStatement.co2\
co2/InterfaceDeclaration.co2\
co2/InterfaceMethodDefinition.co2\
co2/io.co2\
co2/IScope.co2\
co2/IsOfExpression.co2\
co2/MacroDeclaration.co2\
co2/NestedExpression.co2\
co2/NewExpression.co2\
co2/NullExpression.co2\
co2/ObjectType.co2\
co2/ObjectTypeDeclaration.co2\
co2/Path.co2\
co2/PrimitiveType.co2\
co2/RefList.co2\
co2/ReturnStatement.co2\
co2/Scope.co2\
co2/SizeExpression.co2\
co2/Statement.co2\
co2/StructDeclaration.co2\
co2/SuperExpression.co2\
co2/SwitchStatement.co2\
co2/ThrowStatement.co2\
co2/Token.co2\
co2/TokenExpression.co2\
co2/TokenGenerator.co2\
co2/TryStatement.co2\
co2/TypeCheckVisitor.co2\
co2/Type.co2\
co2/TypeDeclaration.co2\
co2/UnaryExpression.co2\
co2/VarArgExpression.co2\
co2/VariableDeclaration.co2\
co2/WhileStatement.co2

GENERATED_CSOURCES=\
$(libcarbon_1_0_la_CO2SOURCES:.co2=.c)

GENERATED_CHEADERS=\
co2/IncludeStack.h\
$(libcarbon_1_0_la_CO2SOURCES:.co2=.h)

GENERATED_CO2DEPEND=\
$(libcarbon_1_0_la_CO2SOURCES:.co2=.d)

BUILT_SOURCES=\
$(GENERATED_CSOURCES)\
$(GENERATED_CHEADERS)

libcarbon_1_0_la_SOURCES=\
$(libcarbon_1_0_la_CO2SOURCES)\
co2/grammar.y\
co2/lex.l\
$(GENERATED_CSOURCES)

libcarbon_1_0_la_HEADERS=\
$(libcarbon_1_0_la_CO2SOURCES)\
$(GENERATED_CHEADERS)

libcarbon_1_0_la_CPPFLAGS=-O0 -g3 -DO_DEBUG

CO2_BASE_PATH=$(shell pkg-config co2-base-1.0 --cflags | sed 's/-I/-I /g' | sed 's/[ \t]*$$//g' )

CARBON_FLAGS=$(CO2_BASE_PATH) $(CO2_BASE_PATH)/co2 -I .

%.c: %.co2
	if test "$(CARBON)" == "no"; then echo "please install carbon first"; else rm -f $@ | true; $(CARBON) $(CARBON_FLAGS) $< $@ && indent $@; fi

%.h: %.c

%.d: %.co2
	-rm -f $@
	echo -n "`echo $@ | sed 's/\.d/.c/g'` " > $@
	if test "$(CARBON)" == "no"; then echo ": " $< >> $@; else $(CARBON) --depend $(CARBON_FLAGS) $< $@ >> $@; fi

grammar.h: grammar.c

libcarbon_1_0_la_CPPFLAGS=-O0 -g3 -DO_DEBUG

bin_PROGRAMS=carbon
carbon_CO2SOURCES=\
co2/Compiler.co2\
co2/GenerateHeaderIncludesVisitor.co2\
co2/GenerateHeaderVisitor.co2\
co2/GenerateSourceIncludesVisitor.co2\
co2/GenerateSourceVisitor.co2
carbon_SOURCES=$(carbon_CO2SOURCES) $(carbon_CO2SOURCES:.co2=.c)
carbon_LDADD=libcarbon-1.0.la
carbon_LDFLAGS=$(CO2_LIBS) $(CO2BASE_LIBS)

co2/IncludeStack.h: co2/IncludeStack.co2
	-rm $@
	carbon $(CARBON_FLAGS) $< co2/IncludeStack.c

clean-local:
	-rm -f grammar.c grammar.h lex.c
	if ! test "$(CARBON)" == "no"; then rm -f $(GENERATED_CHEADERS) $(GENERATED_CSOURCES); fi

distclean-local:
	-rm -r $(bin_PROGRAMS) $(GENERATED_CO2DEPEND)


include $(GENERATED_CO2DEPEND)
