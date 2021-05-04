#include <iostream>
#include "printBoardLIB.h"
#include "hexPlayLIB.h"
using namespace std;

int main(){
	char hexBoard[12][12];
	bool checkedBefore[12][12];  //stores the checked path for winCheck functions.
	int boardSize;
	int	gameMod; // //1 is Player vs Player, 2 is Player vs Computer.
	cout << "WELCOME TO THE HEX GAME."<< endl << "X->PLAYS ON LEFT-RIGHT AXIS"<< endl <<"O->PLAYS ON UP-DOWN AXIS"<< endl <<"MAKE YOUR MOVES WITH LOWERCASE CHARACTERS. (EX: f9)"<< endl 
	<< "PICK YOUR BOARD SIZE:"<< endl << "1: 6x6\n2: 7x7\n3: 8x8\n4: 9x9\n5: 10x10\n6: 11x11\n7: 12x12"<<endl;
	cin >> boardSize;
	boardSize+=5; //1 is 6x6 2 is 7x7...
	cout << "1: PLAYER VS PLAYER\n 2: PLAYER VS COMPUTER"<<endl;
	cin >> gameMod; //1 is PVP 2 is PtoComp
	initializer(hexBoard,checkedBefore,boardSize);  //fills the hexBoard with dots. and makes all the checkedBefore values false.
	if(gameMod==1){  //if the mod is player to player
    	pvpHex(hexBoard,boardSize,checkedBefore);
   	}
	if(gameMod==2){ //PLAYER VS COMPUTER
		pvCompHex(hexBoard,boardSize,checkedBefore);
	}
	return 0;
}
