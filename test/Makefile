CC		=gcc
DEP_FLAGS	= -MM -MG -MMD
INCLUDE		=-I../
CFLAGS		=-W -Wall -g3 -O0 $(INCLUDE) -DDEBUG=2
LD		=gcc
LIBRARIES	=-L../ -lObject

SRCS		=MyInterface.c MyObject.c MySingleton.c TestInterface.c TestObject.c

CC_SRCS		= $(GEN_SRCS) $(SRCS)

OBJS		=$(CC_SRCS:%.c=%.o)
BINS		=test
LIBS		=../libObject.a

DEPENDENCIES	=$(OBJS:%.o=%.d)

all::		$(BINS)

clean::
		$(RM) -f $(BINS) $(OBJS) $(GEN) $(DEPENDENCIES)

doc::
		doxywizard Doxyfile

$(BINS):	$(OBJS) $(LIBS)
		$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LIBRARIES)

../libObject.a:
		make -C ../ libObject.a

%.o:		%.c
		$(CC) $(CFLAGS) -c $< -o $@

%.d:		%.c
		$(CC) $(CFLAGS) $(DEP_FLAGS) $< > $@

ifeq		($(filter clean tar doc, $(MAKECMDGOALS)),)
-include	$(DEPENDENCIES)
endif
