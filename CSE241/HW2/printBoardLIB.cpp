#include <iostream>
#include "printBoardLIB.h"
using namespace std;

void initializer(char hexBoard[12][12],bool checkedBefore[12][12],const int& boardSize){
	int i,j;
	for(i=0;i<boardSize;i++){            //INITIALIZING THE
		for(j=0;j<boardSize;j++){        // BOARD
			hexBoard[i][j] = '.';
			checkedBefore[i][j]=false;
		}
	}
}

void printBoard(char hexBoard[12][12],const int& boardSize){
	int i,j,k;
	cout << "   "; //spaces between horizontal board indexes (A B C...)
	for(i=0;i<boardSize;i++){
		cout << char(97+i)<<' '; //ASCII number of letter a is 97. So this line creates the horizontal indexes(a b c d e...).
	}
	for(i=0;i<boardSize;i++){
		cout << "\n";
		if(i<9)
			cout <<' ';
		cout << i+1 << ' ';
		for(k=-1;k<i;k+=1){ //this loop is for making the diamond shape. (shifting the dots for each cycle.)
			cout << " ";
		}
		for(j=0;j<boardSize;j++){  //this loop prints the board.
			cout << hexBoard[i][j]<<' ';
		}
	}
	cout << "\n";
}
