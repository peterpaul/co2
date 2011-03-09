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
#include <stdio.h>
#include <stdlib.h>

#include "co2/MyObject.h"
#include "co2/MySingleton.h"
#include "co2/TestInterface.h"

int test_if(void *_self)
{
	struct MyInterface *IF = o_get_interface(_self, MyInterface());
	return IF->getValue(_self);
}

int main(void)
{
	printf("Hello world");
	fflush(stdout);

	struct MySingleton *singleton = O_CALL_CLASS(MySingleton(), new, 5);

	printf(".");
	fflush(stdout);


	printf("%d", singleton->value);

	printf(".");
	fflush(stdout);
/*
	O_CALL(singleton, delete);
*/
	printf(".");
	fflush(stdout);
	printf("\n");
	fflush(stdout);



	printf("Hello world");
	fflush(stdout);

	struct MyObject *object = O_CALL_CLASS(MyObject(), new, 5);

	printf(".");
	fflush(stdout);

	printf("%d", O_CALL(object, getValue));

	printf(".");
	fflush(stdout);

	O_CALL(object, delete);

	printf(".");
	fflush(stdout);
	printf("\n");
	fflush(stdout);



	printf("Hello world");
	fflush(stdout);

	singleton = O_CALL_CLASS(MySingleton(), new, 9);

	printf(".");
	fflush(stdout);

	printf("%d", singleton->value);

	printf(".");
	fflush(stdout);

	O_CALL(singleton, delete);

	printf(".");
	fflush(stdout);
	printf("\n");
	fflush(stdout);

	struct TestInterface *IF = O_CALL_CLASS(TestInterface(), new, 8);
	printf("test_if returned: %d\n", MyInterface_getValue(IF));

	/* o_cleanup_class_hashmap(); */

	return (0);
}
