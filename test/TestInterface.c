#include "TestInterface.h"

#define O_SUPER Object()

O_IMPLEMENT(TestInterface, void *, ctor, (void *_self, va_list * argp),
	    (_self, argp))
{
	struct TestInterface *self = o_cast(_self, TestInterface());
	self = O_SUPER->ctor(self, argp);
	self->value = va_arg(*argp, int);
	return self;
}

O_IMPLEMENT_IF(TestInterface, int, getValue, (void *_self), (_self))
{
	struct TestInterface *self = O_CAST(_self, TestInterface());
	return self->value;
}

O_OBJECT(TestInterface, Object);
O_OBJECT_METHOD(TestInterface, ctor);
O_OBJECT_METHOD(TestInterface, getValue);
/* implement MyInterface */
O_OBJECT_IF(MyInterface);
O_OBJECT_IF_METHOD(TestInterface, getValue);
O_OBJECT_IF_END O_END_OBJECT
