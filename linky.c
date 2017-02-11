#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int val;
	struct node * next;
} node_t;

void printList(node_t * head){
	node_t * current = head;

	while (current != NULL){
		printf("%d\n", current->val);
		current = current->next;
	}
}

//add to the end of the list
void put(node_t ** head, int val){
	node_t * current;
	current = malloc(sizeof(node_t));
	while(current->next != NULL){
		current = current->next;
	}

	current->next = *head;
	current->val = val;
	//current->next->next = NULL;
}

//add to the beginning of the list
void push(node_t ** head, int val){
	node_t * new_node;
	new_node = malloc(sizeof(node_t));

	new_node->val = val;
	new_node->next = *head;
	*head = new_node;
}

//removes the first item of linkedlist
int * pop(node_t ** head){

	node_t * next_node = NULL;

	next_node = (*head)->next;
	free(*head);
	*head = next_node;
}		

//removes the node at the given index
int removeAtIndex(node_t ** head, int n){
	int i = 0;
	int returnVal = -1;
	node_t * current = *head;
	node_t * temp_node = NULL;

	/*if(n == 0){
		returnVal = (*head)->val;
		pop(head);
		//pop(head);
		return returnVal;
	}*/


	for (i = 0; i < n-1; i++){
		if(current->next == NULL) return -1;
		current = current->next;
	}

	temp_node = current->next;
	returnVal = temp_node->val;
	current->next = temp_node->next;
	free(temp_node);

	return returnVal;
}


//finds the minimum value and removes it from the list
int findMin(node_t ** head){
	node_t * current = *head;
	int index = 0;
	int minIndex = 0;
	int minVal = current->val;
	while(current->next != NULL){
		if(minVal >= current->val){
			minVal = current->val;
			minIndex = index;
		}
		current = current->next;
		index++;
	}
	

	if (minIndex == 0){
		pop(head);
	}
	else{
		removeAtIndex(head, minIndex);
	}
	
	return (minIndex);
}


int main(void){

	
	node_t * head = NULL;

	push(&head,5);

	push(&head, 3);
	push(&head, 4);
	push(&head,34);
	push(&head,7);
	
	findMin(&head);


	printList(head);

}

