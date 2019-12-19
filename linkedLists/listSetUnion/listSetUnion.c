
#include "list.h"
// refer geeks for geeks algo - pretty similar to mine
static void pushNode (List l, int val);
static bool isPresent (List l, Node n);

List listSetUnion(List s1, List s2) {
	
	List result = newList();
	Node list1 = s1->head;
	Node list2 = s2->head;

	// Insert all elements of list1 to the result list 
	while (list1 != NULL) {

		pushNode(result, list1->value);
		
		list1 = list1->next;
	}

	while (list2 != NULL) {
 
		if(!isPresent(result, list2)){
			pushNode(result, list2->value);
		}

		list2= list2->next;
	}

	return result;
}

static void pushNode (List l, int val){

	Node n = newNode(val);
	
	if(l == NULL){
		l->head = n;
	} else {
		
		n -> next = l->head;
		l->head = n;
	}
}

static bool isPresent (List l, Node n){
	Node curr = l->head;
	bool isFound = false;
	while (curr != NULL) {

		if (curr->value == n->value) {
			isFound =true;
		}
		curr = curr->next;
	}
	return isFound;
}


