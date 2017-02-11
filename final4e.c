#include <stdio.h>
#include <math.h>
#include "rngs.h"
#include "rvgs.h"

#define START 0.0
#define STOP 2000


/*double * slope(int t[], double arrival[]){
	int i;
	double slopes[6];
	for(i = 0; i < 6; i++){
		slopes[i] = (arrival[i+1] - arrival[i]) / (t[i+1] - t[i]);
	}

	return slopes;
}*/	

/*double inverse_function(double u, int j){
	return t[j] + ((2 * (u - event_rates[j]))/(arrival[j] + sqrt(pow(arrival[j],2) + (2 * slopes[j] * (u - event_rates[j]))));
}*/

int main(void){

	int t[7] = {0, 200, 600, 1000, 1200, 1500, 2000}; //arrival time
	double arrival[7] = {0.5,0.5,0.9,0.9,1.2,0.9,0.5}; //arrival rate

	//double *slopes;
	//slopes = slope(t, arrival);
	double slopes[6] = {0, 0.001, 0, 0.0015, -0.001, -0.0008};


	int event_rates[7] = {0, 100, 540, 900, 1440, 1350, 1000};

	

	double a = 0.0;
	double u = 0.0;
	int n = 0;
	int j = 0;
	int k = 7; //the constant number of points

	while (a < STOP){
		u += Exponential(1.0);
		while ((event_rates[j + 1] < u) && (j < k)){
			j++;
		}
		a =  t[j] + ((2 * (u - event_rates[j]))/(arrival[j] + sqrt(pow(arrival[j],2) + (2 * slopes[j] * (u - event_rates[j])))));
		printf("%f\n", a);

		n++;
	}
	



}
