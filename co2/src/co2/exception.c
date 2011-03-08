/*
 * libco2 Library for object oriented programming in C
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
throw (int id, const void *data, const char *file, int line, const char *function)
{
  ex_stack->ex_val = id;
  ex_stack->ex_data = data;
  ex_stack->ex_file = file;
  ex_stack->ex_line = line;
  ex_stack->ex_function = function;
  longjmp (ex_stack->ex_buf, id);
}
