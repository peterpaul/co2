#include "Interface.h"

#define O_SUPER Object()

O_IMPLEMENT(Interface, void *, ctor, (void *_self, va_list * app))
{
	struct Interface *self = O_CAST(_self, Interface());
	self = O_SUPER->ctor(self, app);
	self->name = va_arg(*app, char *);
	self->next = o_branch_cast(va_arg(*app, struct Interface *), Interface());
	return self;
}

O_OBJECT(Interface, Object);
O_OBJECT_METHOD(Interface, ctor);
O_END_OBJECT
