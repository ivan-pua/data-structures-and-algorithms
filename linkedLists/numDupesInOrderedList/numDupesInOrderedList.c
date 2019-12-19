
#include "list.h"


int numDupesInOrderedList(List l) {

	int duplicates = 0;
	Node curr = l->head;
	if (curr == NULL || curr->next == NULL) return duplicates;

	while (curr->next != NULL) {
		
		if(curr->value == curr->next->value){
			duplicates++;
		}

		curr = curr->next;
	}

	return duplicates;
}


// SLEEK CODE