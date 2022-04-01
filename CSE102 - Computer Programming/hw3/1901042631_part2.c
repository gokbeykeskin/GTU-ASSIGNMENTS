#include <stdio.h>
#include <math.h>
int isprime(int a);

void main(void){
	int num;
	printf("Please enter an integer: ");
	scanf("%d",&num);
	for(int i=2;i<num;i++){
		if(isprime(i)==1)
			printf("%d is a prime number\n",i);
		else
			printf("%d is not a prime number it is dividible by %d\n",i,isprime(i));

	}
}

int isprime(int a){
	int divider;
	for(int i=2;i<=sqrt(a);i++){
		if(a%i==0)
			return i;
	}
	return 1;
}