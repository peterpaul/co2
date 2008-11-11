CC		= gcc
AR		= ar
RANLIB		= ranlib
#CC		= llvmgcc
#AR		= llvm-ar
#RANLIB		= llvm-ranlib
DEP_FLAGS	= -MM -MG -MMD
DEBUGFLG	= -g3
#DEBUGFLG	= -g3 -DP_DEBUG
ANSIFLGS	= -ansi -pedantic
#OPTFLAGS	= -O9 -march=i686 -ffast-math -funroll-loops -fomit-frame-pointer
#OPTFLAGS	= -O9 -mfpmath=sse -ffast-math -funroll-loops -fomit-frame-pointer
OPTFLAGS	= -O2
CFLAGS		= -Wall $(OPTFLAGS) $(DEBUGFLG)
LFLAGS		= 

LIBSRCS		= Object.c List.c String.c Hash.c utils.c LinkedList.c ListList.c Interface.c Singleton.c

LIBHDRS		= Object.h List.h string.h Hash.h utils.h LinkedList.h ListList.h Interface.h Singleton.h

LIBOBJS		= $(LIBSRCS:%.c=%.o)

SUBDIRS		= lexer sudoku web test

BINOBJS		= $(TESTSRCS:%.c=%.o)

SRCS		= $(LIBSRCS) $(BINSRCS)
HDRS		= $(LIBHDRS)
OBJS		= $(LIBOBJS) $(BINOBJS)

LIBS		= libObject.a
BINS		=

FILES		= $(SRCS) $(HDRS) Makefile

DEPENDENCIES	=$(SRCS:%.c=%.d)

all::		$(LIBS) 

clean::
		$(RM) -f $(BINS) $(LIBS) $(OBJS) $(DEPENDENCIES)
		make clean -C lexer
		make clean -C sudoku
		make clean -C web
		make clean -C test

tar::
		indent $(SRCS)
		rm *~
		tar -czvf Object.tar.gz $(FILES)	

doc::
		doxygen Doxyfile

libObject.a:	$(LIBOBJS)
		$(AR) rc $@ $(LIBOBJS)
		$(RANLIB) $@

%.o:		%.c
		$(CC) $(CFLAGS) -c $< -o $@

%.d:		%.c
		$(CC) $(CFLAGS) $(DEP_FLAGS) $< > $@

ifeq		($(filter clean tar doc, $(MAKECMDGOALS)),)
-include	$(DEPENDENCIES)
endif
