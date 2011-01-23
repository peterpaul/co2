#ifndef LIST_LIST_H
#define LIST_LIST_H

#include "co2/LinkedList.h"

#define ListListClass_Attr						\
	LinkedListClass_Attr

#define ListList_Attr							\
	LinkedList_Attr;							\
	struct LinkedList * item

O_CLASS(ListList, LinkedList);

#endif				/* LIST_LIST_H */
