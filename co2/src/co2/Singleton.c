/*
 * Singleton.c
 *
 *  Created on: Oct 23, 2008
 *      Author: epetkle
 */

#include "co2/Singleton.h"

#define O_SUPER Object()

O_IMPLEMENT (Singleton, void *, new, (void *_self, ...))
{
	struct SingletonClass *self = o_cast(_self, SingletonClass());
	if (!self->singleton) {
		struct Singleton *object = calloc(1, self->object_size);
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

O_IMPLEMENT (Singleton, void *, delete, (void *_self))
{
	struct Singleton *self = o_cast(_self, Singleton());
	self->class->singleton = NULL;
	return O_SUPER->delete(self);
}

O_IMPLEMENT (Singleton, void *, instance, (void *_self))
{
	struct Singleton *self = o_cast(_self, Singleton());
	return self->class->singleton;
}

O_OBJECT(Singleton, Object);
O_OBJECT_METHOD(Singleton, new);
O_OBJECT_METHOD(Singleton, delete);
O_OBJECT_METHOD(Singleton, instance);
O_OBJECT_END
