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
  else if (current_file->absolute_path != NULL)
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
