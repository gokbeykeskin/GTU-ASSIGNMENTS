#include <stdio.h>

int andop (int a, int b);
int isBin (int a, int b); //this function checks both if the numbers are binary and if the numbers have the same length.
 
void main(void){
	int flag1=1,num1,num2,temp1,temp2;
	while(flag1!=0){
		printf("First Integer: ");
		scanf("%d",&num1);
		printf("Second Integer: ");
		scanf("%d",&num2);
		if(isBin(num1,num2)==1)
			printf("Integers should be binary,please enter 2 new integers.\n");
		else if(isBin(num1,num2)==2)
			printf("Integers should have the same length,please enter 2 new integers.\n");
		else{
			printf("%d AND %d = %d\n",num1,num2,andop(num1,num2));
			flag1=0;
		}
	}
}
int isBin (int a, int b){
	int flag=1,temp1,temp2,bin=0;
	while(flag!=0){
		temp1=a%10;
		temp2=b%10;
		if((temp1!=1 && temp1!=0)||(temp2!=1 && temp2!=0)){
			bin=1;
			flag=0;
		}
		else if((a==0 && b!=0)||(a!=0 && b==0)){
			flag=0;
			bin=2;
		}
		else if(a==0&&b==0)
			flag=0;
		a=a/10;
		b=b/10;
	}
	return bin;
}

int andop (int a, int b){
	int result=1,digit=1,temp=b,border;
	while(temp!=1){
		temp=temp/10;
		result*=10; //this operation finds the digit number of the result
	}
	while(a!=1){
		border = a%10+b%10;
		if(border==2)
			result+=digit; //this adds the digits to the numbers
		a=a/10;
		b=b/10;
		digit*=10; //next digit
	}
	return result;
}