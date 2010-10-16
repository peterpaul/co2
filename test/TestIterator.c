#include "List.h"
#include "String.h"
#include "Iterator.h"

void testIterator(struct List *list)
{
  struct Iterator *iterator = O_CALL(list, getIterator);
  while (O_CALL_IF(Iterator,iterator,hasNext))
    {
      struct String * str = O_CALL_IF(Iterator,iterator,getNext);
      O_CALL(str, fprint, stdout);
      O_CALL(iterator, delete);
    }
}

int main(int argc, char **argv)
{
	int i;
	struct List *list = O_CALL_CLASS(List(), new, 4, String());
	for (i = 0; i < 4; i++) {
	  O_CALL(list, append, O_CALL_CLASS(String(), new, "item %d\n", i));
	}

	testIterator(list);

	O_CALL(list, delete);

	o_print_classes(stdout);

	return 0;
}
