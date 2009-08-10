#include "io.h"

FILE * in;
FILE * out;

FILE * open_input (const char * _filename) {
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

FILE * open_output (const char * filename) {
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
  return out;
}
