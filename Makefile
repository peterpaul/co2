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

include Makefile.include

all::		$(LIBS)
		make -C lexer && \
			make -C sudoku && \
			make -C web && \
			make -C test

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

libObject.a:	$(LIBOBJS)
		$(AR) rc $@ $(LIBOBJS)
		$(RANLIB) $@
