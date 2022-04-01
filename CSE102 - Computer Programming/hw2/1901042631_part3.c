#include <stdio.h>

#define TRY_TO_EURO 6.69
#define TRY_TO_USD 6.14
#define EURO_TO_USD 0.918

int main()
{
	double amount,temp;
	int selection1,selection2;
	printf("***** Welcome to ABC Exchange Office *****\n");
	printf("Enter your amount:\n");
	scanf("%lf",&amount);

	printf("Please select your currency\n1. Turkish Lira\n2. Euro\n3. Dollar\n\n");
	scanf("%d",&selection1);
	switch(selection1)
	{
		case 1:
			printf("You have %lf Turkish Liras\n",amount);
			printf("Choose which currency you want to convert\n");
			scanf("%d",&selection2);
			switch(selection2)
			{
				case 1:
					printf("You have %lf Turkish Liras\n",amount);
					break;
				case 2:
					printf("You have %lf Euros\n",amount/TRY_TO_EURO);
					break;
				case 3:
					printf("You have %lf Dollars\n",amount/TRY_TO_USD);
					break;
				default:
					printf("Your selection is invalid.\n");
					break;
			}
		break;
		case 2:
			printf("You have %lf Euros\n",amount);
			printf("Choose which currency you want to convert\n");
			scanf("%d",&selection2);
			switch(selection2)
			{
				case 1:
					printf("You have %lf Turkish Liras\n",amount*TRY_TO_EURO);
					break;
				case 2:
					printf("You have %lf Euros\n",amount);
					break;
				case 3:
					printf("You have %lf Dollars\n",amount/EURO_TO_USD);
					break;
				default:
					printf("Your selection is invalid.\n");
					break;
			}
			break;
		case 3:
			printf("You have %lf Dollars\n",amount);
			printf("Choose which currency you want to convert\n");
			scanf("%d",&selection2);
			switch(selection2)
			{
				case 1:
					printf("You have %lf Turkish Liras\n",amount*TRY_TO_USD);
					break;
				case 2:
					printf("You have %lf Euros\n",amount*EURO_TO_USD);
					break;
				case 3:
					printf("You have %lf Dollars\n",amount);
					break;
				default:
					printf("Your selection is invalid.\n");
					break;
			}
			break;
			default:
			printf("Your selection is invalid.\n");
			break;
	}
}