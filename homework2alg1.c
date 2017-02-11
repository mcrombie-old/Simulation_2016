#include <stdio.h>
#include <math.h>


const long m = 4294967161;
/*finds a full period multiplier given m*/
int main(void){

long a = 2;
long p = 1;
long x = a;

while (a < 100){
	x = a;
	p = 1;
	printf("a is at %ld\n", a);
	while (x != 1){
		p++;
		x = (a * x) % m;
	}

	if (p == (m - 1)){
		printf("a full period multiplier is...\n");
		printf("%d\n", a);
	}
	a++;



}
return 0;

}
