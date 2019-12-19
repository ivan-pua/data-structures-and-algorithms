
#include "list.h"

void reverseDLList(List l) {
	
	Node curr = l->first;
	if (curr == NULL) return;

	l->last = curr;
	Node prev = NULL;

	while (curr != NULL) {

		Node soon = curr->next;

		curr->next = prev;
		curr->prev = soon;

		prev = curr;

		curr = soon;
		
		
	}

	l->first = prev;
	
}

