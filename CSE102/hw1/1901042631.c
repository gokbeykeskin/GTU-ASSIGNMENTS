#include <stdio.h>
#include <math.h>

void find_root();
void find_newtonian_root(); 
int find_multiple_closest(int a, int b);

void main(void)
{
	// PART 1
	find_root();
	printf("-----------------------------------------------------------\n");
	//PART 2
	find_newtonian_root();
	printf("-----------------------------------------------------------\n");
	//PART 3
	int num1,num2;
	printf("Enter the first integer: ");
	scanf("%d",&num1);
	printf("Enter the second integer: ");
	scanf("%d",&num2);
	if(num1%num2==0) //there is no need the find the closest in this case because closest one is itself.
	{
		printf("Closest number to %d that is multiple of %d is itself.",num1,num2);
	}
	else //find the closest one using function.
	{
		printf("Closest number to %d that is multiple of %d is %d.",num1,num2,find_multiple_closest(num1,num2));
	}
}

void find_root()
{
	int a,b,c; //coefficients of the second degree equation.
	printf("Please Enter the first coefficient: ");
	scanf("%d",&a);
	printf("Please Enter the second coefficient: ");
	scanf("%d",&b);
	printf("Please Enter the third coefficient: ");
	scanf("%d",&c);

	int delta; // delta is the discriminant for 2nd degree equations.
	float root1,root2;
	delta=(b*b)-(4*a*c);
	root1 = (-b + sqrt(delta))/2*a; //root formula 1
	root2 = (-b - sqrt(delta))/2*a; // root formula 2

	if(delta<0)
	{
		printf("Your equation %dx^2 %+dx %+d does not have any real roots. \n",a,b,c);
	}
	else if(delta==0)
	{
		printf("Your equation %dx^2 %+dx %+d has one real root {%.1f}\n",a,b,c,root1);
	}
	else
	{
		printf("Your equation %dx^2 %+dx %+d has real roots {%.1f,%.1f}\n",a,b,c,root1,root2);
	}
}

void find_newtonian_root()
{

	//first part is getting inputs
	float cef,ces,cet,ig; // ig is initial guess, ce means coefficient (first,second,third)
	printf("Please Enter the first coefficient: ");
	scanf("%f",&cef);
	printf("Please Enter the second coefficient: ");
	scanf("%f",&ces);
	printf("Please Enter the third coefficient: ");
	scanf("%f",&cet);
	printf("Please Enter the initial: ");
	scanf("%f",&ig);

	//second part is finding root with regular formulas to find the difference at every cycle
	int delta; // delta is the discriminant for 2nd degree equations.
	float root,root1,root2,differ1,differ2; //difference 1 and 2 are the differences between inital guess and actual root(s).
	delta=(ces*ces)-(4*cef*cet);
	root1 = (-ces + sqrt(delta))/2*cef;
	root2 = (-ces - sqrt(delta))/2*cef;
	differ1=ig-root1;
	differ2=ig-root2;
	fabs(differ1); 	//took absolute value for comparison of differences.
	fabs(differ2);	//took absolute value for comparison of differences.
	if(differ1<=differ2) //determine which root is closer to initial guess.
	{
		root=root1;
	}
	else
	{
		root=root2;
	}
	printf("Your equation is %.0fx^2%+.0fx%+.0f and iterations are\n\n",cef,ces,cet);
	printf("Step\t  x\t f(x)\tDifference\n\n");
	float fx;
	//apply the newtonian method 5 times
	ig=(ig-((cef*ig*ig+ces*ig+cet)/(2*cef*ig+ces))); //newtonian method formula x(n+1) = xn - (f(x) / f'(x))
	fx=cef*ig*ig+ces*ig+cet;
	printf("x%d\t%.4f\t%.4f\t %.4f\n",1,ig,fx,ig-root); // step,x,f(x),difference
		
	ig=(ig-((cef*ig*ig+ces*ig+cet)/(2*cef*ig+ces))); //newtonian method formula x(n+1) = xn - (f(x) / f'(x))
	fx=cef*ig*ig+ces*ig+cet;
	printf("x%d\t%.4f\t%.5f\t %.4f\n",2,ig,fx,ig-root); // step,x,f(x),difference
	
	ig=(ig-((cef*ig*ig+ces*ig+cet)/(2*cef*ig+ces))); //newtonian method formula x(n+1) = xn - (f(x) / f'(x))
	fx=cef*ig*ig+ces*ig+cet;
	printf("x%d\t%.4f\t%.5f\t %.4f\n",3,ig,fx,ig-root); // step,x,f(x),difference
		
	ig=(ig-((cef*ig*ig+ces*ig+cet)/(2*cef*ig+ces))); //newtonian method formula x(n+1) = xn - (f(x) / f'(x))
	fx=cef*ig*ig+ces*ig+cet;
	printf("x%d\t%.4f\t%.5f\t %.4f\n",4,ig,fx,ig-root); // step,x,f(x),difference
		
	ig=(ig-((cef*ig*ig+ces*ig+cet)/(2*cef*ig+ces))); //newtonian method formula x(n+1) = xn - (f(x) / f'(x))
	fx=cef*ig*ig+ces*ig+cet;
	printf("x%d\t%.4f\t%.5f\t %.4f\n",5,ig,fx,ig-root); // step,x,f(x),difference
}

int find_multiple_closest(int a, int b)
{
	int c,mclosestlow,mclosestup;
	c=a/b;
	mclosestlow= c*b; //lower multiple of b
	mclosestup= (c+1)*b; // upper multiple of b
	if(a-mclosestlow < mclosestup-a) //determine which one is closer to a
	{
		return mclosestlow;
	}
	else
	{
		return mclosestup;
	}
}
