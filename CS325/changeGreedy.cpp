#include <stdio.h>
#include <stdlib.h>

using namespace std;

int changeGreedy(int* v, int* c, int length, int A){
	int i = 0, m = 0;

	for(i = length; i >= 0; i--){
		while(A - v[i] >= 0){
			//printf("A = %d\n",A);//DEBUG line
			c[i]++;
			A -= v[i];
			m++;
		}
	}

return m;
	
}


int main(){
	int v[4] = {1,3,9,27}; //coin denomination
	int c[4] = {0,0,0,0};  //amount of coins needed
	int i = 0, m;
	int arrayLen = sizeof(v)/sizeof(int);

	m = changeGreedy(v, c, arrayLen-1, 109);

	printf("Total coins: %d\n",m);

	for(i = 0; i < sizeof(c)/sizeof(int); i++){
		printf("c[%d] is %d, ",i,c[i]);
	}
	printf("\n");
}
