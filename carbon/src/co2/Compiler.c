#include "co2/Compiler.h"
#include "co2/RefList.h"
#include "grammar.h"
#include "co2/RefObject.h"
#include "co2/Declaration.h"
#include "co2/io.h"
#include "co2/error.h"
#include "co2/HelloWorldVisitor.h"
#include "co2/TypeCheckVisitor.h"
#include "co2/GenerateHeaderVisitor.h"
#include "co2/GenerateSourceVisitor.h"

static void File_include_dependencies(void *_self)
{
  struct File * self = O_CAST (_self, File ());
  fprintf (out, "#include \"%s.h\"\n", self->name->data);
}

static char * extract_name(char * file)
{
  char * start = strrchr (file, '/');
  if (start == NULL)
    {
      start = file;
    }
  else
    {
      start += 1;
    }
  char * end = strrchr (file, '.');
  if (end == NULL)
    {
      end = &file[strlen(file)];
    }
  int size = (int) end - (int) start;
  char * base = malloc (size + 1);
  strncpy (base, start, size);
  base[size] = '\0';
  return base;
}

char * get_header_file (char * c_file)
{
  int len = strlen (c_file);
  char * header_file = malloc (len + 1);
  strcpy (header_file, c_file);
  header_file[len -1] = 'h';
  return header_file;
}

extern int parse (void);

struct File *main_file = NULL;
struct File *current_file = NULL;

struct File * try_search_path(struct String *);

void
create_release_pool ()
{
  O_CALL_CLASS (ReleasePool (), new);
}

void
delete_release_pool ()
{
  O_CALL (current_release_pool, delete);
}

int mainImpl(char * output_file)
{
  /* syntax analysis */
  parse ();
  if (main_file == NULL || errors != 0)
    {
      return 1;
    }

  // struct BaseCompileObjectVisitor * visitor = O_CALL_CLASS (HelloWorldVisitor (), new);
  // O_CALL (main_file, accept, visitor);

  /*
  struct TypeCheckVisitor * type_check_visitor = O_CALL_CLASS (TypeCheckVisitor (), new, out);
  O_CALL (main_file, accept, type_check_visitor);
  O_CALL (type_check_visitor, delete);
  */

  /* semantic analysis */
  O_CALL (main_file, type_check);
  if (errors != 0)
    {
      return 1;
    }
  /* optimization */
  O_CALL (main_file, optimize);
  if (errors != 0)
    {
      return 1;
    }

  /* code generation */
  if (output_file)
    {
      char * header_file = get_header_file(output_file);
      open_output (header_file);
      free (header_file);
    }
  else
    open_output (NULL);

  struct GenerateHeaderVisitor * header_visitor = O_CALL_CLASS (GenerateHeaderVisitor (), new, out);
  char * header_name = extract_name (main_file->name->data);
  fprintf (out, "#ifndef %s_H\n", header_name);
  fprintf (out, "#define %s_H\n", header_name);
  fprintf (out, "#include \"Object.h\"\n");
  O_CALL (main_file->file_dependencies, map, File_include_dependencies);
  O_CALL (main_file, accept, header_visitor);
  fprintf (out, "#endif /* %s_H */\n", header_name);
  O_CALL (header_visitor, delete);
  free (header_name);

  if (out != stdout)
    {
      fclose(out);
    }

  /* code generation */
  if (output_file)
    {
      open_output (output_file);
      char * base = extract_name (output_file);
      fprintf (out, "#include \"%s.h\"\n", base);
      free (base);
    }
  else
    open_output (NULL);

  struct GenerateSourceVisitor * source_visitor = O_CALL_CLASS (GenerateSourceVisitor (), new, out);
  O_CALL (main_file, accept, source_visitor);
  O_CALL (source_visitor, delete);

  if (out != stdout)
    {
      fclose(out);
    }

  // O_CALL (main_file, generate);

  return errors;
}

int
main (int argc, char **argv)
{
  create_release_pool ();
  const char *input_file = NULL;
  const char *output_file = NULL;

  /* io */
  if (argc >= 2)
    input_file = argv[1];

  if (argc >= 3)
    output_file = argv[2];

  file_path = O_CALL (analyze_file_name (input_file), retain);
  base_dir = O_CALL (determine_base_dir (file_path), retain);

  path = O_CALL_CLASS (RefList (), new, 8, String ());
  O_CALL (path, append, base_dir);

  main_file = try_search_path (O_CALL_CLASS (String (), new, "%s", input_file));
  current_file = O_CALL (main_file, retain);

  int retval = mainImpl (output_file);

  O_CALL (main_file, release);
  O_CALL (file_path, release);
  O_CALL (base_dir, release);

  delete_release_pool ();
  return retval;
}
