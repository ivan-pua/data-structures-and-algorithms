
#include "list.h"

bool listIsOrdered(List l) {
	
	Node curr = l->head;

	if (curr == NULL || curr->next == NULL) return true; // An empty list or list with a single node is considered to be sorted

	int ascending = 0;
	int descending = 0;

	if (curr->next->value > curr->value) {
		
		ascending = 1;
		while (curr->next != NULL) {

			if(curr->next->value < curr->value) {
				ascending = 0;

			}
			curr=curr->next;
		}
	} else {
		descending = 1;
		while (curr->next != NULL) {

			if(curr->next->value > curr->value) {
				descending = 0;

			}
			curr=curr->next;
		}
	}
	
	if(ascending == 0 && descending == 0) {
		return false;
	}
	
	return true;
}

