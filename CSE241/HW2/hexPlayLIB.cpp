#include <iostream>
#include <random>  //BOT STARTS FROM A RANDOM LOCATION
#include <chrono>  //THATS WHY I USED THIS 2 LIBRARIES. FOR CREATING A RANDOM SEED AND ENGINE.
#include "winCheckLIB.h"
#include "printBoardLIB.h"
#include "hexPlayLIB.h"
#include "LoadSaveLIB.h"
using namespace std;


void pvpHex(char hexBoard[12][12],int boardSize,bool checkedBefore[12][12]){
	cellState c1=unOccupied,c2=p1Occupied,c3=p2Occupied,c4=p1Won,c5=p2Won;
	auto gameFinished1LR=false,gameFinished1RL=false,gameFinished2LR=false,gameFinished2RL=false; //player 1 won(LEFT TO RIGHT - RIGHT TO LEFT), player 2 won(LEFT TO RIGHT - RIGHT TO LEFT).
	string move;
	char moveX; //player move X axis
	int moveY; //player move Y axis
	decltype(moveY) gameMode = 1;

	printBoard(hexBoard,boardSize);
	bool wrongMove=false; // true when player tries to capture an occupied place or outside of the board.
	while(gameFinished1LR==false && gameFinished1RL==false){
		wrongMove=false;
		do{ //player 1
			cout <<"SAVE filename.txt to save current state, LOAD filename.txt to load a previous game."<<endl<< "Make your move(1st player): ";
			getline(cin,move);
			if(move[0]>=97 && move[0]<=97+boardSize-1 && stoi(move.substr(1))>0 && stoi(move.substr(1))<=boardSize){
				moveX=move[0];
				moveY=stoi(move.substr(1));
			
				moveX-=97; //a is 0. index (a-97 = 0)
				moveY-=1;
				if(moveX<0 || moveX>boardSize-1 ||moveY<0 ||moveY>boardSize-1){
					cout << "This is not a valid move. Make another move.";
					wrongMove=true;
				}
				else if(hexBoard[moveY][moveX]== c1){ //if this move is available
					hexBoard[moveY][moveX]=c2;
					wrongMove=false;
				}
				else{
					cout << "This place is already occupied. Make another move.";
					wrongMove=true;
				}
				gameFinished1LR=winCheckP1LR(hexBoard,boardSize,checkedBefore);                     //the reason I made 2 check conditions for each player is:
				gameFinished1RL=winCheckP1RL(hexBoard,boardSize,checkedBefore,boardSize-1,0);			//when there is a 'x'(or 'o') on left, 
				if(gameFinished1LR==true || gameFinished1RL==true){										//it wasn't controlling the other possible routes.
					cout << "PLAYER 1 HAS WON. CONGRATS!!!"<< endl;
					return;
				}
			}
			else if(move.substr(0,4) == "SAVE"){
				saveGame(hexBoard,move.substr(5),boardSize,1);
				wrongMove=true; //make the player1 play again.
			}
			else if(move.substr(0,4)=="LOAD"){
				loadGame(hexBoard,move.substr(5),boardSize,gameMode);
				printBoard(hexBoard,boardSize);
				wrongMove=true; //make the player1 play again.
			}
			else{
				wrongMove=true;
			}
				
		}while(wrongMove==true); //take another move until p1 makes a valid move.
		printBoard(hexBoard,boardSize);
		if(gameMode==1){
			do{  //player 2 (same as player 1.)
				cout <<"SAVE filename.txt to save current state, LOAD filename.txt to load a previous game."<<endl<< "Make your move(2st player): ";
				getline(cin,move);
				if(move[0]>=97 && move[0]<=97+boardSize-1 && stoi(move.substr(1))>0 && stoi(move.substr(1))<=boardSize){
					moveX=move[0];
					moveY=stoi(move.substr(1));
				
					moveX-=97; //a is 0. index (a-97 = 0)
					moveY-=1;
					if(moveX<0 || moveX>boardSize-1 ||moveY<0 ||moveY>boardSize-1){
						cout << "This is not a valid move. Make another move.";
						wrongMove=true;
					}
					else if(hexBoard[moveY][moveX]== c1){ //if this move is available
						hexBoard[moveY][moveX]=c3;
						wrongMove=false;
					}
					else{
						cout << "This place is already occupied. Make another move.";
						wrongMove=true;
					}
					gameFinished2LR=winCheckP2LR(hexBoard,boardSize,checkedBefore);
					gameFinished2RL=winCheckP2RL(hexBoard,boardSize,checkedBefore,0,boardSize-1);
					if(gameFinished2LR==true || gameFinished2RL==true){
						cout << "PLAYER 2 HAS WON. CONGRATS!!!"<<endl;
						return;
					}
				}
				else if(move.substr(0,4) == "SAVE"){
					saveGame(hexBoard,move.substr(5),boardSize,1);
					wrongMove=true; //make the player1 play again.
				}
				else if(move.substr(0,4)=="LOAD"){
					loadGame(hexBoard,move.substr(5),boardSize,gameMode);
					printBoard(hexBoard,boardSize);
					wrongMove=true; //make the player1 play again.
				}
				else{
					wrongMove=true;
				}
					
				
			}while(wrongMove==true);
			printBoard(hexBoard,boardSize);
		}
		if(gameMode==2){
			gameFinished1LR=true;
			pvCompHex(hexBoard,boardSize,checkedBefore);
		}
	}
}

void pvCompHex(char hexBoard[12][12],int boardSize,bool checkedBefore[12][12]){
	cellState c1=unOccupied,c2=p1Occupied,c3=p2Occupied,c4=p1Won,c5=p2Won;
	int gameMode=2;
	unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count(); //create a seed for random num generation
	static std::default_random_engine eng(seed); 			//create a random engine (eng)
	bool startOver=false; //true if bot has nowhere to go. bot starts from beginning with a different path.
	bool botFirstMove=true;
	bool botFlag=false;
	int botMoveX,botMoveY;
	int botTempX;  //when bot blocks the player holds the current x position of the bot.
	int botTempY; //when bot blocks the player holds the current y position of the bot.
	string move;
	char moveX; //player move X axis
	int moveY; //player move Y axis
	bool gameFinished1LR=false,gameFinished1RL=false,gameFinished2LR=false,gameFinished2RL=false;

	bool wrongMove=false; // true when player tries to capture an occupied place or outside of the board.
	while(gameFinished2RL==false && gameFinished2LR==false){
		if(botFirstMove==true){  //if it's bot's first move or if bot is stuck somewhere, put 'o' to a random location.
			do{
				botMoveX=(eng()%(boardSize)); //first move of the computer. A random x location.
				botMoveY =0; //always starts from the top
			}while(hexBoard[botMoveY][botMoveX]!=c1);
		}
		else if(startOver==true){
			do{
				botMoveX=(eng()%(boardSize)); // A random x location.
				botMoveY =(eng()%(boardSize));// A random y location.
				startOver=false;
			}while(hexBoard[botMoveY][botMoveX]!=c1);
		}


		do{
			if(botFirstMove==false && startOver==false){
				if(botMoveY+1<(boardSize) && hexBoard[botMoveY+1][botMoveX]=='.' && botFlag==false){  //always tries to go down if possible
					botMoveY++;
					botFlag=false;
				}
				else if(botMoveX-1>=0 && botMoveY-1<(boardSize) && hexBoard[botMoveY+1][botMoveX-1]=='.'&& botFlag==false){ //if can't go down, go left down.
					botMoveX--;
					botMoveY++;
					botFlag=false;
				}
				else if(botMoveY==boardSize-1){  //when bot occupies somewhere on bottom after blocking player, goes back to the place which it blocked the player and proceeds upward to win.
					botMoveX=botTempX;
					botMoveY=botTempY-1;
					botFlag=true;
				}
				else if(hexBoard[moveY][moveX+1]==c1 && botFlag==false){ //if bot can't go down or leftdown, it blocks the player1. (if possible)
					botMoveX=moveX+1;
					botMoveY=moveY;
					botTempX=botMoveX;
					botTempY=botMoveY;
				}
				else if(hexBoard[moveY][moveX-1]==c1 && botFlag==false){ //if bot can't go down or leftdown, it blocks the player1. (if possible)
					botMoveX=moveX-1;
					botMoveY=moveY;
					botTempX=botMoveX;
					botTempY=botMoveY;
				}
				else if(botMoveX+1<boardSize && hexBoard[botMoveY][botMoveX+1]==c1&&botFlag==false){ //if can't go down or block p1 go right
					botMoveX++;
					botFlag=false;
				}
				else if(botMoveX-1>=0 && hexBoard[botMoveY][botMoveX-1]==c1 &&botFlag==false){ //otherwise go left
					botMoveX--;
					botFlag=false;
				}
				else if(botMoveX+1<(boardSize) && (botMoveY-1>=0) && hexBoard[botMoveY-1][botMoveX+1]=='.'){ //if can't do any of the things above, go upright.
					botMoveX++;																				//if botFlag==true(blocked player and went bottom) starts to go upwards
					botMoveY--;
				}
				else if(botMoveY-1>=0 && hexBoard[botMoveY-1][botMoveX]==c1) //go directly up
					botMoveY--;												  //if botFlag==true(blocked player and went bottom) starts to go upwards
				else   //if bot has nowhere to play and it cant block player 1, it starts over from somewhere random.
					startOver=true;
			}

				botFirstMove=false;

				if(hexBoard[botMoveY][botMoveX]== c1){ //place the bot's move
					hexBoard[botMoveY][botMoveX]=c3;
					wrongMove=false;
				}
				else if(startOver==false)
					wrongMove= true;

				gameFinished2LR=winCheckP2LR(hexBoard,boardSize,checkedBefore);
				gameFinished2RL=winCheckP2RL(hexBoard,boardSize,checkedBefore,0,boardSize-1);
				if(gameFinished2LR==true || gameFinished2RL==true){
					cout << "COMPUTER HAS WON. YOU HAVE BEEN DEFEATED."<<endl;
					return;
				}
		}while(wrongMove==true);
		printBoard(hexBoard,boardSize);

		do{
			cout <<"SAVE filename.txt to save current state, LOAD filename.txt to load a previous game."<<endl<< "Make your move(1st player): ";
						getline(cin,move);
						if(move[0]>=97 && move[0]<=97+boardSize-1 && stoi(move.substr(1))>0 && stoi(move.substr(1))<=boardSize){
							moveX=move[0];
							moveY=stoi(move.substr(1));
						
							moveX-=97; //a is 0. index (a-97 = 0)
							moveY-=1; // char to int
							if(moveX<0 || moveX>boardSize-1 ||moveY<0 ||moveY>boardSize-1){
								cout << "This is not a valid move. Make another move.";
								wrongMove=true;
							}
							else if(hexBoard[moveY][moveX]== c1){ //if this move is available
								hexBoard[moveY][moveX]=c2;
								wrongMove=false;
							}
							else{
								cout << "This place is already occupied. Make another move.";
								wrongMove=true;
							}
							gameFinished1LR=winCheckP1LR(hexBoard,boardSize,checkedBefore);                     //the reason I made 2 check conditions for each player is:
							gameFinished1RL=winCheckP1RL(hexBoard,boardSize,checkedBefore,boardSize-1,0);			//when there is a 'x'(or 'o') on left, 
							if(gameFinished1LR==true || gameFinished1RL==true){										//it wasn't controlling the other possible routes.
								cout << "PLAYER 1 HAS WON. CONGRATS!!!"<< endl;
								return;
							}
						}
						else if(move.substr(0,4) == "SAVE"){
							saveGame(hexBoard,move.substr(5),boardSize,2);
							wrongMove=true; //make the player1 play again.
						}
						else if(move.substr(0,4)=="LOAD"){
							loadGame(hexBoard,move.substr(5),boardSize,gameMode);
							printBoard(hexBoard,boardSize);
						}
						else{
							wrongMove=true;
						}
		}while(wrongMove==true); //take another move until bot  makes a valid move
		if(gameMode==1){
			gameFinished2LR=true;
			pvpHex(hexBoard,boardSize,checkedBefore);
		}
		cout << '\n';
	}
}
