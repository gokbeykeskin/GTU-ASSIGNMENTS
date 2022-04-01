#include <stdio.h>
#include <math.h>

int number_length(int number);
int find_digit(int number, int index);


void main(void)
{	int num,index;
	printf("Enter a number(maximum 6 digits):\n");
	scanf("%d",&num);
	printf("Your number has %d digits.\n\n",number_length(num));
	printf("When your number is written 100 times next to each other, which digit of this number would you like to see?\n");
	scanf("%d",&index);
	if(index>number_length(num)*100)
		printf("This index doesn't exist when your number writed 100 times");
	else
		printf("\n%d. number of the big number sequence: %d\n",index,find_digit(num,index));
}

int number_length(int number)
{
	int length;
	length=log10(number)+1;
	return length;
}

int find_digit(int number, int index)
{
	int n,digit;
	n = index%number_length(number); //the number will repeat itself after number_length times. 
	if(n==0)
		n=number_length(number);
	digit = number/pow(10,number_length(number)-n); 
	digit = digit%10;
	return digit;
}
