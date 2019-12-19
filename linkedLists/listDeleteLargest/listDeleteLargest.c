
#include "list.h"

int listDeleteLargest(List l) {
	
	Node curr = l->head;

	// If only 1 node in the list
	if (curr->next == NULL) return curr->value;


	// Checks for the largest value
	int max = curr->value;
	while (curr != NULL) {

		if (curr->value > max) {
			max = curr->value;
		}
		curr= curr->next;
	}
	
	curr = l->head;
	Node prev = NULL;

	while (curr != NULL) {

		if (curr->value == max && prev ==NULL) {
			l->head = curr->next;
			free (curr);
			break;
		}
		

		if (curr->value == max){
			Node n = curr;
			prev->next = curr->next;
			free(n);
			break;
		} 
		prev = curr;
		curr=curr->next;
	}
	return max;
}

