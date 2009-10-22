#include "RefList.h"
#include "String.h"
#include "ListIterator.h"

#define O_SUPER RefObject()

/**
 * This implements a list where the entries may be empty.
 * Also, a type (class) may be provided, so that entries
 * entered into the list can be cast to this type.
 */

O_IMPLEMENT(RefList, void *, ctor, (void *_self, va_list * app))
{
	struct RefList *self = O_CAST(_self, RefList());
	self = O_SUPER->ctor(self, app);
	self->max = va_arg(*app, unsigned);
	if (self->max <= 0)
		self->max = 1;
	self->type = va_arg(*app, struct ObjectClass *);
	self->data = calloc(self->max, sizeof(void *));
	self->length = 0;
	return self;
}

/* hook for deletion of members */
static void List_delete_items(void *_item)
{
	struct RefObject *item = o_cast(_item, RefObject());
	O_CALL(item, release);
}

O_IMPLEMENT(RefList, void *, dtor, (void *_self))
{
	struct RefList *self = O_CAST(_self, RefList());
	O_CALL(self, map, List_delete_items);
	free(self->data);
	self->data = NULL;
	return O_SUPER->dtor(self);
}

O_IMPLEMENT(RefList, void *, resize, (void *_self, unsigned size))
{
	struct RefList *self = O_CAST(_self, RefList());
	assert(size >= self->length);
	self->data = realloc(self->data, sizeof(void *[size]));
	if (size > 0);
		self->max = size;
	return self;
}

O_IMPLEMENT(RefList, void *, append, (void *_self, void *_item))
{
	struct RefList *self = O_CAST(_self, RefList());
	struct RefObject *item = O_CAST(_item, RefObject());
	/* if item and type, cast item to type */
	if (item && self->type)
		o_cast(item, self->type);
	/* check if need to be resized */
	if (self->length == self->max)
		self = O_CALL(self, resize, self->max << 1);
	/* append item */
	self->data[self->length] = O_CALL(item, retain);
	self->length++;
	return item;
}

static void RefList_append_item(void *_item, va_list * ap)
{
	struct RefList *list = o_cast(va_arg(*ap, struct RefList *), RefList());
	O_CALL(list, append, _item);
}

O_IMPLEMENT(RefList, void *, append_list, (void *_self, void *_list))
{
	struct RefList *self = O_CAST(_self, RefList());
	struct RefList *list = o_cast(_list, RefList());
	O_CALL(list, map_args, RefList_append_item, self);
	return self;
}

O_IMPLEMENT(RefList, void *, merge, (void *_self, void *_other))
{
	struct RefList *self = O_CAST(_self, RefList());
	struct RefList *other = O_CAST(_other, RefList());
	O_CALL(self, append_list, other);
	/* delete other list */
	other->length = 0;
	O_CALL(other, delete);
	/* return result */
	return self;
}

O_IMPLEMENT(RefList, void *, remove, (void *_self))
{
	struct RefList *self = O_CAST(_self, RefList());
	self->length--;
	return O_CALL(self->data[self->length], auto_release);
}

O_IMPLEMENT(RefList, void *, get, (void *_self, unsigned index))
{
	struct RefList *self = O_CAST(_self, RefList());
	assert(index < self->length);
	return self->data[index];
}

O_IMPLEMENT(RefList, void *, set, (void *_self, unsigned index, void *item))
{
	struct RefList *self = O_CAST(_self, RefList());
	assert(index < self->length && item);
	return self->data[index] = self->type ? o_cast(item, self->type) : item;
}

O_IMPLEMENT(RefList, void *, map, (void *_self, void (*fun) (void *)))
{
	struct RefList *self = O_CAST(_self, RefList());
	int i;
	const unsigned length = self->length;
	for (i = 0; i < length; i++) {
		fun(self->data[i]);
	}
	return self;
}

O_IMPLEMENT(RefList, void *, map_args,
	    (void *_self, void (*fun) (void *, va_list *),...))
{
	struct RefList *self = O_CAST(_self, RefList());
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

O_IMPLEMENT(RefList, void *, filter, (void *_self, int (*filter) (void *)))
{
	struct RefList *self = O_CAST(_self, RefList());
	int i;
	const unsigned length = self->length;
	struct RefList *result = O_CALL_CLASS(RefList(), new, self->length / 2, self->type);
	for (i = 0; i < length; i++) {
		if (filter(self->data[i]))
			O_CALL(result, append, self->data[i]);
	}
	return result;
}

O_IMPLEMENT(RefList, void *, filter_args,
	    (void *_self, int (*filter) (void *, va_list *),...))
{
	struct RefList *self = O_CAST(_self, RefList());
	int i;
	va_list ap;
	va_start(ap, filter);
	const unsigned length = self->length;
	struct RefList *result = O_CALL_CLASS(RefList(), new, self->length / 2, self->type);
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

O_IMPLEMENT(RefList, struct String *, to_string, (void *_self))
{
	struct RefList *self = O_CAST(_self, RefList());
	struct String *str = O_SUPER->to_string(self);

	struct String *enter = O_CALL_CLASS(String(), new, "\n");
	struct String *item_enter = O_CALL_CLASS(String(), new, "\n\t");

	int i;
	const unsigned length = self->length;
	O_CALL(str, append_str, " [\n");
	for (i = 0; i < length; i++) {
		struct RefObject *item = O_CALL(self, get, i);
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

O_IMPLEMENT(RefList, void *, getIterator, (void *_self))
{
	struct RefList *self = O_CAST(_self, RefList());
	return O_CALL_CLASS(ListIterator(), new, self);
}

O_OBJECT(RefList, RefObject);
O_OBJECT_METHOD(RefList, ctor);
O_OBJECT_METHOD(RefList, dtor);
O_OBJECT_METHOD(RefList, resize);
O_OBJECT_METHOD(RefList, append);
O_OBJECT_METHOD(RefList, append_list);
O_OBJECT_METHOD(RefList, merge);
O_OBJECT_METHOD(RefList, remove);
O_OBJECT_METHOD(RefList, get);
O_OBJECT_METHOD(RefList, set);
O_OBJECT_METHOD(RefList, map);
O_OBJECT_METHOD(RefList, map_args);
O_OBJECT_METHOD(RefList, filter);
O_OBJECT_METHOD(RefList, filter_args);
O_OBJECT_METHOD(RefList, to_string);
O_OBJECT_METHOD(RefList, getIterator);
O_END_OBJECT
