/*
 * MySingleton.c
 *
 *  Created on: Oct 20, 2008
 *      Author: epetkle
 */

#include "MySingleton.h"

#define O_SUPER Singleton()

void *MySingleton_ctor(void *_self, va_list * argp)
{
	struct MySingleton *self = o_cast(_self, MySingleton());
	self = O_SUPER->ctor(self, argp);
	self->value = va_arg(*argp, int);
	return self;
}

O_OBJECT(MySingleton, Singleton);
self->ctor = MySingleton_ctor;
O_OBJECT_END
