#include <iostream>
#include "winCheckLIB.h"
#include "printBoardLIB.h"

using namespace std;

bool winCheckP1LR(char hexBoard[12][12],int i,int j,int boardSize,bool checkedBefore[12][12]){ //returns true if game is finished.
	if(i==0&&j==0){
		while(hexBoard[i][0]!='x' && i<boardSize)
			i++;
	}
	bool returnFlag=false;
	if(j==boardSize-1 && hexBoard[i][j]=='x'){
		checkedBefore[i][j]=true;
		for(int u=0;u<boardSize;u++){
			for(int p=0;p<boardSize;p++){
				if(hexBoard[u][p]=='x' && checkedBefore[u][p]==true)
				hexBoard[u][p]='X';
			}
		}
		printBoard(hexBoard,boardSize);
		return true;
	}
	else if(hexBoard[i][j]=='x'){
		if(hexBoard[i][j+1]=='x' && checkedBefore[i][j+1]==false){
			checkedBefore[i][j]=true;
			j++;
			returnFlag= true;
		}
		else if(hexBoard[i-1][j+1]=='x' && checkedBefore[i-1][j+1]==false){
			checkedBefore[i][j]=true;
			i--;
			j++;
			returnFlag= true;
		}

		else if(hexBoard[i+1][j]=='x' && checkedBefore[i+1][j]==false){
			checkedBefore[i][j]=true;
			i++;
			returnFlag= true;
		}
		else if(hexBoard[i-1][j]=='x' && checkedBefore[i-1][j]==false){
			checkedBefore[i][j]=true;
			i--;
			returnFlag= true;
		}
		else if(hexBoard[i+1][j-1]=='x'&& checkedBefore[i+1][j-1]==false){
			checkedBefore[i][j]=true;
			i++;
			j--;
			returnFlag= true;
		}
		else if(hexBoard[i][j-1]=='x' && checkedBefore[i][j-1]==false){
			checkedBefore[i][j]=true;
			j--;
			returnFlag= true;
		}
		else{
			for(int p=0;p<12;p++){
				for(int u=0;u<12;u++){
					checkedBefore[p][u]=false;
				}
			}
		}
		if(returnFlag==true){ //if there is 'x' path ahead, proceed to next one.
			return winCheckP1LR(hexBoard,i,j,boardSize,checkedBefore);
		}
	}
	return false;
}

bool winCheckP1RL(char hexBoard[12][12],int i,int j,int boardSize,bool checkedBefore[12][12]){ //returns true if game is finished.
	if(i==boardSize-1&&j==0){
		while(hexBoard[i][0]!='x' && i>0)
			i--;
	}
	bool returnFlag=false;
	if(j==boardSize-1 && hexBoard[i][j]=='x'){
		checkedBefore[i][j]=true;
		for(int u=0;u<boardSize;u++){
			for(int p=0;p<boardSize;p++){
				if(hexBoard[u][p]=='x' && checkedBefore[u][p]==true)
				hexBoard[u][p]='X';
			}
		}
		printBoard(hexBoard,boardSize);
		return true;
	}
	else if(hexBoard[i][j]=='x'){
		if(hexBoard[i][j+1]=='x' && checkedBefore[i][j+1]==false){
			checkedBefore[i][j]=true;
			j++;
			returnFlag= true;
		}
		else if(hexBoard[i-1][j+1]=='x' && checkedBefore[i-1][j+1]==false){
			checkedBefore[i][j]=true;
			i--;
			j++;
			returnFlag= true;
		}
		else if(hexBoard[i+1][j]=='x' && checkedBefore[i+1][j]==false){
			checkedBefore[i][j]=true;
			i++;
			returnFlag= true;
		}
		else if(hexBoard[i-1][j]=='x' && checkedBefore[i-1][j]==false){
			checkedBefore[i][j]=true;
			i--;
			returnFlag= true;
		}
		else if(hexBoard[i+1][j-1]=='x'&& checkedBefore[i+1][j-1]==false){
			checkedBefore[i][j]=true;
			i++;
			j--;
			returnFlag= true;
		}
		else if(hexBoard[i][j-1]=='x' && checkedBefore[i][j-1]==false){
					checkedBefore[i][j]=true;
					j--;
					returnFlag= true;
		}
		else{
			for(int p=0;p<12;p++){
				for(int u=0;u<12;u++){
					checkedBefore[p][u]=false;
				}
			}
		}
		if(returnFlag==true){ //if there is 'x' path ahead, proceed to next one.
			return winCheckP1RL(hexBoard,i,j,boardSize,checkedBefore);
		}
	}
	return false;
}

bool winCheckP2LR(char hexBoard[12][12],int i,int j,int boardSize,bool checkedBefore[12][12]){ //returns true if game is finished.
	if(i==0&&j==0){
		while(hexBoard[0][j]!='o' && j<boardSize){
			j++;
		}
	}
	bool returnFlag=false;
		if(i==boardSize-1 && hexBoard[i][j]=='o'){
			checkedBefore[i][j]=true;
			for(int u=0;u<boardSize;u++){
				for(int p=0;p<boardSize;p++){
					if(hexBoard[u][p]=='o' && checkedBefore[u][p]==true)
						hexBoard[u][p]='O';
				}
			}
			printBoard(hexBoard,boardSize);
			return true;
		}
		else if(hexBoard[i][j]=='o'){
			if(hexBoard[i+1][j]=='o' && checkedBefore[i+1][j]==false){
				checkedBefore[i][j]=true;
				i++;
				returnFlag= true;

			}
			else if(hexBoard[i+1][j-1]=='o'&& checkedBefore[i+1][j-1]==false){
				checkedBefore[i][j]=true;
				i++;
				j--;
				returnFlag= true;
			}
			else if(hexBoard[i][j+1]=='o' && checkedBefore[i][j+1]==false){
				checkedBefore[i][j]=true;
				j++;
				returnFlag= true;
			}
			else if(hexBoard[i][j-1]=='o' && checkedBefore[i][j-1]==false){
				checkedBefore[i][j]=true;
				j--;
				returnFlag= true;
			}
			else if(hexBoard[i-1][j]=='o' && checkedBefore[i-1][j]==false){
				checkedBefore[i][j]=true;
				i--;
				returnFlag= true;
			}
			else if(hexBoard[i-1][j+1]=='o' && checkedBefore[i-1][j+1]==false){
				checkedBefore[i][j]=true;
				i--;
				j++;
				returnFlag= true;
			}
			else{
				for(int p=0;p<12;p++){
					for(int u=0;u<12;u++){
						checkedBefore[p][u]=false;
					}
				}
			}

			if(returnFlag==true){ ////if there is 'o' path ahead, proceed to next one.
				return winCheckP2LR(hexBoard,i,j,boardSize,checkedBefore);
			}
		}
	return false;
}

bool winCheckP2RL(char hexBoard[12][12],int i,int j,int boardSize,bool checkedBefore[12][12]){ //returns true if game is finished.
	if(i==0&&j==boardSize-1){
		while(hexBoard[0][j]!='o' && j>0){
			j--;
		}
	}
	bool returnFlag=false;
		if(i==boardSize-1 && hexBoard[i][j]=='o'){
			checkedBefore[i][j]=true;
			for(int u=0;u<boardSize;u++){
				for(int p=0;p<boardSize;p++){
					if(hexBoard[u][p]=='o' && checkedBefore[u][p]==true)
						hexBoard[u][p]='O';
				}
			}
			printBoard(hexBoard,boardSize);
			return true;
		}
		else if(hexBoard[i][j]=='o'){
			if(hexBoard[i+1][j]=='o' && checkedBefore[i+1][j]==false){
				checkedBefore[i][j]=true;
				i++;
				returnFlag= true;

			}
			else if(hexBoard[i+1][j-1]=='o'&& checkedBefore[i+1][j-1]==false){
				checkedBefore[i][j]=true;
				i++;
				j--;
				returnFlag= true;
			}
			else if(hexBoard[i][j+1]=='o' && checkedBefore[i][j+1]==false){
				checkedBefore[i][j]=true;
				j++;
				returnFlag= true;
			}
			else if(hexBoard[i][j-1]=='o' && checkedBefore[i][j-1]==false){
				checkedBefore[i][j]=true;
				j--;
				returnFlag= true;
			}
			else if(hexBoard[i-1][j]=='o' && checkedBefore[i-1][j]==false){
				checkedBefore[i][j]=true;
				i--;
				returnFlag= true;
			}
			else if(hexBoard[i-1][j+1]=='o' && checkedBefore[i-1][j+1]==false){
				checkedBefore[i][j]=true;
				i--;
				j++;
				returnFlag= true;
			}
			else{
				for(int p=0;p<12;p++){
					for(int u=0;u<12;u++){
						checkedBefore[p][u]=false;
					}
				}
			}

			if(returnFlag==true){ ////if there is 'o' path ahead, proceed to next one.
				return winCheckP2RL(hexBoard,i,j,boardSize,checkedBefore);
			}
		}
	return false;
}
