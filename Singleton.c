/*
 * Singleton.c
 *
 *  Created on: Oct 23, 2008
 *      Author: epetkle
 */

#include "Singleton.h"

#define O_SUPER Object()

void *Singleton_new(void *_self, ...)
{
	struct SingletonClass *self = o_cast(_self, SingletonClass());
	if (!self->singleton) {
		struct Singleton *object = calloc(1, self->size);
		self->singleton = object;
		va_list ap;
		object->class = self;
		va_start(ap, _self);
		/* Call ctor method */
		O_CALL(object, ctor, &ap);
		va_end(ap);
	}
	return self->singleton;
}

void *Singleton_delete(void *_self)
{
	struct Singleton *self = o_cast(_self, Singleton());
	self->class->singleton = NULL;
	return O_SUPER->delete(self);
}

O_OBJECT(Singleton, Object);
self->new = Singleton_new;
self->delete = Singleton_delete;
O_OBJECT_END
