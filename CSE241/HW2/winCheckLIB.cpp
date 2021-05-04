#include <iostream>
#include "winCheckLIB.h"
#include "printBoardLIB.h"

using namespace std;



bool winCheckP1LR(char hexBoard[12][12],int boardSize,bool checkedBefore[12][12],int i,int j){ //returns true if game is finished.
	cellState c1=unOccupied,c2=p1Occupied,c3=p2Occupied,c4=p1Won,c5=p2Won;
	if(i==0&&j==0){
		while(hexBoard[i][0]!=c2 && i<boardSize)
			i++;
	}
	bool returnFlag=false;
	if(j==boardSize-1 && hexBoard[i][j]==c2){
		checkedBefore[i][j]=true;
		for(int u=0;u<boardSize;u++){
			for(int p=0;p<boardSize;p++){
				if(hexBoard[u][p]==c2 && checkedBefore[u][p]==true)
				hexBoard[u][p]=c4;
			}
		}
		printBoard(hexBoard,boardSize);
		return true;
	}
	else if(hexBoard[i][j]==c2){
		if(hexBoard[i][j+1]==c2 && checkedBefore[i][j+1]==false){
			checkedBefore[i][j]=true;
			j++;
			returnFlag= true;
		}
		else if(hexBoard[i-1][j+1]==c2 && checkedBefore[i-1][j+1]==false){
			checkedBefore[i][j]=true;
			i--;
			j++;
			returnFlag= true;
		}

		else if(hexBoard[i+1][j]==c2 && checkedBefore[i+1][j]==false){
			checkedBefore[i][j]=true;
			i++;
			returnFlag= true;
		}
		else if(hexBoard[i-1][j]==c2 && checkedBefore[i-1][j]==false){
			checkedBefore[i][j]=true;
			i--;
			returnFlag= true;
		}
		else if(hexBoard[i+1][j-1]==c2 && checkedBefore[i+1][j-1]==false){
			checkedBefore[i][j]=true;
			i++;
			j--;
			returnFlag= true;
		}
		else if(hexBoard[i][j-1]==c2 && checkedBefore[i][j-1]==false){
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
			return winCheckP1LR(hexBoard,boardSize,checkedBefore,i,j);
		}
	}
	return false;
}

bool winCheckP1RL(char hexBoard[12][12],int boardSize,bool checkedBefore[12][12],int i,int j){ //returns true if game is finished.
	cellState c1=unOccupied,c2=p1Occupied,c3=p2Occupied,c4=p1Won,c5=p2Won;
	if(i==boardSize-1&&j==0){
		while(hexBoard[i][0]!=c2 && i>0)
			i--;
	}
	bool returnFlag=false;
	if(j==boardSize-1 && hexBoard[i][j]==c2){
		checkedBefore[i][j]=true;
		for(int u=0;u<boardSize;u++){
			for(int p=0;p<boardSize;p++){
				if(hexBoard[u][p]==c2 && checkedBefore[u][p]==true)
				hexBoard[u][p]=c4;
			}
		}
		printBoard(hexBoard,boardSize);
		return true;
	}
	else if(hexBoard[i][j]==c2){
		if(hexBoard[i][j+1]==c2 && checkedBefore[i][j+1]==false){
			checkedBefore[i][j]=true;
			j++;
			returnFlag= true;
		}
		else if(hexBoard[i-1][j+1]==c2 && checkedBefore[i-1][j+1]==false){
			checkedBefore[i][j]=true;
			i--;
			j++;
			returnFlag= true;
		}
		else if(hexBoard[i+1][j]==c2 && checkedBefore[i+1][j]==false){
			checkedBefore[i][j]=true;
			i++;
			returnFlag= true;
		}
		else if(hexBoard[i-1][j]==c2 && checkedBefore[i-1][j]==false){
			checkedBefore[i][j]=true;
			i--;
			returnFlag= true;
		}
		else if(hexBoard[i+1][j-1]==c2&& checkedBefore[i+1][j-1]==false){
			checkedBefore[i][j]=true;
			i++;
			j--;
			returnFlag= true;
		}
		else if(hexBoard[i][j-1]==c2 && checkedBefore[i][j-1]==false){
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
			return winCheckP1RL(hexBoard,boardSize,checkedBefore,i,j);
		}
	}
	return false;
}

bool winCheckP2LR(char hexBoard[12][12],int boardSize,bool checkedBefore[12][12],int i,int j){ //returns true if game is finished.
	cellState c1=unOccupied,c2=p1Occupied,c3=p2Occupied,c4=p1Won,c5=p2Won;
	if(i==0&&j==0){
		while(hexBoard[0][j]!=c3 && j<boardSize){
			j++;
		}
	}
	bool returnFlag=false;
		if(i==boardSize-1 && hexBoard[i][j]==c3){
			checkedBefore[i][j]=true;
			for(int u=0;u<boardSize;u++){
				for(int p=0;p<boardSize;p++){
					if(hexBoard[u][p]==c3 && checkedBefore[u][p]==true)
						hexBoard[u][p]=c5;
				}
			}
			printBoard(hexBoard,boardSize);
			return true;
		}
		else if(hexBoard[i][j]==c3){
			if(hexBoard[i+1][j]==c5 && checkedBefore[i+1][j]==false){
				checkedBefore[i][j]=true;
				i++;
				returnFlag= true;

			}
			else if(hexBoard[i+1][j-1]==c3 && checkedBefore[i+1][j-1]==false){
				checkedBefore[i][j]=true;
				i++;
				j--;
				returnFlag= true;
			}
			else if(hexBoard[i][j+1]==c3 && checkedBefore[i][j+1]==false){
				checkedBefore[i][j]=true;
				j++;
				returnFlag= true;
			}
			else if(hexBoard[i][j-1]==c3 && checkedBefore[i][j-1]==false){
				checkedBefore[i][j]=true;
				j--;
				returnFlag= true;
			}
			else if(hexBoard[i-1][j]==c3 && checkedBefore[i-1][j]==false){
				checkedBefore[i][j]=true;
				i--;
				returnFlag= true;
			}
			else if(hexBoard[i-1][j+1]==c3 && checkedBefore[i-1][j+1]==false){
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

			if(returnFlag==true){ ////if there is c3 path ahead, proceed to next one.
				return winCheckP2LR(hexBoard,boardSize,checkedBefore,i,j);
			}
		}
	return false;
}

bool winCheckP2RL(char hexBoard[12][12],int boardSize,bool checkedBefore[12][12],int i,int j){ //returns true if game is finished.
	cellState c1=unOccupied,c2=p1Occupied,c3=p2Occupied,c4=p1Won,c5=p2Won;
	if(i==0&&j==boardSize-1){
		while(hexBoard[0][j]!=c3 && j>0){
			j--;
		}
	}
	bool returnFlag=false;
		if(i==boardSize-1 && hexBoard[i][j]==c3){
			checkedBefore[i][j]=true;
			for(int u=0;u<boardSize;u++){
				for(int p=0;p<boardSize;p++){
					if(hexBoard[u][p]==c3 && checkedBefore[u][p]==true)
						hexBoard[u][p]=c5;
				}
			}
			printBoard(hexBoard,boardSize);
			return true;
		}
		else if(hexBoard[i][j]==c3){
			if(hexBoard[i+1][j]==c3 && checkedBefore[i+1][j]==false){
				checkedBefore[i][j]=true;
				i++;
				returnFlag= true;

			}
			else if(hexBoard[i+1][j-1]==c3 && checkedBefore[i+1][j-1]==false){
				checkedBefore[i][j]=true;
				i++;
				j--;
				returnFlag= true;
			}
			else if(hexBoard[i][j+1]==c3 && checkedBefore[i][j+1]==false){
				checkedBefore[i][j]=true;
				j++;
				returnFlag= true;
			}
			else if(hexBoard[i][j-1]==c3 && checkedBefore[i][j-1]==false){
				checkedBefore[i][j]=true;
				j--;
				returnFlag= true;
			}
			else if(hexBoard[i-1][j]==c3 && checkedBefore[i-1][j]==false){
				checkedBefore[i][j]=true;
				i--;
				returnFlag= true;
			}
			else if(hexBoard[i-1][j+1]==c3 && checkedBefore[i-1][j+1]==false){
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

			if(returnFlag==true){ ////if there is c3 path ahead, proceed to next one.
				return winCheckP2RL(hexBoard,boardSize,checkedBefore,i,j);
			}
		}
	return false;
}
