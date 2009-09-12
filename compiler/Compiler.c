#include "RefList.h"
#include "grammar.tab.h"
#include "RefObject.h"
#include "Declaration.h"
#include "File.h"
#include "io.h"
#include "error.h"

extern int parse (void);
struct File * parsed_file = NULL;

void create_release_pool()
{
  O_CALL_CLASS(ReleasePool(), new);
}

void delete_release_pool()
{
  O_CALL(current_release_pool, delete);
}

int main(int argc, char ** argv)
{
  create_release_pool();

  /* io */
  if (argc >= 2)
    yyin = open_input (argv[1]);
  else
    yyin = open_input (NULL);

  /* syntax analysis */
  parse ();
  if (parsed_file == NULL || errors != 0)
    {
      delete_release_pool();
      return 1;
    }
  /* semantic analysis */
  O_CALL(parsed_file, type_check);
  if (errors != 0)
    {
      delete_release_pool();
      return 1;
    }
  /* optimization */
  O_CALL(parsed_file, optimize);
  if (errors != 0)
    {
      delete_release_pool();
      return 1;
    }
  /* code generation */
  if (argc >= 3) 
    open_output (argv[2]);
  else
    open_output (NULL);
  O_CALL(parsed_file, generate);
  
  delete_release_pool();
  return errors;
}
