#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <setjmp.h>

struct ex_stack
{
  struct ex_stack *previous;
  jmp_buf ex_buf;
  int ex_val;
  const void *ex_data;
  const char *ex_file;
  int ex_line;
  const char *ex_function;
};

extern struct ex_stack *ex_stack;

void ex_push ();
void ex_pop ();
void throw (int id, const void *data, const char *file, int line, const char *function);

#define try					\
  ex_push ();					\
  if (!setjmp (ex_stack->ex_buf))
#define catch(x)					\
  else if (ex_stack && o_is_of (ex_stack->ex_data, x))
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
#define end_try								\
  else									\
    {									\
      struct ex_stack ex_backup = *ex_stack;				\
      ex_pop ();							\
      throw (ex_backup.ex_val, ex_backup.ex_data, NULL, 0, NULL);	\
    }
#define end_try_finally							\
  else									\
    {									\
      struct ex_stack ex_backup = *ex_stack;				\
      ex_pop ();							\
      do_finally;							\
      throw (ex_backup.ex_val, ex_backup.ex_data, NULL, 0, NULL);	\
    }

#endif /* _EXCEPTION_H_ */
