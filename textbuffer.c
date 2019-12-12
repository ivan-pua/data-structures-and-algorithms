
#include <stdlib.h>
#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <math.h>

#include "textbuffer.h"

typedef struct DLListNode {
	char *line;        /* Stores 1 line per node */
	struct DLListNode *prev;
	                    /**< pointer to previous node in list */
	struct DLListNode *next;
	                    /**< pointer to next node in list */
} DLListNode; // Don't change the name of struct because pre-defined in header file

// Representation - Double Linked List
typedef struct textbuffer {
   	int nLines;      /* < count of nodes in list */
	int numChar;
	char *textDuplicate; /* Stores the original text */
	DLListNode *first;  /* < first node in list */
	DLListNode *curr;   /* < current node in list */
	DLListNode *last;   /* < last node in list */
} textbuffer; // Don't change the name of struct because pre-defined in header file

static DLListNode *newNode (char *it);
static void freeDLListNode (DLListNode *node);
static Match newMatchNode (int lineNum, int colNum);
static int intToNumDigits (int n);
static char *replaceChar(char *s);
TB newDLList (void);

/**
 * Allocate a new textbuffer whose contents is initialised with the text
 * in the given string.
 */
TB newTB (char *text) {
   
	TB tb1 = newDLList();
	
	// Empty String Case
	if(text == NULL){
		fprintf(stderr, "NULL is not a valid string\n");
        abort();
	}

	// String with only '\n' character
	if (text[0] == '\n' && text[1] == '\0') {
		tb1->nLines= 1;
		DLListNode *singleNode = newNode(text);
		tb1->first = tb1->last =tb1->curr = singleNode;
		return tb1;
	}

	// Normal Case - more than one letter 

	char *first_sentence = calloc(1,10000);
	
	int i = 0;
	char firstCtoStr[2]; 

	while (text[i] != '\n'){
			
		firstCtoStr[0] = text[i];
		firstCtoStr[1] = '\0';	// By adding a '\0' after the character
		strcat(first_sentence, firstCtoStr); // Source and destinations must be strings
		i++;
	}

	// To include the '\n'
	firstCtoStr[0] = text[i];
	firstCtoStr[1] = '\0';
	strcat(first_sentence, firstCtoStr);
	i++;

	// Insert 1 line into node
	DLListNode *firstNode = newNode(first_sentence);
	tb1->first = tb1->last = tb1->curr = firstNode;
	DLListNode *currentNode = tb1->curr;

	// Find the size of string for malloc 
	int stringSize = strlen(text);

	while(text[i] != '\0') {
		char cToStr[2]; // Initialize to convert char to string 

		// Dynamically allocated memory 
		char *sentence = calloc(stringSize, sizeof(char)); // Memory leak here???

		while (text[i] != '\n'){
			
			cToStr[0] = text[i];
			cToStr[1] = '\0';	// By adding a '\0' after the character
			strcat(sentence, cToStr); // Source and destinations must be strings
			i++;
		}
		
		// To include the '\n'
		cToStr[0] = text[i];
		cToStr[1] = '\0';

		strcat(sentence, cToStr);	

		DLListNode *n = newNode(sentence);
		currentNode->next = n;	
		n->prev = currentNode;
		currentNode = currentNode->next;
	
		i++;
	}	
	tb1->last = currentNode;
	tb1->nLines = linesTB(tb1);
	tb1->textDuplicate = text;
	tb1->numChar = i;
	// free(first_sentence);
	return tb1;

}

/**
 * Free  the  memory occupied by the given textbuffer. It is an error to
 * access the buffer afterwards.
 */
void releaseTB (TB tb) {
	
	if (tb == NULL) return;

	DLListNode *curr = tb->first;
	while (curr != NULL) {
		DLListNode *next = curr->next;
		freeDLListNode (curr);
		curr = next;
	}
	free (tb);
}


/**
 * Allocate  and return a string containing all of the text in the given
 * textbuffer. If showLineNumbers is true, add a prefix corresponding to
 * the line number.
 */
char *dumpTB (TB tb, bool showLineNumbers) {
	
	DLListNode *curr = tb->first;

	tb->nLines = linesTB(tb);

	/* Sets an upper limit for the memory to ve allocated
	 * Number of Lines in TextBuffer * (numbering + '.' + ' ') + 
	 * total num of chracters in the text buffer + null terminator '\0'
	 * e.g. "1. Hello World\nHow\n\0"
	 */
	int bufferSize = (tb->nLines)*(3+intToNumDigits(tb->nLines)) + tb->numChar;
	
	char *text = calloc(bufferSize, sizeof(char));

	// If true, prepend a line number (along with a dot and space) to each line of the output.
	if (showLineNumbers == true ){
		
		int i = 1;
		while(curr != NULL) {
			
			// Line size is added by 100 as an upper limit
			int lineSize = strlen(curr->line)+(3+intToNumDigits(tb->nLines)) + 100;

			// Allocate memory dynamically based on the size of string.
			char *outputLine = calloc(lineSize, sizeof(char));

			// Creates a new line 
			snprintf(outputLine, lineSize, "%d. %s", i, curr->line);
			strcat(text, outputLine);
			curr=curr->next;
			i++;
			free(outputLine);
		}

	// showLineNumber == false
	} else {

		while(curr != NULL) {
			strcat(text, curr->line);
			curr=curr->next;
		} 
	}
	return text;
}

// Return the number of lines of the given textbuffer.
int linesTB (TB tb) {
	
	DLListNode *curr = tb->first;
	int lineCount = 0;
	while (curr != NULL) {
		lineCount++;
		curr = curr->next;
	}
	return lineCount;
}

/**
 * Add a given prefix to all lines between 'from' and 'to', inclusive.
 * - The  program  should abort() wih an error message if 'from' or 'to'
 *   is out of range. The first line of a textbuffer is at position 1.
 */
void addPrefixTB (TB tb, int from, int to, char *prefix) {
	
	int numLines = linesTB(tb);
	if (from > numLines || to > numLines || to < from) {
		fprintf(stderr, "to or from is out of bounds\n");
		abort();
	}

	DLListNode *curr = tb->first;

	int lineNum = 1;
	while (curr != NULL) {

		if (lineNum >= from && lineNum <= to){
			
			// Creates a new string to store 'prefix' + curr->line
			int prefixedLineSize = strlen(prefix) + strlen(curr->line) + 1;
			char *prefixedLine = calloc(prefixedLineSize, sizeof(char));
			strcpy(prefixedLine, prefix);
			strcat(prefixedLine, curr->line);

			// Updates curr->line
			curr->line = prefixedLine;
			// free(prefixedLine); 
		}
		curr = curr->next;
		lineNum++;
	}
}

/**
 * Merge 'tb2' (source) into 'tb1' (destination) at line 'pos' or position.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the merged-in lines from 'tb2'.
 *   - 'tb2' can't be used anymore (as if we had used releaseTB() on it)
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void mergeTB (TB tb1, int pos, TB tb2) {
	int numLinesOfDest = linesTB(tb1);
	if (pos > numLinesOfDest+1){
		fprintf(stderr, "to or from is out of bounds\n");
		abort();
	}

	if (tb1 == tb2) {
		return;
	}
	// Updates the number of characters of the tb to be merged 
	tb1->numChar = tb1->numChar + tb2->numChar;

	// Insert src before the start of dest
	if (pos ==1){
		tb2->last->next = tb1->first;
		tb1->first->prev = tb2->last;
		tb1->first = tb2->first;
	}
	// Append src to the end of dest
	else if (pos == linesTB(tb1)+1) {
		 tb1->last->next = tb2->first;
		 tb1->first->prev = tb1->last;
		 tb1->last = tb2->last;
	}
	// Insert src before the pos
	else {
		 DLListNode *curr = tb1->first;
		 int count = 1;

		 // Go to the node at pos
		 while (count != pos) {
			 count++;
			 curr=curr->next;
		 }
		 DLListNode *n = curr;
		 curr->prev->next = tb2->first;
		 tb2->first->prev = curr->prev;

		 tb2->last->next = n;
		 n->prev = tb2->last; 
	}
	free(tb2);
	
}

/**
 * Copy 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the pasted-in lines from 'tb2'.
 *   - 'tb2' is unmodified and remains usable independent of tb1.
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void pasteTB (TB tb1, int pos, TB tb2) {

	int numLinesOfDest = linesTB(tb1);
	if (pos > numLinesOfDest+1){
		fprintf(stderr, "to or from is out of bounds\n");
		abort();
	}

	// Create a new string to store all text from tb2
	char *tb2Text = dumpTB(tb2, false);
	TB temp = newTB(tb2Text);
	
	// Combine both texts using mergeTB function
	mergeTB(tb1, pos, temp);
	free(tb2Text);
}

/**
 * Cut  the lines between and including 'from' and 'to' out of the given
 * textbuffer 'tb' into a new textbuffer.
 * - The result is a new textbuffer (much as one created with newTB()).
 * - The cut lines will be deleted from 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
TB cutTB (TB tb, int from, int to) {
	
	if (to < from) {
		return NULL;
	}

	if (from > tb->nLines || to > tb->nLines) {
		fprintf(stderr, "to or from is out of bounds\n");
		abort();
	}

	DLListNode *curr = tb->first;

	// Go to node to be cut
	int count = 1;
	while (count != from) {
		count++;
		curr= curr->next;
	}
	
	// Creates a string to store the cut lines
	char *textToBeCut = calloc(sizeof(char), tb->numChar); 
	
	int i = from;
	while (i <= to) {
		strcat(textToBeCut, curr->line);
		curr  = curr->next;
		i++;
	}
	// Delete the lines of the original text buffer from line number "from" to "to"
	deleteTB(tb, from, to);

	// Creates a new TB for the "textToBeCut"
	TB tbToBeCut = newTB(textToBeCut);
	tbToBeCut->nLines = to - from + 1;
	free(textToBeCut);
	return tbToBeCut;
}

/**
 * Return  a  linked list of match nodes containing the positions of all
 * of the matches of string 'search' in 'tb'.
 * - The textbuffer 'tb' should remain unmodified.
 * - The user is responsible for freeing the returned list.
 */
Match searchTB (TB tb, char *search) {
	
	if (search == NULL) {
		abort();
	}

	// first line
	DLListNode *curr = tb->first;

	int line = 1;
	int col = 1;
	Match firstMatchNode = newMatchNode(0, 0);
	Match m = firstMatchNode;

	while (line <= tb->nLines){
		if (strstr(curr->line, search) != NULL) {

			// Find the specific string in the textbuffer
			char *substring = strstr(curr->line, search);

			// To determine the col of the character
			col = strlen(curr->line) - strlen (substring) + 1;

			// Create a new Match node
			Match n = newMatchNode(line, col);

			m->next = n;
			m = m->next;

		}  
		curr = curr->next;
		line ++;
	}

	Match node = firstMatchNode->next;
	// Free first node in match linked list
	free(firstMatchNode);
	return node; 
}

/**
 * Remove  the  lines between 'from' and 'to' (inclusive) from the given
 * textbuffer 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
void deleteTB (TB tb, int from, int to) {

	int numLines = linesTB(tb);
	if (from > numLines || to > numLines || to < from) {
		fprintf(stderr, "to or from is out of bounds\n");
		abort();
	}

	DLListNode *curr = tb->first;

	int lineNum = 1;
	while (curr != NULL) {

		if (lineNum == from ) {
			//int charToDelete = strlen(curr->line);
			while (lineNum <= to){
				DLListNode *nodeToDelete = curr;

				/* If only node in the list */
				if (nodeToDelete->prev == NULL && nodeToDelete->next == NULL) {
					curr = NULL;
					tb->first = tb->last = NULL;
				}
					
				/* If node to be deleted is head node */
				else if  (nodeToDelete->prev == NULL && nodeToDelete->next !=NULL) {
					tb->first= nodeToDelete->next;        
					curr = tb->first;
					nodeToDelete->next->prev=NULL;
				}
					
				// If node to be deleted is last node 
				else if (nodeToDelete->prev != NULL && nodeToDelete->next == NULL) {
					tb->last = nodeToDelete->prev;
					curr= nodeToDelete->prev;
					nodeToDelete->prev->next = NULL;             
				} 
					
				/* Change next only if node to be deleted is NOT the head or last node */
				else  {
					// FOR DOUBLY LINKED LIST, ALWAYS DEFINE BOTH DIRECTIONS
					nodeToDelete->next->prev = nodeToDelete->prev; 
					nodeToDelete->prev->next = nodeToDelete->next;
					curr = nodeToDelete->next;
				}
				// Increment line counter
				lineNum++;
				tb->nLines--;
				freeDLListNode(nodeToDelete);
			}
		
		} else {
			
			//printf("%s", curr->line);
			curr = curr->next;			
			lineNum++;
		}
	
	}

}

/**
 * Search  every  line of the given textbuffer for every occurrence of a
 * set of specified substitutions and alter them accordingly.
 * - Refer to the spec for details.
 */
void formRichText (TB tb) {

	//char *text = dumpTB(tb, false);
	DLListNode *curr = tb -> first;
	while (curr != NULL) {
		char *text = curr -> line;
		char *newText = replaceChar(text);
		curr -> line = newText;
		curr = curr -> next;
		// free(text);
		// free(newText);
	}
}

////////////////////////////////////////////////////////////////////////
// Bonus challenges

char *diffTB (TB tb1, TB tb2) {
	return NULL;
}

void undoTB (TB tb) {

}

void redoTB (TB tb) {

}

// Creates DLList (node) that contains a line of text returns pointer of the node 
static DLListNode *newNode (char *it)
{
	DLListNode *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate Match node");
	new->line = it;
	new->prev = new->next = NULL;
	return new;
}

// Creates a new Match Node 
static Match newMatchNode (int lineNum, int colNum)
{
	Match new = malloc (sizeof(struct _matchNode));
	if (new == NULL) err (EX_OSERR, "couldn't allocate Match node");
	new->lineNumber = lineNum;
	new->columnNumber = colNum;
	new->next = NULL;
	return new;
}

// Release a DLListNode 
static void freeDLListNode (DLListNode *node)
{
	if (node == NULL) return;
	free (node->line); // need to free the pointer within the pointer 
	free (node);
}


// Create a new, empty text buffer TB
TB newDLList (void)
{
	textbuffer *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate DLList");
	new->curr = new->first = new->last = NULL;
	new->nLines = 0;
	new->numChar = 0;
	new->textDuplicate = "";
	return new;

	// Note to self: If use calloc(), don't need that many lines ^
}

// Converts the digits into number of characters
// For example, "100" has three characters
static int intToNumDigits (int n) {

	// Add 1 in case there are decimal places. 
	int length = log10(n)+1;
	return length;
}

// Performs substitutions for special characters and returns the string
static char *replaceChar(char text[]) 
{ 
	
	char *newText = calloc(strlen(text) + 10000, sizeof(char)); 
	// "#" Case
	if (text[0] == '#') {
		strcpy(newText, "<h1>");
		strcat(newText, text+1);
		newText[strlen(newText)-1] = '\0';

		strcat(newText, "</h1>\n");

	// "*" and "_" Case
	} else {
		int i = 0;
		int lastIndex = 0;
		while (text[i] != '\0') {
			if (text[i] == '*'){
				// find the second asterisk, starting from index i to the end 
				int j = i+1;
				if (text[i+1] != '*'){
					while (text[j] != '\0') {
						if (text[j] == '*'){
							text[i] = '\0';
							text[j] = '\0';
							strcat(newText, &text[lastIndex]);
							strcat(newText, "<b>");
							strcat(newText, &text[i+1]);
							strcat(newText, "</b>");
							lastIndex = j+1;
							i = j;
							break;
						}
						j++;
					}
				}

			} else if (text[i] == '_'){
				// find the underline or asterisk, starting from index i to the end 
				int j = i+1;
				if (text[i+1] != '_'){
					while (text[j] != '\0') {
						if (text[j] == '_'){

							text[i] = '\0';
							text[j] = '\0';
							strcat(newText, &text[lastIndex]);
							strcat(newText, "<i>");
							strcat(newText, &text[i+1]);
							strcat(newText, "</i>");
							lastIndex = j+1;
							i = j;
							break;
						}
						j++;
					}
				}
			}

			i++;
		}
	strcat(newText, &text[lastIndex]);
	}
	
	return newText;
} 