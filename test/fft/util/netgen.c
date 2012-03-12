#include <stdio.h>
#include <math.h>
int
main(int argc, char ** argv)
{
	int i;
	double j;
	double x,y;


#define SIZE 256
#define SIZEF (256.0)
	for(i = 0; i < SIZE; i++){
		printf("\tnet i[0x%02x], s%1x.i[0x%02x];\n", i, i&1, i>>1);
	}
	printf("\n");

	for(j = 0.0; j<SIZEF/2; j+=1.0){
		x = cos(2.0 * M_PI * j /SIZEF);
		y = sin(-2.0 * M_PI * j/SIZEF);
		printf("\tnet u%02x.w[0], %.16f;\n", (int)j,x);
		printf("\tnet u%02x.w[1], %.16f;\n", (int)j,y);
	}
	printf("\n");

	for(i = 0; i < SIZE; i++){
		printf("\tnet u%02x.i[0x%0x], s%x.o[0x%02x];\n", i>>1, i&1, i&1, i>>1);
	}
	printf("\n");

	for(i = 0; i < SIZE; i++){
		printf("\tnet o[0x%02x], u%02x.o[0x%02x];\n", i, i%(SIZE/2), i/(SIZE/2));
	}
	printf("\n");


	return 0;


}

