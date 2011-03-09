/*
 * libco2 Library for object oriented programming in C
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "co2/LinkedList.h"
#include "co2/ListList.h"

#define O_SUPER Object()

O_IMPLEMENT(LinkedList, void *, ctor, (void *_self, va_list * app))
{
	struct LinkedList *self = O_CAST(_self, LinkedList());
	self = O_SUPER->ctor(self, app);
	self->next = NULL;
	return self;
}

O_IMPLEMENT(LinkedList, void *, dtor, (void *_self))
{
	struct LinkedList *self = O_CAST(_self, LinkedList());
	struct LinkedList *next = self->next;
	self->next = NULL;
	/* Free the rest of the list iteratively instead of recursively,
	 * as for large lists it might run out of stack space.
	 */
	while (next) {
		struct LinkedList *curr = next;
		next = next->next;
		curr->next = NULL;
		O_CALL(curr, delete);
	}
	return O_SUPER->dtor(self);
}

O_IMPLEMENT(LinkedList, void *, merge_sorted, (void *_self, void *_other))
{
	struct LinkedList *self = O_CAST(_self, LinkedList());
	struct LinkedList *other = O_CAST(_other, LinkedList());
	struct LinkedList *head = NULL, *tail = NULL;
	while (self && other) {
		if (O_CALL(self, compare, other) <= 0) {
			/* if (O_CALL(self, compare, other) <= 0) { */
			APPEND_LIST(self);
			self = self->next;
		} else {
			APPEND_LIST(other);
			other = other->next;
		}
	}
	if (self) {
		tail->next = self;
	} else if (other) {
		tail->next = other;
	} else {
		tail->next = NULL;
	}
	return head;
}

static struct ListList *prepare_sort(struct LinkedList *list)
{
	struct ListList *head = NULL, *tail = NULL;
	struct LinkedList *last;
	assert(list);
	while (list && list->next) {
		APPEND_LIST(O_CALL_CLASS(ListList(), new, list));
		while (list && list->next
		       && O_CALL(list, compare, list->next) <= 0) {
			list = list->next;
		}
		last = list;
		list = list->next;
		last->next = NULL;
	}
	if (list) {
		APPEND_LIST(O_CALL_CLASS(ListList(), new, list));
	}
	return head;
}

O_IMPLEMENT(LinkedList, void *, sort, (void *_self))
{
	struct LinkedList *self = O_CAST(_self, LinkedList());
	struct ListList *head = prepare_sort(self), *tail;

	while (head->next) {
		tail = head;
		while (tail && tail->next) {
			struct ListList *tmp = tail->next;
			tail->item =
			    (struct LinkedList *) O_CALL(tail->item,
							 merge_sorted,
							 tmp->item);
			/* O_CALL(tail->item, merge_sorted, tmp->item); */
			tail->next = tmp->next;
			tail = tmp->next;
			/* delete node */
			tmp->next = NULL;
			O_CALL(tmp, delete);
		}
	}
	self = head->item;
	/* delete node */
	head->next = NULL;
	O_CALL(head, delete);

	return self;
}

O_IMPLEMENT(LinkedList, void *, map, (void *_self, void (*fun) (void *)))
{
	struct LinkedList *self = O_CAST(_self, LinkedList());
	if (self->next) {
		struct LinkedList *next = self->next;
		O_CALL(next, map, fun);
		/* O_CALL(next, map, fun); */
	}
	return self;
}

O_OBJECT(LinkedList, Object);
O_OBJECT_METHOD(LinkedList, ctor);
O_OBJECT_METHOD(LinkedList, dtor);
O_OBJECT_METHOD(LinkedList, sort);
O_OBJECT_METHOD(LinkedList, merge_sorted);
O_OBJECT_METHOD(LinkedList, map);
O_END_OBJECT
