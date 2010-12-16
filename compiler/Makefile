INCLUDE		=-I../ -I../lexer/
LIBRARIES	=-L./ -L../ -lCompiler -lObject 

YACC		= bison
LEX		= flex

YACC_SRC	= grammar.y
YACC_GEN_C	= ${YACC_SRC:%.y=%.tab.c}
YACC_GEN_H	= ${YACC_SRC:%.y=%.tab.h}
YACC_FLAGS	= -d -v -k

LEX_SRC		= lex.l
LEX_GEN_C	= ${LEX_SRC:%.l=%.yy.c}
LEX_GEN_H	= ${LEX_SRC:%.l=%.yy.h}
LEX_FLAGS	=--header-file=${LEX_GEN_H}

GEN		= ${YACC_GEN_C} ${YACC_GEN_H} ${LEX_GEN_C} ${LEX_GEN_H}

CC_SRCS		=ArgumentDeclaration.c ArrayType.c BinaryExpression.c ClassDeclaration.c CastExpression.c CompileObject.c Compiler.c \
		CompoundStatement.c ConstructorDeclaration.c Context.c Declaration.c DeleteStatement.c DestructorDeclaration.c \
		DoStatement.c error.c Expression.c \
		ExpressionStatement.c File.c ForEachStatement.c ForStatement.c FunctionType.c FunctionDeclaration.c \
		FunctionCallExpression.c IfStatement.c IncludeStack.c InterfaceDeclaration.c io.c \
		MacroDeclaration.c NewExpression.c NestedExpression.c ObjectType.c ObjectTypeDeclaration.c Path.c PrimitiveType.c RefList.c \
		ReturnStatement.c Scope.c SizeExpression.c Statement.c SuperExpression.c Token.c TokenExpression.c TokenGenerator.c Type.c \
		UnaryExpression.c VarArgExpression.c VariableDeclaration.c WhileStatement.c
LIBSRCS		=$(CC_SRCS) $(YACC_GEN_C) ${LEX_GEN_C}
LIBHDRS		=${LIBSRCS:%.c=%.h}
LIBOBJS		=${LIBSRCS:%.c=%.o}

SRCS		=Compiler.c

OBJS		=${SRCS:%.c=%.o}
BINS		=Compiler.bin
LIBS		=./libCompiler.a ../libObject.a

include ../Makefile.include

%.tab.c %.tab.h: %.y
	${YACC} ${YACC_FLAGS} $<

%.yy.c: %.l grammar.tab.h
	${LEX} ${LEX_FLAGS} $<

../libObject.a:
		make -C ../ libObject.a

# Tests
test:		test/target/OK

test/target/OK:	${BINS}
		scripts/run_tests.sh
