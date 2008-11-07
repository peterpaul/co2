#include "PInterface.h"

#define P_SUPER PObject()

P_IMPLEMENT(PInterface, void *, ctor, (void *_self, va_list * app))
{
	P_DEBUG_IMPLEMENT(PInterface, void *, ctor, (void *_self, va_list * app));
	struct PInterface *self = P_CAST(_self, PInterface());
	self = P_SUPER->ctor(self, app);
	self->name = va_arg(*app, char *);
	return self;
}

P_OBJECT(PInterface,PObject);
self->ctor = PInterface_ctor;
P_END_OBJECT
