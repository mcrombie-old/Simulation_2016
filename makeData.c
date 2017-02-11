#include <stdio.h>
#include <math.h>
#include "rngs.h"

#define LAST 100000

double Uniform(double a, double b)           
/* --------------------------------------------
 * generate a Uniform random variate, use a < b 
 * --------------------------------------------
 */
{                                         
  return (a + (b - a) * Random());    
}
int main(void){
	PlantSeeds(12345554789);
	int index = 0;
	FILE *f = fopen("grades.txt", "w");

	while (index < LAST){
	index++;
	int array[120];
	int array2[12];
	int i,q,p;
	int totalQ = 120;

	int score = 0;
	
	for (i = 0; i < 90; i++){
		array[i] = 1;
	}

	for (i = 91; i < 120; i++){
		array[i] = 2;
	}
        
	int count = 0;
	for (i = 0; i < 12; i++){
		double temp = Uniform(0.0, totalQ);
		if (temp <= (90 - count)){
			array2[i] = 1;
			count++;
			totalQ--;
		}
		else {
			array2[i] = 2;
			totalQ--;
		}
	}
	
	/*int inc = 0;
	int dec = 0;
	for (q = 0; q < 12; q++){
		int num = Uniform(1 + inc ,121 - dec);
		
		array2[q] = array[num];	

		if (array2[q] == 1) inc++;
		else if (array2[q] == 2) dec++; //this replicates how the odds of selecting one is decreased when you've already selected one
	}*/
	
	for (p = 0; p < 12; p++){
		
		if (array2[p] == 1){
			double cheesey = Uniform(0.0, 1.0);
			if (cheesey < 0.1) score += 2;
			else if (cheesey < 0.4) score += 3;
			else score += 4;

		}
		else if (array2[p] == 2){
			double pickle = Uniform(0.0, 1.0);
			if (pickle < 0.1) score +=   0;
			else if (pickle < 0.5) score += 1;
			else if (pickle < 0.9) score += 2;
			else score += 3;
		}

	}
	


	
	fprintf(f, "%d\n", score);

	}
	fclose(f);

}

