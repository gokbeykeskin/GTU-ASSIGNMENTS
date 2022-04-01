#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void file_reader(char *fpath); //reads the whole file.
void array_generator(char buffer[2000][200]); //generates the desired arrays and creates the menu.

void main(void){
	char *fpath="Video_Games.txt";
	file_reader(fpath);
}

void file_reader(char *fpath){
	char buffer[2000][200];
	FILE *fp;
	fp=fopen(fpath,"r");
	int i=0;
	fgets(buffer[i],200,fp); //gets the first line (junk)
	while(fgets(buffer[i],200,fp)){
		i++;
	} 
	fclose(fp);
	array_generator(buffer);
}

void array_generator(char buffer[2000][200]){
	char gamenames[2000][100],genres[20][20],platforms[20][20],tempgenre[20],tempplat[20],junk[20];
	float data[2000][7];
	int indexgenre=0,indexplat=0,wrotebefore=0,i;
	int option,year,option2,printedflag,flag; //this variables are for the menu part of the function.
	int pc=0,ps=0,dc=0,ps2=0,x=0,gba=0,gc=0,ds=0,psp=0,x360=0; //platform counters
	int simulation=0,shooter=0,fighting=0,action=0,role_playing=0,racing=0,misc=0,puzzle=0,platform=0,strategy=0,sports=0,adventure=0; //genre counters
	float sumscore=0; //this variables are for the menu part of the function.
	char name[100],name2[100];//this variables are for the menu part of the function.
	for(i=0;i<2000;i++){
		sscanf(buffer[i],"%[^,],%[^,],%[^,],%f,%f,%f,%f,%f",gamenames[i],tempgenre,tempplat,&data[i][2],&data[i][3],&data[i][4],&data[i][5],&data[i][6]); 
																						// indexgenre is data[i][0],indexplat is data[i][1]
		if(data[i][5]==0) //not_available eu sales.
			sscanf(buffer[i],"%[^,],%[^,],%[^,],%f,%f,%f,%[^,],%f",gamenames[i],tempgenre,tempplat,&data[i][2],&data[i][3],&data[i][4],junk,&data[i][6]); 	
		for(int j=0;j<indexgenre;j++){
			if(strcmp(genres[j],tempgenre)==0){ //if the tempgenre writed to genres before.
				wrotebefore=1;
				data[i][0]=j;
				break;
			}
			else wrotebefore=0;
		}

			if(wrotebefore==0){ //if the temp didn't wrote to genres array before.
				strcpy(genres[indexgenre],tempgenre); //copy tempgenre to genres array.
				data[i][0]=indexgenre; 
				indexgenre++;
			}
		for(int j=0;j<indexplat;j++){
			if(strcmp(platforms[j],tempplat)==0){ //same as the genres
				wrotebefore=1;
				data[i][1]=j;
				break;
			}
			else wrotebefore=0;
		}
		if(wrotebefore==0){
			strcpy(platforms[indexplat],tempplat);
			data[i][1]=indexplat;
			indexplat++;
		}
	}

	while(option!=8){
		printf("0:List of the Genres\n1:List of the Platforms\n2:List of the Games Through the Years\n3:All Informations of a Single Game\n4:Average of the user scores:\n5:Geographical Information of a Single Game\n6:Frequence of the Genres\n7:Frequence of the Platforms\n8:Exit\n");
		scanf("%d",&option);
		printf("\n");
		switch(option){
			case 0:
				for(i=0;i<12;i++) printf("%s\n",genres[i]);
				printf("\n");
				break;
			case 1:
				for(i=0;i<10;i++) printf("%s\n",platforms[i]);
				printf("\n");
				break;
			case 2:
				printedflag=0;
				printf("Enter a year:\n");
				scanf("%d",&year);
				printf("Until (0) or Since (1) %d: ",year);
				scanf("%d",&option2);
				if(option2==0){
					for(i=0;i<2000;i++){
						if(data[i][2]<year){
							printf("%s\n",gamenames[i]);
							printedflag=1;
						}
					}
				}
				else if(option2==1){
					for(i=0;i<=2000;i++){
						if(data[i][2]>year){
							printf("%s\n",gamenames[i]);
							printedflag=1;
						}
					}
				}
				else printf("Wrong option\n");
				if(printedflag==0) printf("No games in this range.\n");
				break;
			case 3:
				flag=1;
				while(flag==1){
					printf("Please enter the name of the game:\n");
					scanf("%s",name);
					for(i=0;i<2000;i++){
						if(strcmp(gamenames[i],name)==0){
							if(data[i][5]==0)
								printf("Name: %s\nGenre: %s\nPlatform: %s\nYear: %.0f\nSales in NA: %f\nSales in EU: %f\nTotal Sales: %s\nUser Score: %f\n\n",gamenames[i],genres[(int)data[i][0]],platforms[(int)data[i][1]],data[i][2],data[i][3],data[i][4],"Not Available",data[i][6]);
							else
								printf("Name: %s\nGenre: %s\nPlatform: %s\nYear: %.0f\nSales in NA: %f\nSales in EU: %f\nTotal Sales: %f\nUser Score: %f\n\n",gamenames[i],genres[(int)data[i][0]],platforms[(int)data[i][1]],data[i][2],data[i][3],data[i][4],data[i][5],data[i][6]);
							i=2000;
							flag=0;
						}
					}
					if(flag==1) printf("There is no such a game.\n");
				}
				break;
			case 4:
				for(i=0;i<2000;i++){
					sumscore+=data[i][6];
				}
				printf("Average: %.6f\n",sumscore/2000);
				break;
			case 5:
				flag=1;
				while(flag==1){
					printf("Please enter the name of the game\n");
					scanf("%s",name2);
					for(i=0;i<2000;i++){
						if(strcmp(gamenames[i],name2)==0){
							if(data[i][3]>data[i][4]) printf("This game was more popular on North America.\n");
							else if(data[i][3]<data[i][4]) printf("This game was more popular on Europe.\n");
							else printf("This game was equally popular on Europe and North America.\n");
							i=2000;
							flag=0;
						}
					}
					if(flag==1) printf("There is no such a game.\n");
				}
				break;
			case 6:
				for(i=0;i<2000;i++){
					if(data[i][0]==0) simulation++;
					else if(data[i][0]==1) shooter++;
					else if(data[i][0]==2) fighting++;
					else if(data[i][0]==3) action++;
					else if(data[i][0]==4) role_playing++;
					else if(data[i][0]==5) racing++;
					else if(data[i][0]==6) misc++;
					else if(data[i][0]==7) puzzle++;
					else if(data[i][0]==8) platform++;
					else if(data[i][0]==9) strategy++;
					else if(data[i][0]==10) sports++;
					else if(data[i][0]==11) adventure++;
				}
				printf("simulation\t%d\nshooter\t\t%d\nfighting\t%d\naction\t\t%d\nrole_playing\t%d\nracing\t\t%d\nmisc\t\t%d\npuzzle\t\t%d\nplatform\t%d\nstrategy\t%d\nsports\t\t%d\nadventure\t%d\n\n",simulation,shooter,fighting,action,role_playing,racing,misc,puzzle,platform,strategy,sports,adventure);
				break;
			case 7:
				for(i=0;i<2000;i++){
					if(data[i][1]==0) pc++;
					else if(data[i][1]==1) ps++;
					else if(data[i][1]==2) dc++;
					else if(data[i][1]==3) ps2++;
					else if(data[i][1]==4) x++;
					else if(data[i][1]==5) gba++;
					else if(data[i][1]==6) gc++;
					else if(data[i][1]==7) ds++;
					else if(data[i][1]==8) psp++;
					else if(data[i][1]==9) x360++;
				}
				printf("pc\t%d\nps\t%d\ndc\t%d\nps2\t%d\nx\t%d\ngba\t%d\ngc\t%d\nds\t%d\npsp\t%d\nx360\t%d\n\n",pc,ps,dc,ps2,x,gba,gc,ds,psp,x360);
				break;
			default:
				if(option!=8)
					printf("INVALID OPTION. TRY AGAIN.");
				break;
		}
	}
	printf("Goodbye!\n");
}
