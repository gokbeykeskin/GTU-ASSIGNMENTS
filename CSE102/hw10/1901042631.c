#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define PI 3.141592654
#define FILEPATH "commands.txt"


typedef struct point{ //
	char name[3];
	double x,y;
}pt;

typedef struct line{
	char name[4]; //example: line made by point 1 and 2 will be named l12.
	pt p1;
	pt p2;
}ln;

typedef struct polygon{
	char name[4];
	pt corners[20];
}pg;

void createPoint(double x,double y,char object[4],pt *point);
void createLine(pt point1,pt point2,char object[4],ln *line);
void createPolygon(pt points[20],char object[4],int cornerAmount,pg *polygon);
double DistancePtoP(pt *p1,pt *p2); // the distance between two points.
double DistancePtoL(pt *p,ln *l); // the distance between a point and a line.
double AngleLtoL(ln *l1,ln *l2); // the angle between two lines.
double LengthL(ln *l); //the length of a line.
double LengthPG(pg *p,int cornerAmount); //the length(circumference) of a polygon.
double AreaPG(pg *p,int cornerAmount); //the area of a polygon.
void main(){
	char temp[200],junk[500]; //temp gets a whole line each time.
	int commandAmount,i=0,j=0,m=0,n=0,k,l,cornerAmount,ctr; //I used lots of local counters. 
	double datas1[20],datas2[20];  //holds the x y values, point indexes etc.
	int actionDatas1[20],actionDatas2[20]; //holds the x y values, point indexes etc.
	char polydata[20][4]; 
	int polycrn[20]; //holds the indexes of the points which will construct a polygon.
	char polyln[20][4]; //holds the name of the lines which will construct a polygon.
	pt polypts[20]; //holds the points which will construct a polygon.
	char objects[20][4]; //holds the name of the objects
	pt points[20]; //holds the created points.
	ln lines[20]; //holds the created lines.
	char wantedLine1[4],wantedLine2[4]; //used these for string variables such as L12 L23...
	pg polygons[20]; //holds the created polygons.
	int flag;
	FILE *fp,*fp2;
	char outputFile[20];
	char action[20];
	fp= fopen(FILEPATH,"r");
	fgets(junk,100,fp); //takes the action line to junk
	fscanf(fp,"%d",&commandAmount);
	while(i<=commandAmount){
		fgets(temp,100,fp); //gets a whole line each time.
		if(temp[0]!='P'&& temp[0]!='L'){ //if the command doesn't wants a line or a polygon.
			sscanf(temp,"%lf %lf %s %[^\n]",&datas1[i],&datas2[i],objects[i],junk);
		}
		else if(temp[0]=='P'){ //if it wants a line or a polygon with points.
			flag=0;
			for(int k=0;k<100;k++){
				if(temp[k]=='G') flag=1; //if its a polygon
			}
			if(flag==0){ //if it's a line
				sscanf(temp,"%c %lf %c %lf %s %[^\n]",&junk[0],&datas1[i],junk,&datas2[i],objects[i],junk);
			}
			if(flag==1){ //if its a polygon
				k=0;
				l=0;
				do{
					sscanf(temp+l,"%s",polydata[k]);
					sscanf(polydata[k],"%c %d",&junk[0],&polycrn[k]);
					k++;
					l+=3;
				}while(strncmp(polydata[k-1],"PG",2)!=0);
				ctr=k-2;
				cornerAmount=k-1;
				strcpy(objects[i],polydata[k-1]); 
				while(ctr>=0){
					polypts[ctr]=points[polycrn[ctr]-1];
					ctr--;
				}
			}
		}
		else if(temp[0]=='L'){
			k=0;
			l=0;
			do{
				sscanf(temp+l,"%s",polyln[k]);
				k++;
				l+=4;
			}while(strncmp(polyln[k-1],"PG",2)!=0);
			cornerAmount=k-1;
			strcpy(objects[i],polyln[k-1]);
			for(l=0;l<20;l++){
				if(strncmp(lines[l].name,polyln[l],3)==0){
					polypts[l].x=lines[l].p1.x;
					polypts[l].y=lines[l].p1.y;
				}
			}
		}

		if(strncmp(objects[i],"PG",2)==0){
			createPolygon(polypts,objects[i],cornerAmount,&polygons[m]);
			m++;
		}
		else if(objects[i][0]=='P'){
			createPoint(datas1[i],datas2[i],objects[i],&points[n]);
			n++;
		}
		else if(objects[i][0]=='L'){
			createLine(points[(int)datas1[i]-1],points[(int)datas2[i]-1],objects[i],&lines[j]);
			j++;
		}
		i++;
	}
	while(strncmp(junk,"actions",7)!=0){
		fgets(junk,100,fp);
	}
	fscanf(fp,"%s",outputFile);
	fscanf(fp,"%d",&commandAmount);
	fp2=fopen(outputFile,"w");
	i=0;
	while(i<commandAmount){
		flag=0;
		fscanf(fp,"%s",action);
		if(strncmp(action,"Distance",8)==0){
			fgets(temp,100,fp);
			for(k=0;k<strlen(temp);k++){
				if(temp[k]=='L'){
					flag=1;
				}
			}
			if(flag==0){ //if it's the distance between two points.
				sscanf(temp," %c %d %c %d",&junk[0],&actionDatas1[i],&junk[0],&actionDatas2[i]);
				fprintf(fp2,"Distance(%s,%s)=%lf\n",points[actionDatas1[i]-1].name,points[actionDatas2[i]-1].name,DistancePtoP(&points[actionDatas1[i]-1],&points[actionDatas2[i]-1]));
			}
			else if(flag==1){
				sscanf(temp," %c %d %s",&junk[0],&actionDatas1[i],wantedLine1);
				for(k=0;k<20;k++){
					if(strncmp(lines[k].name,wantedLine1,3)==0){
						if(isnan(DistancePtoL(&points[actionDatas1[i]-1],&lines[k]))==0)
							fprintf(fp2,"Distance(%s,%s)=%lf\n",points[actionDatas1[i]-1].name,lines[k].name,DistancePtoL(&points[actionDatas1[i]-1],&lines[k]));
						else
							fprintf(fp2,"Distance(%s,%s)=%lf\n",points[actionDatas1[i]-1].name,lines[k].name,(lines[k].p1.x)-(points[actionDatas1[i]-1].x)); //slope of the line aproaches infinity.
					}
				}
			}
		}
		else if(strncmp(action,"Angle",5)==0){
			fgets(temp,100,fp);
			sscanf(temp," %s %s",wantedLine1,wantedLine2);
			for(k=0;k<20;k++){
				for(l=0;l<20;l++){
					if(strncmp(lines[k].name,wantedLine1,3)==0 && strncmp(lines[l].name,wantedLine2,3)==0){
						if(isnan(AngleLtoL(&lines[k],&lines[l]))==0)
							fprintf(fp2,"Angle(%s,%s)=%lf\n",lines[k].name,lines[l].name,AngleLtoL(&lines[k],&lines[l]));
						else
							fprintf(fp2,"Angle(%s,%s)=0 //They are parallel\n",lines[k].name,lines[l].name);
					}
				}
			}
		}
		else if(strncmp(action,"Length",6)==0){
			fgets(temp,100,fp);
			sscanf(temp," %s",wantedLine1);
			if(strncmp(wantedLine1,"PG",2)==0){
				for(k=0;k<20;k++){
					if(strncmp(polygons[k].name,wantedLine1,3)==0)
						fprintf(fp2,"Length(%s)=%lf\n",polygons[k].name,LengthPG(&polygons[k],cornerAmount));
				}
			}
			else if(strncmp(wantedLine1,"L",1)==0){
				for(k=0;k<20;k++){
					if(strncmp(lines[k].name,wantedLine1,2)==0)
						fprintf(fp2,"Length(%s)=%lf\n",lines[k].name,LengthL(&lines[k]));
				}
			}
		}
		else if(strncmp(action,"Area",4)==0){
			fscanf(fp,"%s",wantedLine1);
			for(k=0;k<20;k++){
				if(strncmp(polygons[k].name,wantedLine1,3)==0){
					fprintf(fp2,"Area(%s)=%lf\n",polygons[k].name,AreaPG(&polygons[k],cornerAmount));
				}
			}
		}
		i++;
	}
	fclose(fp);
	fclose(fp2);
}

void createPoint(double x,double y,char object[3],pt *point){
	point->x=x;
	point->y=y;
	strcpy(point->name,object);
}

void createLine(pt point1,pt point2,char object[4],ln *line){
	line->p1=point1;
	line->p2=point2;
	strcpy(line->name,object);
}

void createPolygon(pt points[20],char object[4],int cornerAmount,pg *polygon){
	int i;
	for(i=0;i<cornerAmount;i++){
		polygon->corners[i]=points[i];
	}
	strcpy(polygon->name,object);
}

double DistancePtoP(pt *p1,pt *p2){
	double distance;
	distance= sqrt(pow((p1->x)-(p2->x),2)+pow((p1->y)-(p2->y),2));
	return distance;
}

double DistancePtoL(pt *p,ln *l){
	/*we need the formula of the line (y=mx+n or mx+n-y=0) where m is slope and the n is intercept.*/
	double m,n,distance;
	m= (l->p1.y - l->p2.y)/(l->p1.x - l->p2.x); //slope
	n= l->p2.y - m * l->p2.x; //intercept. this is same as p1.y -m * p1.x it's a choice.
	distance=fabs(m*(p->x) - (p->y) + n)/sqrt(pow(m,2) + 1); //altered version (where constant of y is always -1) of distance between a line and a point.
	return distance;
}
double AngleLtoL(ln *l1,ln *l2){
	double m1,m2,angle,theta1,theta2; //slopes of the lines 
	m1= (l1->p1.y - l1->p2.y)/(l1->p1.x - l1->p2.x); //slope of l1.
	m2= (l2->p1.y - l2->p2.y)/(l2->p1.x - l2->p2.x); //slope of l2.
	theta1 = atan(m1) * (180.0 / PI);
	theta2 = atan(m2) * (180.0 / PI);
	angle = fabs(theta2 - theta1);
	return angle;
}

double LengthL(ln *l){
	double length;
	length= sqrt(pow((l->p1.x)-(l->p2.x),2) + pow((l->p1.y)-(l->p2.y),2));
	return length;
}

double LengthPG(pg *p,int cornerAmount){
	double length=0.0;
	for(int i=0;i<cornerAmount;i++){
		length+=sqrt(pow((p->corners[i].x)-(p->corners[i+1].x),2)+ pow((p->corners[i].y)-(p->corners[i+1].y),2));
	}
	return length;
}

double AreaPG(pg *p,int cornerAmount){
	double area=0.0;
	for(int i=1;i<cornerAmount;i++){
		area+=((p->corners[i].x) - (p->corners[i-1].x)) * ((p->corners[i].y) + (p->corners[i-1].y)) *0.5; //The easiest non-uniformed polygon area formula I found online.
	}
	area+= ((p->corners[0].x) - (p->corners[cornerAmount-1].x)) * ((p->corners[0].y) + (p->corners[cornerAmount-1].y)) *0.5;
	area=fabs(area);
	return area;
}
