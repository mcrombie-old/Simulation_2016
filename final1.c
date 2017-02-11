#include <stdio.h>
#include <math.h>
#include "rngs.h"
#include "rvgs.h"

#define START 0.0
#define STOP 100000.0
#define SERVERS 4


/*The event list*/
typedef struct {
	double t;  //next event time
	int x;     //event status as 0 or 1
	int q;     //the number waiting in the queue
} event_list[SERVERS + 1];

/*generates the next arrival time with a stationary Poisson process with lambda = 4.0*/
double GetArrival(void){
	static double arrival = START;
	SelectStream(0);
	arrival += Exponential(0.25);
	return (arrival);
}

//generates the next service time
double GetService(int service_index){
	SelectStream(service_index);
	double service;
	if (service_index == 1) service = Erlang(4, 0.04);
	else if (service_index == 2) service = Erlang(6, 0.05);
	else if (service_index == 3) service = Erlang(5, 0.10);
	else if (service_index == 4) service = Erlang(3, 0.08);

	return (service);
}
	

//return the index of the next event type
int NextEvent(event_list event){
	int e;
	int i = 0;
	
	while (event[i].x == 0) i++; //finds index of the first active element
	e = i;

	while ( i < SERVERS){
		i++;
		if ((event[i].x == 1) && (event[i].t < event[e].t))
			e = i;
	}
	
	return (e);		
}


int main(void){
	struct {
		double current;
		double next;
	} t;

	event_list event;

	long number = 0; //the number in the node
	int event_index = 0; //sets the index
	int server_index;
	long job_index = 0; //counts processed jobs
	double area = 0.0; //time integrated number in the node

	struct{
		double serviceTimes;
		long served;
		long queue;
	} sum[SERVERS + 1];

	PlantSeeds(-1);
	t.current = START;
	event[0].t = GetArrival(); //gets first arrival time
	event[0].x = 1;  //server in use?


	
	//initializes t, x, and service trackers for each server
	for (server_index = 1; server_index <= SERVERS; server_index++){
		event[server_index].t = START; 
		event[server_index].x = 0; //not in use because all servers are initially idle
		event[server_index].q = 0;
		sum[server_index].serviceTimes = 0.0;
		sum[server_index].served = 0;
	}
	while ((event[0].x != 0) || (number != 0)) {
		event_index = NextEvent(event); //finds the index of the next event
		t.next = event[event_index].t; //sets the time to that of the next event
		area += (t.next - t.current) * number; //update integral
		//printf("%6.5f\n", (t.next - t.current) * number);
		t.current = t.next; //advances clock

		//arrival event
		if (event_index == 0){
			job_index++;
			number++;
			event[0].t = GetArrival();
			if (event[0].t > STOP){ //ends the loop if we go past STOP time
				event[0].x = 0;
			}
			server_index = 1; //goes straight into the first server
			double service = GetService(server_index);
			sum[server_index].serviceTimes += service;
			sum[server_index].served++;
			sum[server_index].queue++;
			event[server_index].t = t.current + service;
			event[server_index].x = 1;
			event[server_index].q++; 
		
		}
		else if (event_index == 1){
			event[event_index].q--;
			SelectStream(event_index + 5);
			long chance = Equilikely(1, 10); //determine the next server based on probabilities
			if (chance <= 6){
				 server_index = 2;
				 event[2].x = 1; //server 2 now in use
			}
			else { 
				server_index = 3;
				event[3].x = 1;
			}

			double service = GetService(server_index);
			sum[server_index].serviceTimes += service;
			sum[server_index].served++;
			sum[server_index].queue++;
			event[server_index].t = t.current + service;
			event[server_index].x = 1; 
			event[server_index].q++;
		}
		else if (event_index == 2) {
			SelectStream(event_index + 5);
			event[event_index].q--;
			long chance = Equilikely(1,10);
			if (chance <= 8){
				server_index = 4;
				event[server_index].x = 1; //server now in use

				double service = GetService(server_index);
				sum[server_index].serviceTimes += service;
				sum[server_index].served++;
				sum[server_index].queue++;
				event[server_index].t = t.current + service;
				event[server_index].x = 1; 
				event[server_index].q++;
			}
			else {

				number--; //leaves the system
			}
		}
		else if (event_index == 3) {
			SelectStream(event_index + 5);
			event[event_index].q--;

			long chance = Equilikely(1,10);
			if (chance <= 7){
				server_index = 4;
				event[4].x = 1;

				double service = GetService(server_index);


				sum[server_index].serviceTimes += service;
				sum[server_index].served++;
				sum[server_index].queue++;
				event[server_index].t = t.current + service;
				event[server_index].x = 1;
				event[server_index].q++;
			} 
			else{
				number--; //leaves the system
			}
		}
		else if (event_index == 4) {
			SelectStream(event_index + 5);
			event[event_index].q--;
			number--; //leaves the system
		}

		if (event_index != 0){
			if(event[event_index].q >=1){
				double service = GetService(event_index);
				event[event_index].t = t.current + service;
			}
			else{
				event[event_index].x = 0;
			}
		}

		
	
	}
	//printf("%6.5f\n", t.current/ job_index);
	
 	 printf("\nfor %ld jobs the service node statistics are:\n\n", job_index);
 	 printf("  avg interarrivals .. = %6.9f\n", event[0].t / job_index);
 	 printf("  avg wait ........... = %6.9f\n", area / job_index);
 	 printf("  avg # in node ...... = %6.9f\n", area / t.current);
	printf("  ecpected time in network...... = %6.9f\n", t.current / job_index);
	int s;
	for (s = 1; s <= SERVERS; s++)             //adjust area to calculate 
       area -= sum[s].serviceTimes;               //averages for the queue       

  printf("  avg delay .......... = %6.9f\n", area / job_index);
  printf("  avg # in queue ..... = %6.9f\n", area / t.current);

  printf("\nthe server statistics are:\n\n");
  printf("    server     utilization     avg service        share       jobs in queue\n");
  for (s = 1; s <= SERVERS; s++)
    printf("%8d %14.3f %15.2f %15.3f %15.3f\n", s, (sum[s].serviceTimes / t.current), (sum[s].serviceTimes / sum[s].served),
(double) sum[s].served / job_index, ((double)(sum[s].served / t.current)));// * job_index) / t.current);//,double) sum[s].queue / t.current);

  printf("\n");

  return (0);

}

