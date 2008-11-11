/*
 ============================================================================
 Name        : TestEObject.c
 Author      : Peterpaul Klein Haneveld
 Version     :
 Copyright   : 2008 (c) Copyright P.T. Klein Haneveld
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "MyObject.h"
#include "MySingleton.h"
#include "TestInterface.h"

int test_if(void * _self)
{
	struct MyInterface * IF = o_get_interface(_self, MyInterface());
	return IF->getValue(_self);
}

int main(void)
{
	printf("Hello world"); fflush(stdout);

	struct MySingleton * singleton = MySingleton()->new(MySingleton(), 5);

	printf("."); fflush(stdout);

	printf("%d", singleton->value);

	printf("."); fflush(stdout);
/*
	singleton->class->delete(singleton);
*/
	printf("."); fflush(stdout);
	printf("\n"); fflush(stdout);



	printf("Hello world"); fflush(stdout);

	struct MyObject * object = MyObject()->new(MyObject(), 5);

	printf("."); fflush(stdout);

	printf("%d", object->class->getValue(object));

	printf("."); fflush(stdout);

	object->class->delete(object);

	printf("."); fflush(stdout);
	printf("\n"); fflush(stdout);



	printf("Hello world"); fflush(stdout);

	singleton = MySingleton()->new(MySingleton(), 9);

	printf("."); fflush(stdout);

	printf("%d", singleton->value);

	printf("."); fflush(stdout);

	singleton->class->delete(singleton);

	printf("."); fflush(stdout);
	printf("\n"); fflush(stdout);

	struct TestInterface * IF = TestInterface()->new(TestInterface(), 8);
	printf("test_if returned: %d\n", test_if(IF));

	return (0);
}
