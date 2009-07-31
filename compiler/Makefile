INCLUDE		=-I../ -I../lexer/
LIBRARIES	=-L./ -L../ -L../lexer/ -lCompiler -lLexer -lObject 

YACC		= bison -d -v
LEX		= flex

YACC_SRC	= grammar.y
YACC_GEN_C	= ${YACC_SRC:%.y=%.tab.c}
YACC_GEN_H	= ${YACC_SRC:%.y=%.tab.h}

LEX_SRC		= lex.l
LEX_GEN_C	= ${LEX_SRC:%.l=%.yy.c}
LEX_GEN_H	=

GEN		= ${YACC_GEN_C} ${YACC_GEN_H} ${LEX_GEN_C} ${LEX_GEN_H}

LIBSRCS		=Compiler.c Token.c
LIBHDRS		=${LIBSRCS:%.c=%.h}
LIBOBJS		=${LIBSRCS:%.c=%.o}

SRCS		=Compiler.c

CC_SRCS		=$(SRCS) $(YACC_GEN_C) $(LEX_GEN_C)

OBJS		=$(CC_SRCS:%.c=%.o)
BINS		=Compiler.bin
LIBS		=../libObject.a ./libCompiler.a ../lexer/libLexer.a

%.tab.c %.tab.h: %.y
	${YACC} $<

%.yy.c: %.l grammar.tab.h
	${LEX} $<

include ../Makefile.include

../libObject.a:
		make -C ../ libObject.a

../lexer/libLexer.a:
		make -C ../lexer libLexer.a
