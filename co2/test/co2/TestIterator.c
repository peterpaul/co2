/*
 * libco2 Library for object oriented programming in C
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "co2/List.h"
#include "co2/String.h"
#include "co2/ReleasePool.h"
#include "co2/Iterator.h"

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
	O_CALL_CLASS (ReleasePool (), new);
	int i;
	struct List *list = O_CALL_CLASS(List(), new, 4, String());
	for (i = 0; i < 4; i++) {
	  O_CALL(list, append, O_CALL_CLASS(String(), new, "item %d\n", i));
	}

	testIterator(list);

	O_CALL(list, delete);

	o_print_classes(stdout);

	O_CALL (current_release_pool, delete);
	return 0;
}
