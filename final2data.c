#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rngs.h"
#include "rvgs.h"
#include "rvms.h"

#define START 0.0  //initial time
#define STOP 1000.0 //terminal time
#define INFINITY (100.0 * STOP)  //gots to be larger than STOP

//returns the smaller between a and b
double Min(double a, double c){
	if (a < c)
		return (a);
	else
		return (c);
}

//arrival time at lambda = 10.0
double GetArrival(){
	static double arrival = START;

	SelectStream(0);
	arrival += Exponential(0.1);
	return (arrival);
}

double GetService(){
	SelectStream(1);

	return  Erlang(4, 0.02);
}

//Linked list
typedef struct node {
	double val;
	struct node * next;
} node_t;

void printList(node_t * head){
	node_t * current = head;

	while (current != NULL){
		printf("%.6f\n", current->val);
		current = current->next;
	}
}

//add to the end of the list
void put(node_t ** head, double val){
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
void push(node_t ** head, double val){
	node_t * new_node;
	new_node = malloc(sizeof(node_t));

	new_node->val = val;
	new_node->next = *head;
	*head = new_node;
}

//removes the first item of linkedlist
double * pop(node_t ** head){

	node_t * next_node = NULL;

	next_node = (*head)->next;
	free(*head);
	*head = next_node;
}		

//removes the node at the given index
double removeAtIndex(node_t ** head, int n){
	int i = 0;
	int returnVal = -1;
	node_t * current = *head;
	node_t * temp_node = NULL;

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
double findMin(node_t ** head){
	node_t * current = *head;
	int index = 0;
	int minIndex = 0;
	double minVal = current->val;
	while(current->next != NULL){
		if(minVal >= current->val){
			minVal = current->val;
			minIndex = index;
		}
		current = current->next;
		index++;
	}
	

	if (minIndex == 0){
		//printf("%f\n", minVal);
		pop(head);
	}
	else{
		removeAtIndex(head, minIndex);
	}
	
	return (minVal);
}

   int main(void)
{
  struct {
    double arrival;                 /* next arrival time                   */
    double completion;              /* next completion time                */
    double current;                 /* current time                        */
    double next;                    /* next (most imminent) event time     */
    double last;                    /* last arrival time                   */
  } t;
  struct {
    double node;                    /* time integrated number in the node  */
    double queue;                   /* time integrated number in the queue */
    double service;                 /* time integrated number in service   */
  } area      = {0.0, 0.0, 0.0};
  long index  = 0;                  /* used to count departed jobs         */
  long number = 0;                  /* number in the node                  */
  double service_time;

  PlantSeeds(-1);
  t.current    = START;           /* set the clock                         */
  t.arrival    = GetArrival();    /* schedule the first arrival            */

  node_t * head = NULL; //build the queue list
  //head = malloc(sizeof(node_t));
  service_time = GetService();	
  push(&head,service_time); //put the new arrival in it*/

  t.completion = INFINITY;        /* the first event can't be a completion */

  while ((t.arrival < STOP) || (number > 0)) {
    t.next          = Min(t.arrival, t.completion);  /* next event time   */
    if (number > 0)  {                               /* update integrals  */
      area.node    += (t.next - t.current) * number;
      area.queue   += (t.next - t.current) * (number - 1);
      area.service += (t.next - t.current);

	//double result = (t.next - t.current) * number; //l
//	double result = (t.next - t.current) * (number - 1);//q
//	double result = (t.next - t.current);//x
 	// printf("%6.8f\n", result);

    }
    t.current       = t.next;                    /* advance the clock */

    if (t.current == t.arrival)  {               /* process an arrival */
      number++;
      t.arrival     = GetArrival();

      service_time = GetService();	
      push(&head, service_time);  //add a new service time to the queue
      //printf("%.8f\n", head->val);
      if (t.arrival > STOP)  {
        t.last      = t.current;
        t.arrival   = INFINITY;
      }
      if (number == 1){
	service_time = findMin(&head);
	t.completion = t.current + service_time; 
	}
    }

    else {                                        /* process a completion */
      index++;
      number--;
      if (number > 0){
	double service_time = findMin(&head);
        t.completion = t.current + service_time; 
	}
      else
        t.completion = INFINITY;
    }
  //printf("%6.8f\n", area.node / t.current);
 // printf("%6.8f\n", area.queue / t.current);
  printf("%6.8f\n", area.service / t.current);

  } 

 /* printf("\nfor %ld jobs\n", index);
  printf("   average interarrival time = %6.8f\n", t.last / index);
  printf("   average wait ............ = %6.8f\n", area.node / index);
  printf("   average delay ........... = %6.8f\n", area.queue / index);
  printf("   average service time .... = %6.8f\n", area.service / index);
  printf("   average # in the node ... = %6.8f\n", area.node / t.current);
  printf("   average # in the queue .. = %6.8f\n", area.queue / t.current);
  printf("   utilization ............. = %6.8f\n", area.service / t.current);*/




  return (0);
}
		











