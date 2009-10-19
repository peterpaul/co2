#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Object.h"

/* This macro appends a linked list with item x.
 */
#define APPEND_LIST_HT(head,tail,item)			\
	if (!head) {								\
		tail = head = item;						\
	} else {									\
		(tail)->next = item;					\
		tail = (tail)->next;					\
	}

/* This macro appends a linked list with item x.
 */
#define APPEND_LIST(item)						\
	if (!head) {								\
		tail = head = item;						\
	} else {									\
		tail->next = item;						\
		tail = tail->next;						\
	}

O_METHOD_DEF(LinkedList, void *, sort, (void *_self));
O_METHOD_DEF(LinkedList, void *, merge_sorted,
	     (void *_self, void *_other));
O_METHOD_DEF(LinkedList, void *, map, (void *_self, void (*fun) (void *)));

#define LinkedListClass_Attr					\
	ObjectClass_Attr;							\
	O_METHOD(LinkedList, sort);				\
	O_METHOD(LinkedList, merge_sorted);		\
	O_METHOD(LinkedList, map)

#define LinkedList_Attr						\
	Object_Attr;								\
	void * next

O_CLASS(LinkedList, Object);

#endif				/* LINKEDLIST_H */
