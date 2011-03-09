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
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "co2/Object.h"

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
