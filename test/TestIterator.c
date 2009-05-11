#include "List.h"
#include "String.h"
#include "Iterator.h"

void testIterator(struct List *list)
{
	void *iterator = List_getIterator(list);
	while (Iterator_hasNext(iterator)) {
		String_fprint(Iterator_getNext(iterator), stdout);
	}
	Object_delete(iterator);
}

int main(int argc, char **argv)
{
	int i;
	struct List *list = O_CALL_CLASS(List(), new, 4, String());
	for (i = 0; i < 4; i++) {
		List_append(list, O_CALL_CLASS(String(), new, "item %d\n", i));
	}

	testIterator(list);

	Object_delete(list);

	o_print_classes(stdout);

	return 0;
}
