#include <stdio.h>
#include <math.h>

int
main(int argc, char ** argv)
{
	double i,j;
	double x,y;
	for(i = 1.0; i<4096.0; i*=2.0){
		for(j = 0.0; j<i; j+=1.0){
			x = cos(2.0 * M_PI * j /i);
			y = sin(-2.0 * M_PI * j/i);
			printf("net u%.0f.w[0], %.16f;\n", j,x);
			printf("net u%.0f.w[1], %.16f;\n", j,y);
		}
		printf("\n");
	}
	return 0;
}


	
