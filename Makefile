CC		= gcc
AR		= ar
RANLIB		= ranlib
#CC		= llvmgcc
#AR		= llvm-ar
#RANLIB		= llvm-ranlib
DEP_FLAGS	= -MM -MG -MMD
#DEBUGFLG	= -g3 -DP_DEBUG
DEBUGFLG	= 
ANSIFLGS	= -ansi -pedantic
OPTFLAGS	= -O9 -march=i686 -ffast-math -funroll-loops -fomit-frame-pointer
#OPTFLAGS	= -O9 -mfpmath=sse -ffast-math -funroll-loops -fomit-frame-pointer
#OPTFLAGS	= -O0 -g3
CFLAGS		= -Wall $(OPTFLAGS) $(DEBUGFLG)
LFLAGS		= 

LIBSRCS		= PObject.c PList.c PString.c PHash.c utils.c PLinkedList.c PListList.c

LIBHDRS		= PObject.h PList.h Pstring.h PHash.h utils.h PLinkedList.h PListList.h

LIBOBJS		= $(LIBSRCS:%.c=%.o)

BINOBJS		= $(TESTSRCS:%.c=%.o)

SRCS		= $(LIBSRCS) $(BINSRCS)
HDRS		= $(LIBHDRS)
OBJS		= $(LIBOBJS) $(BINOBJS)

LIBS		= libPObject.a
BINS		=

FILES		= $(SRCS) $(HDRS) Makefile

DEPENDENCIES	=$(SRCS:%.c=%.d)

all::		$(LIBS) 

clean::
		$(RM) -f $(BINS) $(LIBS) $(OBJS) $(DEPENDENCIES)

tar::
		indent $(SRCS)
		rm *~
		tar -czvf PObject.tar.gz $(FILES)	

doc::
		doxygen Doxyfile

libPObject.a:	$(LIBOBJS)
		$(AR) rc $@ $(LIBOBJS)
		$(RANLIB) $@

%.o:		%.c
		$(CC) $(CFLAGS) -c $< -o $@

%.d:		%.c
		$(CC) $(CFLAGS) $(DEP_FLAGS) $< > $@

ifeq		($(filter clean tar doc, $(MAKECMDGOALS)),)
-include	$(DEPENDENCIES)
endif
