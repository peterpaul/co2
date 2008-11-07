#include "TestInterface.h"

#define P_SUPER PObject()

P_IMPLEMENT(TestInterface, int, getValue, (void *_self))
{
	struct TestInterface * self = P_CAST(_self, TestInterface());
	return self->value;
}

P_IMPLEMENT(TestInterface, int, getValue_impl, (void *_self))
{
	struct TestInterface * self = P_CAST(_self, TestInterface());
	struct MyInterface * interface = get_interface(self, TestInterface());
}

P_OBJECT(TestInterface, PObject);
self->getValue = TestInterface_getValue;
P_END_OBJECT
