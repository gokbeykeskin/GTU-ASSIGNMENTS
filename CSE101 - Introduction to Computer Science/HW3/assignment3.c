#include <stdio.h>
float addition();
float subtraction();
float multiplication();
float division();
float powern();
float avarage();
float maxnum();
float main()
{
	float a,cont;
	while (a!=0)
	{	
		printf("WELCOME GTU CALCULATOR MACHINE\nSTUDENT NAME: GÖKBEY GAZİ KESKİN\nPLEASE SELECT FROM THE FOLLOWING\nMENU:\n(1)ADD TWO NUMBERS\n(2)SUBTRACT TWO NUMBERS\n(3)MULTIPLY TWO NUMBERS\n(4)DIVIDE TWO NUMBERS\n(5)TAKE NTH POWER OF A NUMBER\n(6)FIND AVARAGE OF NUMBERS INPUTTED\n(7)FIND THE MAXIMUM OF NUMBERS INPUTTED\n(0)EXIT\n");
		scanf("%f",&a);
		if(a==1)
		{
			printf("-------------ANSWER IS: %f-------------\n",addition());
		}
		else if(a==2)
		{
			printf("-------------ANSWER IS: %f -------------\n",subtraction());
		}
		else if(a==3)
		{
			printf("-------------ANSWER IS: %f -------------\n",multiplication());
		}
		else if(a==4)
		{
			printf("-------------ANSWER IS: %f -------------\n",division());
		}
		else if(a==5)
		{
			printf("-------------ANSWER IS: %f -------------\n",powern());
		}
		else if(a==6)
		{
			printf("-------------ANSWER IS: %f -------------\n",avarage());
		}
		else if(a==7)
		{
			printf("-------------ANSWER IS: %f -------------\n", maxnum());
		}
		else if(a!=0) 
		{
			/*
			this line is for if the user enters for example 9 the program will give the
			following error but if the user enters 0 the program won't because 0 is one
			of the given numbers */
			printf("-------------YOU CAN ONLY ENTER THE GIVEN NUMBERS-------------\n");
		}
		
	}
		printf("Thanks for using the program\n");
		return(0);
}
float addition()
{
	float a,b;
	printf("INPUT NUMBER ONE: ");
	scanf("%f",&a);
	printf("INPUT NUMBER TWO: ");
	scanf("%f", &b);
	return(a+b);
	}

float subtraction()
{
	float a,b;
	printf("INPUT NUMBER ONE: ");
	scanf("%f",&a);
	printf("INPUT NUMBER TWO: ");
	scanf("%f", &b);
	return(a-b);
}

float multiplication()
{
	float a,b;
	printf("INPUT NUMBER ONE: ");
	scanf("%f",&a);
	printf("INPUT NUMBER TWO: ");
	scanf("%f", &b);
	return(a*b);
}

float division()
{
	float a,b;
	printf("INPUT NUMBER ONE: ");
	scanf("%f",&a);
	printf("INPUT NUMBER TWO: ");
	scanf("%f", &b);
	return(a/b);
}

float powern()
{
	float a,b,result=1;
	printf("INPUT BASE NUMBER: ");
	scanf("%f",&a);
	printf("INPUT POWER NUMBER: ");
	scanf("%f", &b);
	for(int i=0;i<b;i++)
	{
		result = a*result;
	}
	return(result);
}

float avarage()
{
	float num=3,a=-1,temp = 0,ans,quit;
	char c;
	
	printf("PRESS q TO QUIT\n");
	while(c != 'q')
	{

		printf("Enter a number: ");
		scanf("%f", &num);
		temp = num+temp;
		num =0;
		a++;
		c=getchar();
	}
	
	ans= temp/a;
	return(ans);
}

float maxnum()
{
	float nummax=0,temp=0;
	char quiter;
	printf("PRESS q TO QUIT\n");
	while(quiter!='q')
	{	
		printf("Enter a number: ");
		scanf("%f",&nummax);
		if(nummax>temp)
		{
			temp = nummax;
		}
		quiter =getchar();
	}
	return(temp);
}