MAINTAINERCLEANFILES=Makefile.in
ACLOCAL_AMFLAGS=-I m4

AM_YFLAGS=-d

lib_LTLIBRARIES=libcarbon-1.0.la
libcarbon_1_0_ladir=$(includedir)/carbon-1.0

libcarbon_1_0_la_CO2SOURCES=\
ArgumentDeclaration.co2\
ArrayType.co2\
BaseCompileObjectVisitor.co2\
BinaryExpression.co2\
BreakStatement.co2\
CaseStatement.co2\
CastExpression.co2\
CatchStatement.co2\
ClassDeclaration.co2\
CompileObject.co2\
CompileObjectVisitor.co2\
CompoundStatement.co2\
CompositeScope.co2\
ConditionalBinaryExpression.co2\
ConditionalExpression.co2\
ConstructorDeclaration.co2\
Context.co2\
ContinueStatement.co2\
Declaration.co2\
DeleteStatement.co2\
DestructorDeclaration.co2\
DoStatement.co2\
error.co2\
Expression.co2\
ExpressionStatement.co2\
File.co2\
FinallyStatement.co2\
ForEachStatement.co2\
ForStatement.co2\
FunctionCallExpression.co2\
FunctionDeclaration.co2\
FunctionType.co2\
GenerateHeaderIncludesVisitor.co2\
GenerateHeaderVisitor.co2\
GenerateSourceIncludesVisitor.co2\
GenerateSourceVisitor.co2\
Grammar.co2\
Hash.co2\
HelloWorldVisitor.co2\
IfStatement.co2\
InterfaceDeclaration.co2\
InterfaceMethodDefinition.co2\
io.co2\
IScope.co2\
IsOfExpression.co2\
MacroDeclaration.co2\
NestedExpression.co2\
NewExpression.co2\
NullExpression.co2\
ObjectType.co2\
ObjectTypeDeclaration.co2\
Path.co2\
PrimitiveType.co2\
RefList.co2\
ReturnStatement.co2\
Scope.co2\
Set.co2\
SizeExpression.co2\
Statement.co2\
StructDeclaration.co2\
SuperExpression.co2\
SwitchStatement.co2\
ThrowStatement.co2\
Token.co2\
TokenExpression.co2\
TokenGenerator.co2\
TryStatement.co2\
TypeCheckVisitor.co2\
Type.co2\
TypeDeclaration.co2\
UnaryExpression.co2\
VarArgExpression.co2\
VariableDeclaration.co2\
WhileStatement.co2

GENERATED_CSOURCES=\
$(libcarbon_1_0_la_CO2SOURCES:.co2=.c)

GENERATED_CHEADERS=\
IncludeStack.h\
$(libcarbon_1_0_la_CO2SOURCES:.co2=.h)

GENERATED_CO2DEPEND=\
$(libcarbon_1_0_la_CO2SOURCES:.co2=.d)

BUILT_SOURCES=\
$(GENERATED_CSOURCES)\
$(GENERATED_CHEADERS)

libcarbon_1_0_la_SOURCES=\
$(libcarbon_1_0_la_CO2SOURCES)\
grammar.y\
lex.l\
$(GENERATED_CSOURCES)

libcarbon_1_0_la_HEADERS=\
$(libcarbon_1_0_la_CO2SOURCES)\
$(GENERATED_CHEADERS)

libcarbon_1_0_la_CPPFLAGS=-O0 -g3 -DO_DEBUG

CO2_BASE_PATH=$(shell pkg-config co2-base-1.0 --cflags | sed 's/-I/-I /g' | sed 's/[ \t]*$$//g' )

CARBON_FLAGS=$(CO2_BASE_PATH) $(CO2_BASE_PATH)/co2

%.c: %.co2
	if test "$(CARBON)" == "no"; then echo "please install carbon first"; else rm -f $@ | true; $(CARBON) $(CARBON_FLAGS) $< $@; indent $@; fi

%.h: %.c

%.d: %.co2
	-rm -f $@
	set -x
	echo -n "`echo $@ | sed 's/\.d/.c/g'` " > $@
	if test "$(CARBON)" == "no"; then echo ": " $< >> $@; else $(CARBON) --depend $(CARBON_FLAGS) $< $@ >> $@ | rm -f $@; fi

grammar.h: grammar.c

libcarbon_1_0_la_CPPFLAGS=-O0 -g3 -DO_DEBUG

bin_PROGRAMS=carbon
carbon_SOURCES=Compiler.co2 Compiler.c
carbon_LDADD=libcarbon-1.0.la
carbon_LDFLAGS=$(CO2_LIBS) $(CO2BASE_LIBS)

IncludeStack.h: IncludeStack.co2
	-rm $@
	carbon $(CARBON_FLAGS) $< IncludeStack.c

distclean-local:
	-rm -r ${bin_PROGRAMS} $(GENERATED_CO2DEPEND)

clean-local:
	-rm -f grammar.c grammar.h lex.c
	if ! test "$(CARBON)" == "no"; then rm -f $(GENERATED_CHEADERS) $(GENERATED_CSOURCES); fi

include $(GENERATED_CO2DEPEND)
