
#include <stdio.h>
#include <math.h>
#include "rngs.h"

#define LAST       1000         /* number of replications */
#define HALF_PI (2.0 * atan(1.0))          /* 1.5707963...           */
#define R       1.0                        /* length of the needle   */

typedef int bool;
#define true 1
#define false 0

   double Uniform(double a, double b)      
/* --------------------------------------------
 * generate a Uniform random variate, use a < b 
 * --------------------------------------------
 */
{
  return (a + (b - a) * Random());
}

/*used for qsort*/
int cmpfun(const void * a, const void * b){
	return ( *(int*)a - *(int*)b );
}

int main(void){
 
  int guess;
  int actual;
  int index = 0;
  double mean = 0;
  int winners = 0;
  FILE *f = fopen("pick3.txt", "w"); //output file for creating histogram later
  
  PlantSeeds(12345);
  while(index < LAST){
	index++;
	int i;
	int caching = 365; //money kept/won
	for (i = 0; i < 365;i++){
		//randomizes player's guess and the actual number
		guess = Uniform(0.0,1.0) * 1000;
		actual = Uniform(0.0,1.0) * 1000;
		
		//makes sure the player uses none of the same digits
		int checkArray[3];
		bool run = true;
		int t;
		int checkGuess = guess;
		while(run){
			for (t = 0; t < 3; t++){
				checkArray[t] = checkGuess % 10;	
				checkGuess/=10;	
			}
			if (checkArray[0] != checkArray[1] && checkArray[0] != checkArray[2] && checkArray[1] != checkArray[2]){
				run = false;
			}else{
				guess = Uniform(0.0,1.0) * 1000;
				checkGuess = guess;
			}
		}	
		int j;
		int gArray[3];
		int aArray[3];
		for (j = 0; j < 3; j++){
			gArray[j] = guess % 10;	
			aArray[j] = actual % 10;
			guess/=10;
			actual/=10;	
		}
		qsort(gArray, 3, sizeof(int),cmpfun);
		qsort(aArray, 3, sizeof(int),cmpfun);

 		if (gArray[0] == aArray[0] && gArray[1] == aArray[1] && gArray[2] == aArray[2]){
			caching += 79;
		}else{
			caching -= 1;
		}
	}
	fprintf(f, "%d\n", caching);
	mean += caching;	
	if (caching > 320) winners++;

  }
  fclose(f);
  mean = mean/LAST;
  double winners_proportion = (double) winners/(double)LAST;
  printf("Average - %f\n", mean);
  printf("Chances of winning - %f\n", winners_proportion);

  return (0);
}
