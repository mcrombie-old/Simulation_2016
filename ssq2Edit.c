
/* ------------------------------------------------------------------------- 
 *EDITED VERSION
 * This program - an extension of program ssq1.c - simulates a single-server 
 * FIFO service node using Exponentially distributed interarrival times and 
 * Uniformly distributed service times (i.e. a M/U/1 queue). 
 *
 * Name              : ssq2.c  (Single Server Queue, version 2)
 * Author            : Steve Park & Dave Geyer 
 * Language          : ANSI C 
 * Latest Revision   : 9-11-98
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>
#include <math.h>                                             
#include "rng.h"
#include "rngs.h"

#define LAST         100000L                   /* number of jobs processed */ 
#define START        0.0                      /* initial time             */ 
	int count0 = 0;
	int count1 = 0;
	int count2 = 0;

   double Exponential(double m)                 
/* ---------------------------------------------------
 * generate an Eixponential random variate, use m > 0.0 
 * ---------------------------------------------------
 */
{                                       
  return (-m * log(1.0 - Random()));     
}


   double Uniform(double a, double b)           
/* --------------------------------------------
 * generate a Uniform random variate, use a < b 
 * --------------------------------------------
 */
{                                         
  return (a + (b - a) * Random());    
}


   double GetArrival(int *j)
/* ------------------------------
 * generate the next arrival time
 * ------------------------------
 */ 
{       
	const double mean[3]={4.0,6.0,8.0};
	static double arrival[3] ={START,START,START};
	static int init = 1;
	double temp;



	if (init)
	{ 
		SelectStream(0);
		arrival[0] += Exponential(mean[0]);

		SelectStream(1);
		arrival[1] += Exponential(mean[1]);
		
		SelectStream(2);
		arrival[2] += Exponential(mean[2]);
		
		init = 0;

	}
	if ((arrival[0] <= arrival[1]) && (arrival[0] <= arrival[2])){
	*j=0;
	count0++;
	}

	else if ((arrival[1] <= arrival[2]) && (arrival[1] <= arrival[0])){
	*j=1; 
	count1++;
	}

	else {
	*j = 2;
	count2++;
	}

	temp = arrival[*j];
	SelectStream(*j);
	arrival[*j] += Exponential(mean[*j]);
	return (temp);
}




   double GetService(int j)
/* ------------------------------
 * generate the next service time
 * ------------------------------
 */ 
{
  const double min[3] = {0.0,1.0,1.0};
  const double max[3] = {2.0,2.0,5.0};
  SelectStream(j+3);
  return (Uniform(min[j],max[j]));
}


  int main(void)
{
  long   index     = 0;  
  long index0 = 0;
  long index1 = 0;
  long index2 = 0; 
  int j;                      /* job index            */
  double arrival   = START;                     /* time of arrival      */
  double delay;                                 /* delay in queue       */
  double service;                               /* service time         */
  double wait;                                  /* delay + service      */
  double departure = START;                     /* time of departure    */
  struct {                                      /* sum of ...           */
    double delay;                               /*   delay times        */
    double wait;                                /*   wait times         */
    double service;                             /*   service times      */
    double interarrival;                        /*   interarrival times */
  } sum = {0.0, 0.0, 0.0}; 

 
    double delay0 = 0.0;
    double wait0= 0.0;
    double service0= 0.0;
    double interarrival0;
 


 
    double delay1= 0.0;
    double wait1= 0.0;
    double service1= 0.0;
    double interarrival1;
 

 
    double delay2= 0.0;
    double wait2= 0.0;
    double service2= 0.0;
    double interarrival2;
  

  PlantSeeds(123456789);

  while (index < LAST) {
    index++;
    arrival      = GetArrival(&j);
    if (arrival < departure)
      delay      = departure - arrival;         /* delay in queue    */
    else
      delay      = 0.0;                         /* no delay          */
    service      = GetService(j);
    wait         = delay + service;
    departure    = arrival + wait;              /* time of departure */
    sum.delay   += delay;
    sum.wait    += wait;
    sum.service += service;

    if(j==0){
	delay0 += delay;
	wait0 += wait;
	service0 += service;
	index0++;
	}
    else if(j==1){
	delay1 += delay;
	wait1 += wait;
	service1 += service;
	index1++;
	}
    else if(j==2){
	delay2 += delay;
	wait2 += wait;
	service2 += service;
	index2++;
	}




  } 
  sum.interarrival = arrival - START;

  printf("\nfor %ld jobs\n", index);
  printf("job0 - %ld  job1 - %ld  job2 - %ld\n",count0,count1,count2);


  printf("For Job 0\n");

  printf("   average wait ............ = %6.2f\n", wait0 / index0);
  printf("   average delay ........... = %6.2f\n", delay0 / index0);
  printf("   average service time .... = %6.2f\n", service0 / index0);
  printf("   average # in the node ... = %6.2f\n", wait0 / departure);
  printf("   average # in the queue .. = %6.2f\n", delay0 / departure);
  printf("   utilization ............. = %6.2f\n", service0 / departure);

  printf("For Job 1\n");

  printf("   average waiti ............ = %6.2f\n", wait1 / index1);
  printf("   average delay ........... = %6.2f\n", delay1 / index1);
  printf("   average service time .... = %6.2f\n", service1 / index1);
  printf("   average # in the node ... = %6.2f\n", wait1 / departure);
  printf("   average # in the queue .. = %6.2f\n", delay1 / departure);
  printf("   utilization ............. = %6.2f\n", service1 / departure);

  printf("For Job 2\n");

  printf("   average wait ............ = %6.2f\n", wait2 / index2);
  printf("   average delay ........... = %6.2f\n", delay2 / index2);
  printf("   average service time .... = %6.2f\n", service2 / index2);
  printf("   average # in the node ... = %6.2f\n", wait2 / departure);
  printf("   average # in the queue .. = %6.2f\n", delay2 / departure);
  printf("   utilization ............. = %6.2f\n", service2 / departure);

  printf("For All\n");

  printf("   average interarrival time = %6.2f\n", sum.interarrival / index);
  printf("   average wait ............ = %6.2f\n", sum.wait / index);
  printf("   average delay ........... = %6.2f\n", sum.delay / index);
  printf("   average service time .... = %6.2f\n", sum.service / index);
  printf("   average # in the node ... = %6.2f\n", sum.wait / departure);
  printf("   average # in the queue .. = %6.2f\n", sum.delay / departure);
  printf("   utilization ............. = %6.2f\n", sum.service / departure);

  return (0);
}
