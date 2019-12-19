
#include "list.h"

void listReverse(List l) {
	
    Node curr = l->head;
    Node prev = NULL;

    while (curr != NULL) {

        Node soon = curr->next;
        curr->next = prev;

        prev = curr;
        curr = soon;
        //curr= curr->next; don't need this line because the previous line does it for us
    }

    l->head  = prev;

 
	
}

