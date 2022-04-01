#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define fpath "out.eps"


typedef struct point{
	double x,y;
}Point2D;

typedef struct color{
	double red,green,blue;
}Color;

typedef struct figure{
	double width, height, thickness, resolution;
	Color c;
	Point2D *pointsfunc;
	int pointAmount;
}Figure;

Figure * start_figure(double width, double height);
void set_thickness_resolution(Figure *fig,double thickness,double resolution);
void draw_fx(Figure * fig, double f(double x),double start_x, double end_x,double step_size);
void draw_polyline(Figure * fig,Point2D * poly_line, int n);
void export_eps(Figure * fig, char * file_name);
double f(double x);
double f2(double x);
double f3(double x);
void main(){
	Figure * fig;
	fig=start_figure(100,100);

	Point2D pts[5];
	pts[0].x=5;
	pts[0].y=5;
	pts[1].x=5;
	pts[1].y=25;
	pts[2].x=25;
	pts[2].y=25;
	pts[3].x=25;
	pts[3].y=5;
	pts[4].x=5;
	pts[4].y=5;
	
	set_thickness_resolution(fig,0.2,0.1);

	/*UNCOMMENT ONE AT A TIME*/
	//draw_fx(fig,f,-100,100,0.1);
	//draw_fx(fig,f2,-100,100,0.1);
	draw_fx(fig,f3,-100,100,0.1);
	//draw_polyline(fig,pts,5);

	export_eps(fig,fpath);
}

Figure * start_figure(double width, double height){
	Figure *fig;
	fig=(Figure*)malloc(sizeof(Figure));
	fig->width=width;
	fig->height=height;
	return fig;
}

void set_thickness_resolution(Figure *fig,double thickness,double resolution){
	fig->thickness=thickness;
	fig->resolution=resolution;
}

void set_color(Figure * fig, Color c){
	fig->c=c;
}

void draw_fx(Figure * fig, double f(double x),double start_x, double end_x,double step_size){
	int k=0;
	double i,a=step_size;
	fig->pointsfunc=(Point2D *)malloc(sizeof(Point2D));
	for(i=start_x;i<end_x;i+=step_size){
		if(fig->resolution<sqrt(pow(i+a-i,2)+pow(f(i+a)-f(i),2))){
			fig->pointsfunc[k].x = i;
			fig->pointsfunc[k].y = f(i);
			k++;
			fig->pointsfunc=(Point2D *)realloc(fig->pointsfunc,(k+1)*sizeof(Point2D));
			a=step_size;
		}
		else{
			a+=step_size;
		}
	}
	fig->pointAmount=k;
}

void draw_polyline(Figure *  fig,Point2D * poly_line, int n){
	int i;
	fig->pointsfunc=(Point2D *)malloc(sizeof(Point2D));
	for(i=0;i<n;i++){
		fig->pointsfunc[i]=poly_line[i];
		fig->pointsfunc=(Point2D *)realloc(fig->pointsfunc,(i+2)*sizeof(Point2D));
	}
	fig->pointAmount=n;
}

void export_eps(Figure * fig, char * file_name){
	int i;
	FILE *fp;
	fp=fopen(file_name,"w");
	fprintf(fp,"%%!PS-Adobe-3.0 EPSF-3.0\n");
	fprintf(fp,"%%%%BoundingBox: %lf %lf %lf %lf\n",-(fig->width)/2,-(fig->height)/2,(fig->width)/2,(fig->height)/2);
	fprintf(fp,"%lf setlinewidth\n",fig->thickness);
	fprintf(fp,"newpath\n");
	fprintf(fp,"%lf %lf moveto\n",fig->pointsfunc[0].x,fig->pointsfunc[0].y);
	for(i=1;i<fig->pointAmount;i++){
		fprintf(fp,"%lf %lf lineto\n",fig->pointsfunc[i].x,fig->pointsfunc[i].y);
	}
	fprintf(fp,"stroke\nshowpage\n");
}

double f(double x){
	return sin(x);
}

double f2(double x){
	return x*x;
}

double f3(double x){
	return x*x*x;
}
