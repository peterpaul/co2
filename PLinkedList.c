#include "PLinkedList.h"
#include "PListList.h"

#define P_SUPER PObject()

P_IMPLEMENT(PLinkedList, void *, ctor, (void *_self, va_list * app))
{
	struct PLinkedList *self = P_CAST(_self, PLinkedList());
	self = P_SUPER->ctor(self, app);
	self->next = NULL;
	return self;
}

P_IMPLEMENT(PLinkedList, void *, dtor, (void *_self))
{
	struct PLinkedList *self = P_CAST(_self, PLinkedList());
	struct PLinkedList *next = self->next;
	self->next = NULL;
	/* Free the rest of the list iteratively instead of recursively,
	 * as for large lists it might run out of stack space.
	 */
	while (next) {
		struct PLinkedList *curr = next;
		next = next->next;
		curr->next = NULL;
		p_delete(curr);
	}
	return P_SUPER->dtor(self);
}

P_IMPLEMENT(PLinkedList, void *, merge_sorted, (void *_self, void *_other))
{
	struct PLinkedList * self = P_CAST(_self, PLinkedList());
	struct PLinkedList * other = P_CAST(_other, PLinkedList());
	struct PLinkedList * head = NULL, * tail = NULL;
	while (self && other) {
		if (P_CALL(self, compare, other) <= 0) {
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

static struct PListList * prepare_sort(struct PLinkedList * list)
{
	struct PListList * head = NULL, * tail = NULL;
	struct PLinkedList * last;
	assert(list);
	while(list && list->next) {
		APPEND_LIST(p_new(PListList(), list));
		while (list && list->next && P_CALL(list, compare, list->next) <= 0) {
			list = list->next;
		}
		last = list;
		list = list->next;
		last->next = NULL;
	}
	if (list) {
		APPEND_LIST(p_new(PListList(), list));
	}
	return head;
}

P_IMPLEMENT(PLinkedList, void *, sort, (void *_self))
{
	struct PLinkedList * self = P_CAST(_self, PLinkedList());
	struct PListList * head = prepare_sort(self), * tail;
	
	while (head->next) {
		tail = head;
		while (tail && tail->next) {
			struct PListList * tmp = tail->next;
			tail->item = (struct PLinkedList *)P_CALL(tail->item, merge_sorted, tmp->item);
			tail->next = tmp->next;
			tail = tmp->next;
			/* delete node */
			tmp->next = NULL;
			p_delete(tmp);
		}
	}
	self = head->item;
	/* delete node */
	head->next = NULL;
	p_delete(head);
	
	return self;
}

P_IMPLEMENT(PLinkedList, void *, map, (void *_self, void (*fun) (void *)))
{
	struct PLinkedList * self = P_CAST(_self, PLinkedList());
	if (self->next) {
		struct PLinkedList * next = self->next;
		P_CALL(next, map, fun);
	}
	return self;
}

P_OBJECT(PLinkedList,PObject);
self->ctor = PLinkedList_ctor;
self->dtor = PLinkedList_dtor;
self->sort = PLinkedList_sort;
self->merge_sorted = PLinkedList_merge_sorted;
self->map = PLinkedList_map;
P_END_OBJECT
