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
#include "co2/io.h"
#include <unistd.h>

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
  return out;
}
