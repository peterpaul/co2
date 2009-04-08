#include "List.h"
#include "String.h"
#include "ListIterator.h"

#define O_SUPER Object()

/**
 * This implements a list where the entries may be empty.
 * Also, a type (class) may be provided, so that entries
 * entered into the list can be cast to this type.
 */

O_IMPLEMENT(List, void *, ctor, (void *_self, va_list * app), (_self, app))
{
	struct List *self = O_CAST(_self, List());
	self = O_SUPER->ctor(self, app);
	self->max = va_arg(*app, unsigned);
	self->type = va_arg(*app, struct ObjectClass *);
	self->data = calloc(self->max, sizeof(void *));
	self->length = 0;
	return self;
}

O_IMPLEMENT(List, void *, resize, (void *_self, unsigned size),
	    (_self, size))
{
	struct List *self = O_CAST(_self, List());
	assert(size >= self->length);
	self->data = realloc(self->data, sizeof(void *[size]));
	self->max = size;
	return self;
}

O_IMPLEMENT(List, void *, append, (void *_self, void *item), (_self, item))
{
	struct List *self = O_CAST(_self, List());
	/* if item and type, cast item to type */
	if (item && self->type)
		o_cast(item, self->type);
	/* check if need to be resized */
	if (self->length == self->max)
		self = self->class->resize(self, self->max << 1);
	/* append item */
	self->data[self->length] = item;
	self->length++;
	return item;
}

static void List_append_item(void *_item, va_list * ap)
{
	struct List *list = o_cast(va_arg(*ap, struct List *), List());
	list->class->append(list, _item);
}

O_IMPLEMENT(List, void *, append_list, (void *_self, void *_list),
	    (_self, _list))
{
	struct List *self = O_CAST(_self, List());
	struct List *list = o_cast(_list, List());
	list->class->map_args(list, List_append_item, self);
	return self;
}

O_IMPLEMENT(List, void *, merge, (void *_self, void *_other),
	    (_self, _other))
{
	struct List *self = O_CAST(_self, List());
	struct List *other = O_CAST(_other, List());
	self->class->append_list(self, other);
	/* delete other list */
	other->length = 0;
	other->class->delete(other);
	/* return result */
	return self;
}

O_IMPLEMENT(List, void *, remove, (void *_self), (_self))
{
	struct List *self = O_CAST(_self, List());
	self->length--;
	return self->data[self->length];
}

O_IMPLEMENT(List, void *, get, (void *_self, unsigned index),
	    (_self, index))
{
	struct List *self = O_CAST(_self, List());
	assert(index < self->length);
	return self->data[index];
}

O_IMPLEMENT(List, void *, set, (void *_self, unsigned index, void *item),
	    (_self, index, item))
{
	struct List *self = O_CAST(_self, List());
	assert(index < self->length && item);
	return self->data[index] =
	    self->type ? o_cast(item, self->type) : item;
}

O_IMPLEMENT(List, void *, map, (void *_self, void (*fun) (void *)),
	    (_self, fun))
{
	struct List *self = O_CAST(_self, List());
	int i;
	const unsigned length = self->length;
	for (i = 0; i < length; i++) {
		fun(self->data[i]);
	}
	return self;
}

O_IMPLEMENT(List, void *, map_args,
	    (void *_self, void (*fun) (void *, va_list *),...), (_self,
								 fun))
{
	struct List *self = O_CAST(_self, List());
	int i;
	va_list ap;
	va_start(ap, fun);
	const unsigned length = self->length;
	for (i = 0; i < length; i++) {
		va_list aq;
		va_copy(aq, ap);
		fun(self->data[i], &aq);
		va_end(aq);
	}
	va_end(ap);
	return self;
}

O_IMPLEMENT(List, void *, filter, (void *_self, int (*filter) (void *)),
	    (_self, filter))
{
	struct List *self = O_CAST(_self, List());
	int i;
	const unsigned length = self->length;
	struct List *result = List()->new(List(), self->length / 2);
	for (i = 0; i < length; i++) {
		if (filter(self->data[i]))
			result->class->append(result, self->data[i]);
	}
	return self;
}

O_IMPLEMENT(List, void *, filter_args,
	    (void *_self, int (*filter) (void *, va_list *),...), (_self,
								   filter))
{
	struct List *self = O_CAST(_self, List());
	int i;
	va_list ap;
	va_start(ap, filter);
	const unsigned length = self->length;
	struct List *result = List()->new(List(), self->length / 2);
	for (i = 0; i < length; i++) {
		va_list aq;
		va_copy(aq, ap);
		if (filter(self->data[i], &aq))
			result->class->append(result, self->data[i]);
		va_end(aq);
	}
	va_end(ap);
	return self;
}

/* hook for deletion of members */
static void List_delete_items(void *_item)
{
	struct Object *item = o_cast(_item, Object());
	item->class->delete(item);
}

O_IMPLEMENT(List, void *, dtor, (void *_self), (_self))
{
	struct List *self = O_CAST(_self, List());
	self->class->map(self, List_delete_items);
	free(self->data);
	self->data = NULL;
	return O_SUPER->dtor(self);
}

O_IMPLEMENT(List, struct String *, toString, (void *_self), (_self))
{
	struct List *self = O_CAST(_self, List());
	struct String *str = O_SUPER->toString(self);

	struct String *enter = String()->new(String(), "\n");
	struct String *item_enter = String()->new(String(), "\n\t");

	int i;
	const unsigned length = self->length;
	str->class->append_str(str, " [\n");
	for (i = 0; i < length; i++) {
		struct Object *item = self->class->get(self, i);
		struct String *item_str = item->class->toString(item);
		item_str->class->replace(item_str, enter, item_enter);
		str->class->append_str(str, "\t");
		str->class->append(str, item_str);
		item_str->class->delete(item_str);
		str->class->append_str(str, ";\n");
	}
	str->class->append_str(str, "]");

	enter->class->delete(enter);
	item_enter->class->delete(item_enter);

	return str;
}

O_IMPLEMENT(List, void *, getIterator, (void *_self), (_self))
{
	struct List *self = O_CAST(_self, List());
	return ListIterator()->new(ListIterator(), self);
}

O_OBJECT(List, Object);
O_OBJECT_METHOD(List, ctor);
O_OBJECT_METHOD(List, dtor);
O_OBJECT_METHOD(List, resize);
O_OBJECT_METHOD(List, append);
O_OBJECT_METHOD(List, append_list);
O_OBJECT_METHOD(List, merge);
O_OBJECT_METHOD(List, remove);
O_OBJECT_METHOD(List, get);
O_OBJECT_METHOD(List, set);
O_OBJECT_METHOD(List, map);
O_OBJECT_METHOD(List, map_args);
O_OBJECT_METHOD(List, filter);
O_OBJECT_METHOD(List, filter_args);
O_OBJECT_METHOD(List, toString);
O_OBJECT_METHOD(List, getIterator);
O_END_OBJECT
