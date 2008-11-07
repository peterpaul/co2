#ifndef PLIST_LIST_H
#define PLIST_LIST_H

#include "PLinkedList.h"

#define PListList_Class				\
	PLinkedList_Class

#define PListList_Attr				\
	PLinkedList_Attr;			\
	struct PLinkedList * item

P_CLASS(PListList, PLinkedList);

#endif /* PLIST_LIST_H */
