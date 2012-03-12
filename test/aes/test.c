#include <stdio.h>

int main(int argc, char * argv[]){
	int i,j;
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			printf("\tlink %s%d%d = %s%d%d;\n",argv[1],i,j,argv[2],i,j);
		}
		printf("\n");
	}
	return 0;
}
