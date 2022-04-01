#include <stdio.h>

int sum(int n,int flag);
int mult(int n,int flag);

void main(void)
{
	int num,operation,oddev,check1,check2;
	printf("Enter an integer: ");
	scanf("%d",&num);
	printf("Please enter '0' for sum, '1' for multiplication\n");
	scanf("%d",&operation);
	printf("Please enter '0' to work on even numbers,'1' to work on odd numbers\n");
	scanf("%d",&oddev);
	
	switch(operation){
		case 0:
			if(oddev==1 || oddev==0)
				printf("= %d\n",sum(num,oddev));
			else
				printf("Invalid value for odd/even selection.");
			break;
		case 1:
			if(oddev==1 || oddev==0)
				printf("= %d\n",mult(num,oddev));
			else
				printf("Invalid value for odd/even selection.");
			break;
		default:
			printf("Unsupported operation.");
			break;
	}
}


int sum(int n,int flag){
	int sum=0;
	if(flag==0){
		for (int i=2;i<=n;i+=2){
			if(i==2)
				printf("%d",i);
			else
				printf("+ %d ",i);
			sum+=i;
		}
	}
	else if(flag==1){
		for(int i=1;i<=n;i+=2){
			if(i==1)
				printf("%d",i);
			else
				printf("+ %d ",i);
			sum+=i;
		}
	}
	return sum;
}

int mult(int n,int flag){
	int mult=1;
	if(flag==0){
		for(int i=2;i<=n;i+=2){
			if(i==2)
				printf("%d",i);
			else
				printf("* %d ",i);
			mult*=i;
		}
	}
	else if(flag==1){
		for(int i=1;i<=n;i+=2){
			if(i==1)
				printf("%d",i);
			else
				printf("* %d ",i);
			mult*=i;
			
		}
	}
	return mult;
}
