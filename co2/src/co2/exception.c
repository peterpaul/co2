#include <stdlib.h>
#include "co2/exception.h"

struct ex_stack *ex_stack = NULL;

void
ex_push ()
{
  struct ex_stack *ex_new = calloc (1, sizeof (struct ex_stack));
  ex_new->previous = ex_stack;
  ex_stack = ex_new;
}

void
ex_pop ()
{
  struct ex_stack *ex_old = ex_stack;
  ex_stack = ex_stack->previous;
  free (ex_old);
}

void
throw (int id, void *data)
{
  ex_stack->ex_val = id;
  ex_stack->ex_data = data;
  longjmp (ex_stack->ex_buf, id);
}
