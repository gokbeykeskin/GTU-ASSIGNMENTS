#include <stdio.h>
#include <math.h>

int draw_triangle(int side1,int side2, int side3);
void type_triangle(int side1,int side2, int side3);
int perimeter_triangle(int side1,int side2, int side3);
double area_triangle(int side1,int side2, int side3,int perimeter);

void main(void)
{
	int s1,s2,s3; //sides of a triangle
	//get the inputs
	printf("The Length of the first side:\n");
	scanf("%d",&s1);
	printf("The Length of the second side:\n");
	scanf("%d",&s2);
	printf("The Length of the third side:\n");
	scanf("%d",&s3);
	printf("\n");
	if(draw_triangle(s1,s2,s3)==1) //if triangle can be drawn
	{
		printf("According to the triangle inequality theorem, this triangle can be drawn.\n\n");
		type_triangle(s1,s2,s3);
		printf("The perimeter of the triangle: %d\n",perimeter_triangle(s1,s2,s3));
		printf("The area of the triangle: %lf\n",area_triangle(s1,s2,s3,perimeter_triangle(s1,s2,s3)));
	}
	else //if triangle can't be drawn
		printf("According to the triangle inequality theorem, this triangle cannot be drawn.\n");
}


int draw_triangle(int side1,int side2,int side3)
{
	if(side1+side2>side3 && side1+side3>side2 && side2+side3>side1) //Triangle inequality theorem.
		return 1;
	return 0; 
}
void type_triangle(int side1,int side2, int side3)
{
	if(side1==side2 && side1==side3)
		printf("It is a Equilateral Triangle\n");
	else if(side1==side2 || side1==side3 || side2==side3)
		printf("It is a Isosceles Triangle\n");
	else
		printf("It is a Scalene Triangle\n");
}
int perimeter_triangle(int side1,int side2, int side3)
{
	int perimeter;
	perimeter= side1+side2+side3;
	return perimeter;
}
double area_triangle(int side1,int side2, int side3,int perimeter)
{
	double area,p;
	p = perimeter/2.0; 
	area = sqrt(p*(p-side1)*(p-side2)*(p-side3)); //area formula
	return area;
}
