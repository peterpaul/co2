#include "ListList.h"

#define O_SUPER LinkedList()

O_IMPLEMENT(ListList, void *, ctor, (void *_self, va_list * app))
{
	struct ListList *self = O_CAST(_self, ListList());
	self = O_SUPER->ctor(self, app);
	self->item = va_arg(*app, struct LinkedList *);
	return self;
}

O_OBJECT(ListList, LinkedList);
O_OBJECT_METHOD(ListList, ctor);
O_END_OBJECT
