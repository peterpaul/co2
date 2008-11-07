#include "MyInterface.h"

#define P_SUPER PInterface()

P_IMPLEMENT(MyInterface, int, getValue, (void *_self))
{
	struct MyInterface * self = P_CAST(_self, MyInterface());
//	message();
	return 0;
}

P_OBJECT(MyInterface, PInterface);
self->getValue = MyInterface_getValue;
/*
P_OBJECT_INTERFACE(self, MyInterface());
P_OBJECT_IMPLEMENT
*/
P_END_OBJECT
