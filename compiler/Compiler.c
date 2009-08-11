#include "RefList.h"
#include "grammar.tab.h"
#include "RefObject.h"
#include "Declaration.h"
#include "io.h"
#include "error.h"

extern int parse (void);
struct RefList *global_declarations;

void type_check(void *_declaration)
{
}

void optimize(void *_declaration)
{
}

void generate(void *_declaration)
{
  struct Declaration * declaration = O_CAST(_declaration, Declaration());
  O_CALL(declaration, generate);
}

int main(int argc, char ** argv)
{
  O_CALL_CLASS(ReleasePool(), new);

  /* io */
  if (argc >= 2)
    yyin = open_input (argv[1]);
  else
    yyin = open_input (NULL);
  /* syntax analysis */
  parse ();
  if (global_declarations == NULL)
    {
      O_CALL(current_release_pool, delete);
      return 1;
    }
  /* semantic analysis */
  O_CALL (global_declarations, map, type_check);
  if (errors != 0)
    {
      O_CALL(current_release_pool, delete);
      return 1;
    }
  /* optimization */
  O_CALL (global_declarations, map, optimize);
  if (errors != 0)
    {
      O_CALL(current_release_pool, delete);
      return 1;
    }
  /* code generation */
  if (argc >= 3) 
    open_output (argv[2]);
  else
    open_output (NULL);
  O_CALL (global_declarations, map, generate);

  O_CALL(current_release_pool, delete);
}
