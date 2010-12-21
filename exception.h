#include <setjmp.h>

struct ex_stack
{
  struct ex_stack *previous;
  jmp_buf ex_buf;
  int ex_val;
  void *ex_data;
};

extern struct ex_stack *ex_stack;

void ex_push ();
void ex_pop ();
void throw (int id, void *data);

#define try					\
  ex_push ();					\
  if (!setjmp (ex_stack->ex_buf))
#define catch(x)				\
  else if (ex_stack->ex_val == x)
#define on(x)					\
  case x:
#define finally					\
  else if (ex_stack->ex_val == 0)		\
  ex_finally:
#define do_finally				\
  ex_push ();					\
  if (!setjmp (ex_stack->ex_buf))		\
    goto ex_finally;				\
  else						\
    ex_pop ()
#define end_try						\
  else							\
    {							\
      struct ex_stack ex_backup = *ex_stack;		\
      ex_pop ();					\
      throw (ex_backup.ex_val, ex_backup.ex_data);	\
    }
#define end_try_finally					\
  else							\
    {							\
      struct ex_stack ex_backup = *ex_stack;		\
      ex_pop ();					\
      do_finally;					\
      throw (ex_backup.ex_val, ex_backup.ex_data);	\
    }
