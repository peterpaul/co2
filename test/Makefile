INCLUDE		=-I../
LIBRARIES	=-L../ -L./ -ltest -lObject

LIBSRCS		=MyInterface.c MyObject.c MySingleton.c TestInterface.c
LIBHDRS		=${LIBSRCS:%.c=%.h}
LIBOBJS		=${LIBSRCS:%.c=%.o}

SRCS		=TestObject.c TestIterator.c

CC_SRCS		= $(SRCS)

OBJS		=$(CC_SRCS:%.c=%.o)
BINS		=TestObject.bin TestIterator.bin
LIBS		=../libObject.a ./libtest.a

include ../Makefile.include

../libObject.a:
		make -C ../ libObject.a
