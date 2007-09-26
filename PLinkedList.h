#ifndef PLINKED_LIST_H
#define PLINKED_LIST_H

#include "PObject.h"

/* This macro appends a linked list with item x.
 */
#define APPEND_LIST_HT(head,tail,item)		\
	if (!head) {				\
		tail = head = item;		\
	} else {				\
		(tail)->next = item;		\
		tail = (tail)->next;		\
	}

/* This macro appends a linked list with item x.
 */
#define APPEND_LIST(item)			\
	if (!head) {				\
		tail = head = item;		\
	} else {				\
		tail->next = item;		\
		tail = tail->next;	\
	}

P_METHOD_DEF(PLinkedList, void *, sort, (void *_self));
P_METHOD_DEF(PLinkedList, void *, merge_sorted, (void *_self, void *_other));
P_METHOD_DEF(PLinkedList, void *, map, (void *_self, void (*fun)(void *)));
P_METHOD_DEF(PLinkedList, int, compare, (const void *_self, const void *_other));

#define PLinkedList_Class			\
	PObject_Class;				\
	P_METHOD(PLinkedList, sort);		\
	P_METHOD(PLinkedList, merge_sorted);	\
	P_METHOD(PLinkedList, map);		\
	P_METHOD(PLinkedList, compare)
	     
#define PLinkedList_Attr			\
	PObject_Attr;				\
	void * next

P_CLASS(PLinkedList, PObject);

#endif /* PLINKED_LIST_H */
