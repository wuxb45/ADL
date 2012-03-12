#include <stdio.h>
#include <math.h>

int
main(int argc, char *argv[])
{
	int i;
	double r;

#define SIZE 256

	r= M_PI;
	for(i=0; i< SIZE; i++){
		r= r * (r + 2.34);
		while(r > 10.0)r-=10.0;
		

		printf("\top_set_F32(&($o[%d][0]), %f);\n",i, (i&1)?r:-r);
	}
	return 0;
}
