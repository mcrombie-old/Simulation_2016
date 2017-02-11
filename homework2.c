
    #include <stdio.h>
    #include <math.h>

    #define m 4294967161
    #define a 58

    const long q = m / a;
    const long r = m % a;


    long gcd(long z, long y){
        long c;
        while (z != 0){
            c=z; z = y % z; y = c;
        }
        return y;
    }

    /* Algorithm 2.2.1 finds g(x) = ax mod m*/
    long g(long x)
    {

        long t = a * (x % q) - r * (x / q);
        if (t>0){
            return (t);
        }
        else{
            return (t + m);
        }
    }


    /*Algorithm 2.2.2 Given the prime modulus, m, and any associated full-period, modulus-compatible multiplier, a, the following algorithm generates all      the full-period modulus-compatible multipliers relative to m.*/
    int main(void)
    {


    long  i = 1;
    long x = a;
    long count = 0;
    FILE *f = fopen("DamnitCooper.txt","w");

    while (x != 1){
        if ((m % x < m / x) && (gcd(i, m - 1) == 1)){
            //printf("%lu is full-period and modulo compatible\n", x);
	    fprintf(f, "%ld\n", x);
            count++;
        }
        i++;
        x = g(x);
	//printf("%ld\n", x);
    }
    printf("%d\n", count);
    printf("%ld  %ld\n", m / a, m % a);
    fclose(f);

    return (0);
    }
