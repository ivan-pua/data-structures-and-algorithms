
#include "list.h"

bool listIsPalindromic(List l) {
	Node start=l->first;
	Node end = l->last;
	if(start == NULL || start->next == NULL) return true;

	int i = 0;
	bool isPalindromic = true;
	while (i < (l->size)/2) {

		if(start->value != end->value) {
			isPalindromic = false;
			break;
		}
		start = start->next;
		end = end->prev;
		i++;

	}
	return isPalindromic;
}

