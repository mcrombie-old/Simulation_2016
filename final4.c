#include <stdio.h>
#include <math.h>
#include "rngs.h"
#include "rvgs.h"
#include "rvms.h"

#define START 0.0
#define STOP 2000
#define INFINITY (100.0 * STOP)


/*double * slope(int t[], double arrival[]){
	int i;
	double slopes[6];
	for(i = 0; i < 6; i++){
		slopes[i] = (arrival[i+1] - arrival[i]) / (t[i+1] - t[i]);
	}

	return slopes;
}*/

double Min(double a, double c){ 
  if (a < c)
    return (a);
  else
    return (c);
} 


double GetArrival(){

	SelectStream(0);

	int t[7] = {0, 200, 600, 1000, 1200, 1500, 2000}; //arrival time
	double arrival[7] = {0.5,0.5,0.9,0.9,1.2,0.9,0.5}; //arrival rate
	//double *slopes;
	//slopes = slope(t, arrival);
	double slopes[6] = {0, 0.001, 0, 0.0015, -0.001, -0.0008};
	int event_rates[7] = {0, 100, 380, 740, 950, 1265, 1615};

	//static double arrival_time = START;
	static double a = 0.0;

	if (a > 2000.0) a = 0.0;
	double u = 0.0;
	int n = 0;
	int j = 0;
	int k = 7; //the constant number of points

	//while (a < STOP){
		u += Exponential(1.0);
		while ((event_rates[j + 1] < u) && (j < k)){
			j++;
		}

		if (slopes[j] == 0){
			a += t[j] + ((u - event_rates[j]) / arrival[j]);
		}else{
			a +=  t[j] + ((2 * (u - event_rates[j]))/(arrival[j] + sqrt(pow(arrival[j],2) + (2 * slopes[j] * (u - event_rates[j])))));
		}

		//printf("%f\n", a);

		n++;
	//}
	//arrival_time += a; 

	return (a);
} 

double GetService(){
  SelectStream(1);
  return (Erlang(4, 0.25));
}  

int main(void)
{


  PlantSeeds(123);


int i;
//for (i = 0; i < 64; i++){
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
  t.current    = START;           /* set the clock                         */
  t.arrival    = GetArrival();    /* schedule the first arrival            */
  t.completion = INFINITY;        /* the first event can't be a completion */
  int flag = 0; //marks if the time knot has already been found to account for the variability in arrival times 

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
/*	if (t.arrival >= 200 && flag == 0){
		flag = 1;
		printf("%6.9f\n", area.node/t.current);
	}*/
	
	/*if (t.arrival >= 600 && flag == 0){
		flag = 1;
		printf("%6.9f\n", area.node/t.current);
	}

	if (t.arrival >= 1000 && flag == 0){
		flag = 1;
		printf("%6.9f\n", area.node/t.current);
	}

	if (t.arrival >= 1200 && flag == 0){
		flag = 1;
		printf("%6.9f\n", area.node/t.current);
	}
	if (t.arrival >= 1500 && flag == 0){
		flag = 1;
		printf("%6.9f\n", area.node/t.current);
	}
	if (t.arrival >= 2000 && flag == 0){
		flag = 1;
		printf("%6.9f....%d\n", area.node/t.current, i);
	}*/



	/*double variation = t.arrival - t.current;
	if (200 - variation <= t.arrival && t.arrival <= 200 + variation && flag == 0){
		flag = 1;
		printf("%6.9f......%6.9f....%d\n", area.node/t.current, t.arrival, i);
	}*/




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
  //}
}
  printf("\nfor %ld jobs\n", index);
  printf("   average interarrival time (for the stationary process) = %6.5f\n", t.last / index);
  printf("   average wait ............ = %6.2f\n", area.node / index);
  printf("   average delay ........... = %6.2f\n", area.queue / index);
  printf("   average service time .... = %6.2f\n", area.service / index);
  printf("   average # in the node ... = %6.2f\n", area.node / t.current);
  printf("   average # in the queue .. = %6.2f\n", area.queue / t.current);
  printf("   utilization ............. = %6.2f\n", area.service / t.current);

  return (0);
}
