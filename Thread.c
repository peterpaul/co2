#include "Thread.h"

#define O_SUPER Object()

O_IMPLEMENT(Thread, int, run, (void *_self), (_self))
{
	struct Thread *self = O_CAST(_self, Thread());
	assertTrue(self->class->main,
		   "runtime error: %s at %p doesn't respond to %s.",
		   self->class->name, (void *) self, "main");
	return pthread_create(&self->thread, NULL, self->class->main, self);
}

O_IMPLEMENT(Thread, int, join, (void *_self), (_self))
{
	struct Thread *self = O_CAST(_self, Thread());
	return pthread_join(self->thread, NULL);
}

O_OBJECT(Thread, Object);
O_OBJECT_METHOD(Thread, run);
O_OBJECT_METHOD(Thread, join);
O_END_OBJECT
