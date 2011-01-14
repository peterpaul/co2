#include "Compiler.h"
#include "RefList.h"
#include "grammar.tab.h"
#include "RefObject.h"
#include "Declaration.h"
#include "io.h"
#include "error.h"
#include "HelloWorldVisitor.h"

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

int
main (int argc, char **argv)
{
  create_release_pool ();
  const char *filename;

  /* io */
  if (argc >= 2)
    filename = argv[1];
  else
    filename = NULL;

  file_path = O_CALL (analyze_file_name (filename), retain);
  base_dir = O_CALL (determine_base_dir (file_path), retain);

  path = O_CALL_CLASS (RefList (), new, 8, String ());
  O_CALL (path, append, base_dir);

  main_file = try_search_path (O_CALL_CLASS (String (), new, "%s", filename));
  current_file = O_CALL (main_file, retain);

  /* syntax analysis */
  parse ();
  if (main_file == NULL || errors != 0)
    {
      delete_release_pool ();
      return 1;
    }

  struct CompileObjectVisitor * visitor = O_CALL_CLASS (HelloWorldVisitor (), new);
  O_CALL (main_file, accept, visitor);

  /* semantic analysis */
  O_CALL (main_file, type_check);
  if (errors != 0)
    {
      delete_release_pool ();
      return 1;
    }
  /* optimization */
  O_CALL (main_file, optimize);
  if (errors != 0)
    {
      delete_release_pool ();
      return 1;
    }
  /* code generation */
  if (argc >= 3)
    open_output (argv[2]);
  else
    open_output (NULL);
  O_CALL (main_file, generate);

  O_CALL (main_file, release);

  delete_release_pool ();
  return errors;
}
