#include "io.h"

FILE *out;

struct String *file_path;
struct String *base_dir;

struct String *
analyze_file_name (const char *filename)
{
  struct String *path;
  if (filename == NULL)
    {
      char *directory = getcwd (NULL, 0);
      path = O_CALL_CLASS (String (), new, "%s", directory);
      O_CALL (path, append_str, "/");
      free (directory);
    }
  else if (filename[0] == '/')
    {
      path = O_CALL_CLASS (String (), new, "%s", filename);
    }
  else
    {
      char *directory = getcwd (NULL, 0);
      path = O_CALL_CLASS (String (), new, "%s", directory);
      O_CALL (path, append_str, "/%s", filename);
      free (directory);
    }
  return path;
}

static int
_strrchr (const char *str, char chr)
{
  int pos = -1;
  int i = 0;
  while (str[i] != '\0')
    {
      if (str[i] == chr)
	{
	  pos = i;
	}
      i++;
    }
  return pos;
}

struct String *
determine_base_dir (struct String *path)
{
  int pos = _strrchr (path->data, '/');
  struct String *_base_dir = O_CALL_CLASS (String (), new, "");
  O_CALL (_base_dir, append_str_n, path->data, pos);
  return _base_dir;
}

FILE *
open_input (const char *_filename)
{
  FILE *in;
  if (_filename)
    {
      filename = _filename;
      in = fopen (filename, "r");
    }
  else
    {
      filename = "<stdin>";
      in = stdin;
    }
  return in;
}

FILE *
open_output (const char *filename)
{
  if (filename)
    {
      out = fopen (filename, "w");
    }
  else
    {
      out = stdout;
    }
  fprintf (out, "#include <stdlib.h>\n");
  fprintf (out, "#include <stdio.h>\n");
  fprintf (out, "#include \"Object.h\"\n");
  return out;
}
