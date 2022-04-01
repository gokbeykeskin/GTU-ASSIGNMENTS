#include <stdio.h>
#include <stdlib.h>
#include "arduino-serial-lib.c"
#include "arduino-serial-lib.h"
#include <stdint.h>

int main()
{
	int dev = serialport_init("/dev/ttyACM0", 9600);
	int counter=0;
	int chose=-1;
	int quit=0;
	char c;
	const char* a1="a";
	const char* a2="b";
	const char* a3="c";
	const char* a4="d";
	const char* a5="e";
	char buf[256];
	char count[256];
	uint8_t num;
		printf(" ## WELCOME TO GTU ARDUINO LAB  ##\n ## STUDENT NAME: GÖKBEY GAZİ KESKİN  ##\n ## PLEASE SELECT FROM THE FOLLOWING: ##\n ## MENU:  #\n (1) TURN ON LED ON ARDUINO\n (2) TURN OFF LED ON ARDUINO\n (3) Flash ARDUINO LED 3 TIMES\n (4) SEND A NUMBER TO ARDUINO TO COMPUTE SQUARE BY ARDUINO\n (5)BUTTON PRESS COUNTER\n (0) EXIT\n");
	while(chose!=0){
		quit=0;
		printf("PLEASE SELECT:");
		scanf("%d",&chose);
		if(chose==1){
			serialport_write(dev, a1);
		}
		else if(chose==2){
			serialport_write(dev, a2);
		}
		else if(chose==3){
			serialport_write(dev, a3);
		}
		else if(chose==4){
			serialport_write(dev, a4);
			scanf("%hhd", &num);
			if(num<0){
				num=num*(-1);
			}
			serialport_writebyte(dev, num);
			serialport_read_until(dev, buf, '\n', 256, 1000);
			printf("Answer: ");
			printf("%s\n",buf);
			
			
			
			
		}
		else if(chose==5){
			serialport_write(dev,a5);
			while(exit==0){
				serialport_read_until(dev, count, '\n', 256, 1000);
				if(count[0]=='a'){
					counter+=1;
					printf("%d\n",counter);

				}
				if(count[0]=='q'){
					quit=1;
				}
				
			}
			counter=0;
		}
	}	
	serialport_close(dev);
	return 0;
}

