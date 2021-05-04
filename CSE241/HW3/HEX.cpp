#include <iostream>
#include <vector>
#include <fstream>
#include <random>  //BOT STARTS FROM A RANDOM LOCATION
#include <chrono>
#include "HEX.h"

int Hex::totalMoves = 0;

void Hex::moveCounter(){
	totalMoves++;
}
int Hex::getTotalMoves(){
	return totalMoves;
}

Hex addNewGame(){ //(friend)
	Hex a;
	return a;
}

Hex::Hex(int x,int y){
	isActive=true;
	if(x>5 && x<28)	boardSize = x;
	if(y == 1 || y== 2) 	gameMode=y;
		hexCells.resize(boardSize+2);
		for(int i=0;i<boardSize+2;i++){            //initializing the board
			hexCells[i].resize(boardSize+2);
		}
		checkedBefore.resize(boardSize+2);
		for(int i=0;i<boardSize+2;i++){            //initializing the checkedBefore vector
			checkedBefore[i].resize(boardSize+2);
			for(int j=0;j<boardSize+2;j++){
				checkedBefore[i][j]=false; 
			}
		}
}

inline Hex::Cell::Cell(int x,int y){
	column=x;
	row=y;
	state = empty;
}

inline void Hex::printBoardSize() const{
	cout<<"Width: " << getBoardSize() << "Height:" << getBoardSize()<<endl;
}

void Hex::saveGame(const string& fileName){
	ofstream saveFile;
	saveFile.open(fileName);
	if(saveFile.is_open()){
		saveFile<<boardSize<<'-';
		saveFile<<gameMode<<'-';
		for(int i=0;i<boardSize;i++){
			for(int j=0;j<boardSize;j++){
				saveFile<<static_cast<char>(hexCells[i][j].getState());
			}
		}
		saveFile.close();
	}
	else
		cerr<<"An error has been occured while opening the file.";
}

void Hex::loadGame(const string& fileName){
	auto k=0;
	ifstream loadFile;
	loadFile.open(fileName);
	string gameSave;
	if(loadFile.is_open()){
		loadFile>>gameSave;
		boardSize = stoi(gameSave.substr(0,gameSave.find('-')));
		gameMode = stoi(gameSave.substr(gameSave.find('-')+1,gameSave.find('-')));
		while(gameSave[k]!='.' && gameSave[k]!='x' && gameSave[k]!='o') //find where saved board starts.
			k++;	

		
		for(int i=0;i<boardSize;i++){
			for(int j=0;j<boardSize;j++){
					hexCells[i][j].setState(static_cast<Cell::cellState>(gameSave[k]));
					k++;
			}
		}
		loadFile.close();
	}
		else
			cerr<<"An error has been occured while opening the file.";
}

void Hex::printBoard() const{
	//int i,j,k;
	cout << "   "; //spaces between horizontal board indexes (A B C...)
	for(int i=0;i<boardSize;i++){
		cout << char(97+i)<<' '; //ASCII number of letter a is 97. So this line creates the horizontal indexes(a b c d e...).
	}
	for(int i=1;i<boardSize+1;i++){
		cout << "\n";
		if(i<10)
			cout <<' ';
		cout << i << ' ';
		for(int k=-1;k<i-1;k+=1){ //this loop is for making the diamond shape. (shifting the dots for each cycle.)
			cout << " ";
		}
		for(int j=1;j<boardSize+1;j++){  //this loop prints the board.
			cout << static_cast<char>(hexCells[i][j].getState())<<' ';
		}
	}
	cout << "\n";
}
void Hex::playGame(){  //play functions and wincheck
	bool gameEnded=false,wrongMove=false;
	string move;
	cin.ignore();
	if(gameMode==1){ //pvp
		printBoard();
		while(gameEnded==false){
			do{
				cout<<"Make your move(1st player)(or type SAVE location.txt or LOAD location.txt or type quit to exit.): ";
			    getline(cin,move);
				if(move.substr(0,4)=="SAVE"){
					saveGame(move.substr(5));
					wrongMove=true;
				}
				else if(move.substr(0,4)=="LOAD"){
					loadGame(move.substr(5));
					printBoard();
					wrongMove=true;
				}
				else if(move=="quit"){
					return;
				}
				else if(move[0]>=96 && move[0]<=96+boardSize && stoi(move.substr(1))>0 && stoi(move.substr(1))<=boardSize){
					int yMove = static_cast<int>(move[0])-96;
					int xMove = stoi(move.substr(1));
					if(xMove<1 || xMove>=boardSize+1 || yMove<1 || yMove>=boardSize+1){
						wrongMove = true;
						cout << "This is not an existing cell.";
					}
					else if(hexCells[xMove][yMove].getState() != Cell::empty){
						wrongMove = true;
						cout << "This place has already been occupied.";
					}
					else{
						play(yMove,xMove,1);
						if(winCheckP1(1,1)==true){
							cout<<"Player 1 Won!"<<endl;
							return;
						}
						wrongMove=false;
					}
				}
				else{
					wrongMove=true;
				}
			}while(wrongMove==true);
			printBoard();
			do{
				cout<<"Make your move(2nd player)(or type SAVE location.txt or LOAD location.txt or type quit to exit.):";
				getline(cin,move);
				if(move.substr(0,4)=="SAVE"){
					saveGame(move.substr(5));
					wrongMove=true;
				}
				else if(move.substr(0,4)=="LOAD"){
					loadGame(move.substr(5));
					printBoard();
					wrongMove=true;
				}
				else if(move=="quit"){
					return;
				}
				else if(move[0]>=96 && move[0]<=96+boardSize && stoi(move.substr(1))>0 && stoi(move.substr(1))<=boardSize){
					int yMove = static_cast<int>(move[0])-96;
					int xMove = stoi(move.substr(1));
					if(xMove<1 || xMove>=boardSize+1 || yMove<1 || yMove>=boardSize+1){
						wrongMove = true;
						cout << "This is not an existing cell.";
					}
					else if(hexCells[xMove][yMove].getState() != Cell::empty){
						wrongMove = true;
						cout << "This place has already been occupied.";
					}
					else{
						play(yMove,xMove,2);
						if(winCheckP2(1,1)==true){
							cout<<"Player 2 Won!"<<endl;
							return;
						}
						wrongMove=false;
					}
				}
				else{
					wrongMove=true;
				}
			}while(wrongMove==true);
		}
	}
	else if(gameMode==2){

		while(gameEnded==false){
			play();
			if(winCheckP1(1,1)==true){
				cout<<"Computer has Won!"<<endl;
				return;
			}
			printBoard();
			do{
				cout<<"Make your move(2nd player)(or type SAVE location.txt or LOAD location.txt or type quit to exit.)";
				getline(cin,move);
				if(move.substr(0,4)=="SAVE"){
					saveGame(move.substr(5));
					wrongMove=true;
				}
				else if(move.substr(0,4)=="LOAD"){
					loadGame(move.substr(5));
					printBoard();
					wrongMove=true;
				}
				else if(move=="quit"){
					return;
				}
				else if(move[0]>=96 && move[0]<=96+boardSize && stoi(move.substr(1))>0 && stoi(move.substr(1))<=boardSize){
					int yMove = static_cast<int>(move[0])-96;
					int xMove = stoi(move.substr(1));
					if(xMove<1 || xMove>=boardSize+1 || yMove<1 || yMove>=boardSize+1){
						wrongMove = true;
						cout << "This is not an existing cell.";
					}
					else if(hexCells[xMove][yMove].getState() != Cell::empty){
						wrongMove = true;
						cout << "This place has already been occupied.";
					}
					else{
						play(yMove,xMove,2);
						if(winCheckP2(1,1)==true){
							cout<<"Player Won!"<<endl;
							return;
						}
						wrongMove=false;
					}
				}
				else wrongMove=true;
			}while(wrongMove==true);
		}
	}
}

void Hex::play(){
	unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count(); //create a seed for random num generation
	static std::default_random_engine eng(seed);
	int botMoveY;
	int botMoveX;
	do{
		botMoveY = eng() % (boardSize+1);
		botMoveX = eng() % (boardSize+1);
		cout<<hexCells[botMoveX][botMoveY].getState();
	}while(hexCells[botMoveX][botMoveY].getState() != Cell::empty || botMoveX==0 || botMoveY==0);
	hexCells[botMoveX][botMoveY].setState(Cell::p1);
	moveCounter();
	cout<<"BOT'S MOVE:"<<static_cast<char>(botMoveY+96)<<botMoveX<<endl;
}
	
void Hex::play(int yMove,int xMove,int player){//if player is 1 puts 'x', if it is 2 puts 'o'

	if(player==1){
		hexCells[xMove][yMove].setState(Cell::p1);
		printBoard();
	}
	else if(player==2){
		hexCells[xMove][yMove].setState(Cell::p2);
		printBoard();
	}
	moveCounter();
	
}

bool Hex::winCheckP1(int i,int j){
	if(i==1&&j==1){
		while(hexCells.at(i).at(1).getState()!=Cell::p1 && i<boardSize+1){
			i++;
		}
	}
	bool returnFlag=false;
	if(j==boardSize && hexCells.at(i).at(j).getState()==Cell::p1){
		checkedBefore[i][j]=true;
		for(int u=1;u<boardSize+1;u++){
			for(int p=1;p<boardSize+1;p++){
				if(hexCells[u][p].getState()== Cell::p1 && checkedBefore[u][p]==true)
				hexCells[u][p].setState(Cell::p1won);
			}
		}
		printBoard();
		isActive = false;
		return true;
	}
	else if(hexCells.at(i).at(j).getState()==Cell::p1){
		if(hexCells.at(i).at(j+1).getState()==Cell::p1 && checkedBefore[i][j+1]==false){
			checkedBefore[i][j]=true;
			j++;
			returnFlag= true;
		}
		else if(hexCells.at(i-1).at(j+1).getState()==Cell::p1 && checkedBefore[i-1][j+1]==false){
			checkedBefore[i][j]=true;
			i--;
			j++;
			returnFlag= true;
		}
		else if(hexCells.at(i+1).at(j).getState()==Cell::p1 && checkedBefore[i+1][j]==false){
			checkedBefore[i][j]=true;
			i++;
			returnFlag= true;
		}
		else if(hexCells.at(i-1).at(j).getState()==Cell::p1 && checkedBefore[i-1][j]==false){
			checkedBefore[i][j]=true;
			i--;
			returnFlag= true;
		}
		else if(hexCells.at(i+1).at(j-1).getState()==Cell::p1 && checkedBefore[i+1][j-1]==false){
			checkedBefore[i][j]=true;
			i++;
			j--;
			returnFlag= true;
		}
		else if(hexCells.at(i).at(j-1).getState()==Cell::p1 && checkedBefore[i][j-1]==false){
			checkedBefore[i][j]=true;
			j--;
			returnFlag= true;
		}
		else{
			for(int p=1;p<boardSize+1;p++){
				for(int u=1;u<boardSize+1;u++){
					checkedBefore[p][u]=false;
				}
			}
		}
		if(returnFlag==true){ //if there is 'x' path ahead, proceed to next one.
			return winCheckP1(i,j);
		}
	}
	return false;
}

bool Hex::winCheckP2(int i,int j){
	if(i==1&&j==1){
		while(hexCells.at(1).at(j).getState()!=Cell::p2 && j<boardSize+1){
			j++;
		}
	}
	bool returnFlag=false;
	if(i==boardSize && hexCells.at(i).at(j).getState()==Cell::p2){
		checkedBefore[i][j]=true;
		for(int u=1;u<boardSize+1;u++){
			for(int p=1;p<boardSize+1;p++){
				if(hexCells[u][p].getState()== Cell::p2 && checkedBefore[u][p]==true)
				hexCells[u][p].setState(Cell::p2won);
			}
		}
		printBoard();
		isActive=false;
		return true;
	}
	else if(hexCells.at(i).at(j).getState()==Cell::p2){
		if(hexCells.at(i+1).at(j).getState()==Cell::p2 && checkedBefore[i+1][j]==false){
			checkedBefore[i][j]=true;
			i++;
			returnFlag= true;
		}
		else if(hexCells.at(i+1).at(j-1).getState()==Cell::p2 && checkedBefore[i+1][j-1]==false){
			checkedBefore[i][j]=true;
			i++;
			j--;
			returnFlag= true;
		}
		else if(hexCells.at(i).at(j+1).getState()==Cell::p2 && checkedBefore[i][j+1]==false){
			checkedBefore[i][j]=true;
			j++;
			returnFlag= true;
		}
		else if(hexCells.at(i).at(j-1).getState()==Cell::p2 && checkedBefore[i][j-1]==false){
			checkedBefore[i][j]=true;
			j--;
			returnFlag= true;
		}
		else if(hexCells.at(i-1).at(j+1).getState()==Cell::p2 && checkedBefore[i-1][j+1]==false){
			checkedBefore[i][j]=true;
			i--;
			j++;
			returnFlag= true;
		}
		else if(hexCells.at(i-1).at(j).getState()==Cell::p2 && checkedBefore[i-1][j]==false){
			checkedBefore[i][j]=true;
			i--;
			returnFlag= true;
		}
		else{
			for(int p=1;p<boardSize+1;p++){
				for(int u=1;u<boardSize+1;u++){
					checkedBefore[p][u]=false;
				}
			}
		}
		if(returnFlag==true){ //if there is 'x' path ahead, proceed to next one.
			return winCheckP2(i,j);
		}
	}
	return false;
}

bool Hex::compareWith(Hex game) const{
	int x=0,y=0;
	for(int i=1;i<boardSize+1;i++){
		for(int j=1;j<boardSize+1;j++){
			if(hexCells[i][j].getState()==Cell::p1 || hexCells[i][j].getState()==Cell::p2) x++;
		}
	}
	for(int i=1;i<game.getBoardSize()+1;i++){
			for(int j=1;j<game.getBoardSize()+1;j++){
				if(game.hexCells[i][j].getState()==Cell::p1|| game.hexCells[i][j].getState()==Cell::p2) y++;
			}
	}
	if(x>y) return true;
	else return false;
}
