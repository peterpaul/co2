/*
 * Object.c
 *
 *  Created on: Oct 22, 2008
 *      Author: epetkle
 */

#include "Object.h"
#include "String.h"
#include "Interface.h"

/* General functions */
void *o_branch_cast(const void *_object, const void *_class)
{
	if (_object != NULL) {
		return o_cast(_object, _class);
	}
	return NULL;
}

void *o_cast(const void *_object, const void *_class)
{
	const struct Object *object = O_IS_OBJECT(_object);
	const struct Class *class = O_IS_CLASS(_class);
	const struct Class *myClass = object->class;
	const struct Class *o = Object();
	while (myClass != class && myClass != o)
		myClass = myClass->super;
	assertTrue(myClass == class, "Could not cast %s to %s.", 
		   object->class->name, class->name);
	return (void *) _object;
}

void *o_alloc(const void *_class)
{
	const struct Class *class = O_IS_CLASS(_class);
	struct Object *self = calloc(1, class->size);
	assertTrue(self, "Calloc failed.");
	self->class = (struct Class *) class;
	return self;
}

int o_is_a(const void *_self, const void *_class)
{
	const struct Object *self = O_IS_OBJECT(_self);
	const struct ObjectClass *class = O_IS_CLASS(_class);
	const struct ObjectClass *myClass = self->class;
	return myClass == class;
}

int o_is_of(const void *_self, const void *_class)
{
	const struct Object *self = O_IS_OBJECT(_self);
	const struct ObjectClass *class = O_IS_CLASS(_class);
	const struct ObjectClass *myClass = self->class;
	void *object = Object();
	while (myClass != class && myClass != object) {
		myClass = myClass->super;
	}
	return myClass == class;
}

int o_implements(void *_self, void *_interface)
{
	struct Object *self = O_IS_OBJECT(_self);
	struct Interface *interface = O_IS_OBJECT(_interface);
	struct Interface *IF = self->class->interface_list;
	while (IF && !o_is_of(IF, interface)) {
		IF = IF->next;
	}
	return IF != NULL;
}

void *o_get_interface(void *_self, void *_interface)
{
	struct Object *self = O_IS_OBJECT(_self);
	struct Interface *interface = O_IS_OBJECT(_interface);
	struct Interface *IF = self->class->interface_list;
	while (IF && !o_is_of(IF, interface)) {
		IF = IF->next;
	}
	assertTrue(IF, "%s at 0x%x does not implement %s.", self->class->name,
		   (int) self, interface->class->name);
	return IF;
}

void *o_super_ctor(void *_self, const void *_class, ...)
{
	struct Object *self = O_IS_OBJECT(_self);
	const struct ObjectClass *class = O_IS_CLASS(_class);
	va_list ap;
	va_start(ap, _class);
	self = class->ctor(self, &ap);
	va_end(ap);
	return self;
}

/* Object methods */
O_IMPLEMENT(Object, void *, ctor, (void *_self, va_list * argp), (_self, argp))
{
	struct Object *self = o_cast(_self, Object());
	return self;
}

O_IMPLEMENT(Object, void *, dtor, (void *_self), (_self))
{
	struct Object *self = o_cast(_self, Object());
	self->class = NULL;
	return self;
}

O_IMPLEMENT_VA(Object, void *, new, (void *_self,...), (_self, &ap), _self)
{
	struct Class *self = o_cast(_self, Class());
	struct Object *object = o_alloc(self);
	assertTrue(object, "Allocating memory failed.");
	va_list ap;
	object->class = self;
	va_start(ap, _self);
	/* Call ctor method */
	O_CALL(object, ctor, &ap);
	va_end(ap);
	return object;
}

O_IMPLEMENT_VA(Object, void *, new_ctor, (void *_self, Object_ctor_t ctor,...),
	       (_self, ctor, &ap), ctor)
{
	struct Class *self = o_cast(_self, Class());
	struct Object *object = o_alloc(self);
	assertTrue(object, "Allocating memory failed.");
	va_list ap;
	object->class = self;
	va_start(ap, ctor);
	/* Call ctor method */
	ctor(object, &ap);
	va_end(ap);
	return object;
}

O_IMPLEMENT(Object, void *, delete, (void *_self), (_self))
{
	struct Object *self = o_cast(_self, Object());
	self = O_CALL(self, dtor);
	free(self);
	return NULL;
}

O_IMPLEMENT_VA(Object, void *, init, (const void *_self, void *_object,...),
	       (_self, _object, &ap), _object)
{
	va_list ap;
	struct Object *object = _object;
	const struct ObjectClass *self = O_IS_CLASS(_self);

	assert(object);
	object->class = (struct Class *) self;
	va_start(ap, _object);
	O_CALL(object, ctor, &ap);
	va_end(ap);
	return object;
}

O_IMPLEMENT_VA(Object, void *, init_ctor,
	       (const void *_self, void *_object, Object_ctor_t ctor,...),
	       (_self, _object, ctor, &ap), ctor)
{
	va_list ap;
	struct Object *object = _object;
	const struct ObjectClass *self = O_IS_CLASS(_self);

	assert(object);
	object->class = (struct Class *) self;
	va_start(ap, ctor);
	ctor(object, &ap);
	va_end(ap);
	return object;
}

O_IMPLEMENT(Object, struct String *, to_string, (void *_self), (_self))
{
	struct Object *self = O_CAST(_self, Object());
	return O_CALL_CLASS(String(), new, "%s at %p", self->class->name,
			    _self);
}

O_IMPLEMENT(Object, void *, clone, (void *_self), (_self))
{
	struct Object *self = O_CAST(_self, Object());
	struct Object *clone = o_alloc(self->class);
	memcpy(clone, self, self->class->size);
	return clone;
}

/* Class methods */
#define O_SUPER Object()

O_IMPLEMENT(Class, void *, ctor, (void *_self, va_list * argp), (_self, argp))
{
	struct Class *self = o_cast(_self, Class());
	self = O_SUPER->ctor(self, argp);
	/* remember class, as memcpy will override it */
	void *class = self->class;
	size_t size = va_arg(*argp, size_t);
	const char *name = va_arg(*argp, char *);
	struct Class *super = o_cast(va_arg(*argp, void *), Class());
	memcpy(self, super, super->class->size);
	self->class = class;
	self->size = size;
	self->name = name;
	self->super = (struct ObjectClass *) super;
	o_add_class(self);
	return self;
}

O_IMPLEMENT(Class, void *, dtor, (void *_self), (_self))
{
	o_cast(_self, Class());
	fprintf(stderr, "--- BAD PROGRAMMER ALERT ---\n");
	fprintf(stderr, "You should never destruct a Class!\n");
	assert(0);
	return NULL;
}

O_IMPLEMENT(Class, void *, delete, (void *_self), (_self))
{
	o_cast(_self, Class());
	fprintf(stderr, "--- BAD PROGRAMMER ALERT ---\n");
	fprintf(stderr, "You should never delete a Class!\n");
	assert(0);
	return NULL;
}

#undef O_SUPER

/* Class accessors */
struct Class *Object()
{
	static struct Class _self;
	static struct Class *self = NULL;
	if (self == NULL) {
		self = &_self;
		_self.class = Class();
		_self.magic = O_MAGIC;
		_self.size = sizeof(struct Object);
		_self.name = "Object";
		_self.super = self;
		_self.interface_list = NULL;
		o_add_class(self);
		O_OBJECT_METHOD(Object, ctor);
		O_OBJECT_METHOD(Object, dtor);
		O_OBJECT_METHOD(Object, new);
		O_OBJECT_METHOD(Object, new_ctor);
		O_OBJECT_METHOD(Object, init);
		O_OBJECT_METHOD(Object, init_ctor);
		O_OBJECT_METHOD(Object, delete);
		O_OBJECT_METHOD(Object, to_string);
		O_OBJECT_METHOD(Object, clone);
	}
	return self;
}

struct Class *Class()
{
	static struct Class _self;
	static struct Class *self = NULL;
	if (self == NULL) {
		self = &_self;
		_self.class = self;
		_self.magic = O_MAGIC;
		_self.size = sizeof(struct Class);
		_self.name = "Class";
		_self.super = Object();
		_self.interface_list = NULL;
		o_add_class(self);
		O_OBJECT_METHOD(Class, ctor);
		O_OBJECT_METHOD(Class, dtor);
		O_OBJECT_METHOD(Object, new);
		O_OBJECT_METHOD(Object, new_ctor);
		O_OBJECT_METHOD(Object, init);
		O_OBJECT_METHOD(Object, init_ctor);
		O_OBJECT_METHOD(Class, delete);
		O_OBJECT_METHOD(Object, to_string);
		/* O_OBJECT_METHOD(Object,clone); */
	}
	return self;
}

/* *** Struct for class hashmap *** */
struct ClassHashmapTuple {
	struct ClassHashmapTuple *next;
	struct ObjectClass *class;
};

static struct ClassHashmapTuple *o_new_class_hashmap_tuple(struct
							   ClassHashmapTuple
							   *next, struct
							   ObjectClass
							   *class)
{
	assertTrue(class != NULL, "parameter class may not be NULL");

	struct ClassHashmapTuple *tuple =
	    calloc(1, sizeof(struct ClassHashmapTuple));
	assertTrue(tuple != NULL,
		   "failed allocating memory for struct ClassHashmapTuple");

	tuple->next = next;
	tuple->class = class;
	return tuple;
}

static struct ClassHashmapTuple *o_find_class_hashmap_tuple(struct
							    ClassHashmapTuple
							    *head, const char
							    *class_name)
{
	while (head != NULL && strcmp(class_name, head->class->name) != 0) {
		head = head->next;
	}
	return head;
}

void o_cleanup_class_hashmap_tuple(struct ClassHashmapTuple *tuple)
{
	while (tuple->next) {
		o_cleanup_class_hashmap_tuple(tuple->next);
	}
	free(tuple);
}

/* *** Functions for class registration *** */
#define CLASS_HASHMAP_SIZE 1024

struct ClassHashmapTuple *class_hashmap[CLASS_HASHMAP_SIZE];

void o_add_class(void *_class)
{
	struct ObjectClass *class = O_IS_CLASS(_class);

	unsigned long index =
	    hash_function((unsigned char *) class->name) % CLASS_HASHMAP_SIZE;

	class_hashmap[index] =
	    o_new_class_hashmap_tuple(class_hashmap[index], class);
}

void *o_get_class(const char *class_name)
{
	if (!class_hashmap) {
		return NULL;
	}

	unsigned long index =
	    hash_function((unsigned char *) class_name) % CLASS_HASHMAP_SIZE;

	struct ClassHashmapTuple *tuple =
	    o_find_class_hashmap_tuple(class_hashmap[index], class_name);

	return tuple->class;;
}

void o_print_classes(FILE * fp)
{
	int i;

	for (i = 0; i < CLASS_HASHMAP_SIZE; i++) {
		if (class_hashmap[i]) {
			fprintf(fp, "%s\n", class_hashmap[i]->class->name);
		}
	}
}

void o_cleanup_class_hashmap()
{
	int i;
	for (i = 0; i < CLASS_HASHMAP_SIZE; i++) {
		if (class_hashmap[i]) {
			o_cleanup_class_hashmap_tuple(class_hashmap[i]);
		}
	}
}
