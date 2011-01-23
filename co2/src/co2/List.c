#include "co2/List.h"
#include "co2/String.h"
#include "co2/ListIterator.h"

#define O_SUPER Object()

/**
 * This implements a list where the entries may be empty.
 * Also, a type (class) may be provided, so that entries
 * entered into the list can be cast to this type.
 */

O_IMPLEMENT(List, void *, ctor, (void *_self, va_list * app))
{
	struct List *self = O_CAST(_self, List());
	self = O_SUPER->ctor(self, app);
	self->max = va_arg(*app, unsigned);
	if (self->max <= 0)
		self->max = 1;
	self->type = va_arg(*app, struct ObjectClass *);
	self->data = calloc(self->max, sizeof(void *));
	self->length = 0;
	return self;
}

O_IMPLEMENT(List, void *, resize, (void *_self, unsigned size))
{
	struct List *self = O_CAST(_self, List());
	assert(size >= self->length);
	self->data = realloc(self->data, sizeof(void *[size]));
	if (size > 0)
		self->max = size;
	return self;
}

O_IMPLEMENT(List, void *, append, (void *_self, void *item))
{
	struct List *self = O_CAST(_self, List());
	/* if item and type, cast item to type */
	if (item && self->type)
		o_cast(item, self->type);
	/* check if need to be resized */
	if (self->length == self->max)
		self = O_CALL(self, resize, self->max << 1);
	/* append item */
	self->data[self->length] = item;
	self->length++;
	return item;
}

static void List_append_item(void *_item, va_list * ap)
{
	struct List *list = o_cast(va_arg(*ap, struct List *), List());
	O_CALL(list, append, _item);
}

O_IMPLEMENT(List, void *, append_list, (void *_self, void *_list))
{
	struct List *self = O_CAST(_self, List());
	struct List *list = o_cast(_list, List());
	O_CALL(list, map_args, List_append_item, self);
	return self;
}

O_IMPLEMENT(List, void *, merge, (void *_self, void *_other))
{
	struct List *self = O_CAST(_self, List());
	struct List *other = O_CAST(_other, List());
	O_CALL(self, append_list, other);
	/* delete other list */
	other->length = 0;
	O_CALL(other, delete);
	/* return result */
	return self;
}

O_IMPLEMENT(List, void *, remove, (void *_self))
{
	struct List *self = O_CAST(_self, List());
	self->length--;
	return self->data[self->length];
}

O_IMPLEMENT(List, void *, get, (void *_self, unsigned index))
{
	struct List *self = O_CAST(_self, List());
	assert(index < self->length);
	return self->data[index];
}

O_IMPLEMENT(List, void *, set, (void *_self, unsigned index, void *item))
{
	struct List *self = O_CAST(_self, List());
	assert(index < self->length && item);
	return self->data[index] = self->type ? o_cast(item, self->type) : item;
}

O_IMPLEMENT(List, void *, map, (void *_self, void (*fun) (void *)))
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
	    (void *_self, void (*fun) (void *, va_list *),...))
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

O_IMPLEMENT(List, void *, filter, (void *_self, int (*filter) (void *)))
{
	struct List *self = O_CAST(_self, List());
	int i;
	const unsigned length = self->length;
	struct List *result = O_CALL_CLASS(List(), new, self->length / 2, self->type);
	for (i = 0; i < length; i++) {
		if (filter(self->data[i]))
			O_CALL(result, append, self->data[i]);
	}
	return result;
}

O_IMPLEMENT(List, void *, filter_args,
	    (void *_self, int (*filter) (void *, va_list *),...))
{
	struct List *self = O_CAST(_self, List());
	int i;
	va_list ap;
	va_start(ap, filter);
	const unsigned length = self->length;
	struct List *result = O_CALL_CLASS(List(), new, self->length / 2, self->type);
	for (i = 0; i < length; i++) {
		va_list aq;
		va_copy(aq, ap);
		if (filter(self->data[i], &aq))
			O_CALL(result, append, self->data[i]);
		va_end(aq);
	}
	va_end(ap);
	return result;
}

/* hook for deletion of members */
static void List_delete_items(void *_item)
{
	struct Object *item = o_cast(_item, Object());
	O_CALL(item, delete);
}

O_IMPLEMENT(List, void *, dtor, (void *_self))
{
	struct List *self = O_CAST(_self, List());
	O_CALL(self, map, List_delete_items);
	free(self->data);
	self->data = NULL;
	return O_SUPER->dtor(self);
}

O_IMPLEMENT(List, struct String *, to_string, (void *_self))
{
	struct List *self = O_CAST(_self, List());
	struct String *str = O_SUPER->to_string(self);

	struct String *enter = O_CALL_CLASS(String(), new, "\n");
	struct String *item_enter = O_CALL_CLASS(String(), new, "\n\t");

	int i;
	const unsigned length = self->length;
	O_CALL(str, append_str, " [\n");
	for (i = 0; i < length; i++) {
		struct Object *item = O_CALL(self, get, i);
		struct String *item_str = O_CALL(item, to_string);
		O_CALL(item_str, replace, enter, item_enter);
		O_CALL(str, append_str, "\t");
		O_CALL(str, append, item_str);
		O_CALL(item_str, delete);
		O_CALL(str, append_str, ";\n");
	}
	O_CALL(str, append_str, "]");

	O_CALL(enter, delete);
	O_CALL(item_enter, delete);

	return str;
}

O_IMPLEMENT(List, void *, getIterator, (void *_self))
{
	struct List *self = O_CAST(_self, List());
	return O_CALL_CLASS(ListIterator(), new, self);
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
O_OBJECT_METHOD(List, to_string);
O_OBJECT_METHOD(List, getIterator);
O_END_OBJECT
