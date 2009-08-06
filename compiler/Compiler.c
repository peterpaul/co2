#include "RefList.h"
#include "grammar.tab.h"
#include "lex.h"
#include "RefObject.h"
#include "io.h"

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
  /* semantic analysis */
  O_CALL (global_declarations, map, type_check);
  /* optimization */
  O_CALL (global_declarations, map, optimize);
  /* code generation */
  if (argc >= 3) 
    open_output (argv[2]);
  else
    open_output (NULL);
  O_CALL (global_declarations, map, generate);

  O_CALL(current_release_pool, delete);
}
