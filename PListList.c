#include "PListList.h"

#define P_SUPER PLinkedList()

P_IMPLEMENT(PListList, void *, ctor, (void *_self, va_list * app))
{
	struct PListList *self = P_CAST(_self, PListList());
	self = P_SUPER->ctor(self, app);
	self->item = va_arg(*app, struct PLinkedList *);
	return self;
}

P_OBJECT(PListList, PLinkedList);
self->ctor = PListList_ctor;
P_END_OBJECT
