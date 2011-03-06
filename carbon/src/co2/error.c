/*
 * carbon Compiler and programming language for libco2
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include "co2/Compiler.h"
#include "co2/error.h"
#include "co2/io.h"

int errors = 0;
int warnings = 0;

static int
write_error_message (const struct Token *token, const char *type,  const char *msg,
		     va_list * app)
{
  int result = 0;
  if (token)
    {
      result += fprintf (stderr, "%s:%d:%s: ", token->file->absolute_path->data, token->line, type);
    }
  else if (current_file && current_file->absolute_path != NULL)
    {
      result += fprintf (stderr, "%s:0:%s: ", current_file->absolute_path->data, type);
    }
  else
    {
      result += fprintf (stderr, "<null>:0:%s: ", type);
    }
  result += vfprintf (stderr, msg, *app);

#ifdef O_ABORT
  char * a = NULL;
  *a = '.';
#endif

  return result;
}

int
error (const struct Token *token, const char *msg, ...)
{
  int result;
  va_list ap;
  errors++;
  va_start (ap, msg);
  result = write_error_message (token, "ERROR", msg, &ap);
  va_end (ap);
  return result;
}

int
warning (const struct Token *token, const char *msg, ...)
{
  int result;
  va_list ap;
  warnings++;
  va_start (ap, msg);
  result = write_error_message (token, "WARNING", msg, &ap);
  va_end (ap);
  return result;
}
