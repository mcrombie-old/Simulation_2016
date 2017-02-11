/*FIFO implementation of a single server service node.*/


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

	double erl = Erlang(4, 0.02);
	return (erl);
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

  PlantSeeds(123);
  t.current    = START;           /* set the clock                         */
  t.arrival    = GetArrival();    /* schedule the first arrival            */
  t.completion = INFINITY;        /* the first event can't be a completion */

  while ((t.arrival < STOP) || (number > 0)) {
    t.next          = Min(t.arrival, t.completion);  /* next event time   */
    if (number > 0)  {                               /* update integrals  */
      area.node    += (t.next - t.current) * number;
      area.queue   += (t.next - t.current) * (number - 1);
      area.service += (t.next - t.current);


    }
    t.current       = t.next;                    /* advance the clock */

    if (t.current == t.arrival)  {               /* process an arrival */
      number++;
      t.arrival     = GetArrival();
      if (t.arrival > STOP)  {
        t.last      = t.current;
        t.arrival   = INFINITY;
      }
      if (number == 1)
        t.completion = t.current + GetService();
    }

    else {                                        /* process a completion */
      index++;
      number--;
      if (number > 0)
        t.completion = t.current + GetService();
      else
        t.completion = INFINITY;
    }

 //printf("%6.8f\n", area.node / t.current);
// printf("%6.8f\n", area.queue / t.current); 
 printf("%6.8f\n", area.service / t.current);
  } 

  /*printf("\nfor %ld jobs\n", index);
  printf("   average interarrival time = %6.8f\n", t.last / index);
  printf("   average wait ............ = %6.8f\n", area.node / index);
  printf("   average delay ........... = %6.8f\n", area.queue / index);
  printf("   average service time .... = %6.8f\n", area.service / index);
  printf("   average # in the node ... = %6.8f\n", area.node / t.current);
  printf("   average # in the queue .. = %6.8f\n", area.queue / t.current);
  printf("   utilization ............. = %6.8f\n", area.service / t.current);*/

  return (0);
}
		











