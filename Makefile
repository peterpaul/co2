LIBSRCS		= Object.c List.c String.c Hash.c utils.c LinkedList.c ListList.c Interface.c Singleton.c Iterator.c ListIterator.c Thread.c RefArray.c RefObject.c ReleasePool.c ReleasePoolItem.c Array.c ArrayList.c exception.c

LIBHDRS		= Object.h List.h String.h Hash.h utils.h LinkedList.h ListList.h Interface.h Singleton.h Iterator.h ListIterator.h Thread.h RefArray.h RefObject.h ReleasePool.h ReleasePoolItem.h Array.h ArrayList.h exception.h

LIBOBJS		= $(LIBSRCS:%.c=%.o)

SUBDIRS		= lexer sudoku web test

BINOBJS		= $(TESTSRCS:%.c=%.o)

SRCS		= $(LIBSRCS) $(BINSRCS)
HDRS		= $(LIBHDRS)
OBJS		= $(LIBOBJS) $(BINOBJS)

LIBS		= libObject.a
BINS		=

FILES		= $(SRCS) $(HDRS) Makefile

MAKEOPTS	= -j3

include Makefile.include

all::		$(LIBS)
		make -C lexer && \
			make ${MAKEOPTS} -C config_parser && \
			make ${MAKEOPTS} -C sudoku && \
			make ${MAKEOPTS} -C web && \
			make ${MAKEOPTS} -C test && \
			make ${MAKEOPTS} -C slime

clean::
		$(RM) -f $(BINS) $(LIBS) $(OBJS) $(DEPENDENCIES)
		make clean -C lexer
		make clean -C config_parser
		make clean -C sudoku
		make clean -C web
		make clean -C test
		make clean -C slime

tar::
		indent $(SRCS)
		rm *~
		tar -czvf Object.tar.gz $(FILES)	
