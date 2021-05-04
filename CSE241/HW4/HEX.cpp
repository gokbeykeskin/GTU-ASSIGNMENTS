#include <iostream>
#include <vector>
#include <fstream>
#include <random>  //BOT STARTS FROM A RANDOM LOCATION
#include <chrono>
#include "HEX.h"
int Hex::totalMoves = 0; //static

void Hex::moveCounter(){  //static
	totalMoves++;
}
int Hex::getTotalMoves(){  //static
	return totalMoves;
}

Hex::Hex(int x,int y){
	isActive=true;
	lastMovesCap=20;
	lastMovesUsed=0;
	score1=0;
	score2=0;
	if(x>5 && x<28)	boardSize = x;
	if(y == 1 || y== 2) 	gameMode=y;
	initializer();
	lastMoves = new Cell*[lastMovesCap];
}
Hex::~Hex(){
	delete[] lastMoves;
	for(int i=0;i<getBoardSize()+2;i++) delete[] hexCells[i];
	delete[] hexCells;
	for(int i=0;i<getBoardSize()+2;i++) delete[] checkedBefore[i];
	delete[] checkedBefore;
}

inline Hex::Cell::Cell(int x,int y){
	column=x;
	row=y;
	state = empty;
}

Hex Hex::operator -- (){   //undo move
	if(lastMovesUsed<=0){
		cout<<"There is no move on the board!"<<endl;
	}
	else{
    	this->lastMoves[--lastMovesUsed]->setState(Cell::empty);
    	cout<<(*this);
	}
	return *this;
}
Hex Hex::operator -- (int){  //undo move
	Hex temp;
	if(lastMovesUsed<=0){
		cout<<"There is no move on the board!"<<endl;
	}
	else{
		temp = *this;
    	this->lastMoves[--lastMovesUsed]->setState(Cell::empty);
    	cout<<(*this);
	}
	return temp;
}

const Hex& Hex::operator = (const Hex& other){ 
	int x,y;

	if(this!=&other){
		for(int i=0;i<getBoardSize()+2;i++){ //delete the old board
			delete hexCells[i];
		}
		delete[] hexCells; //delete the old board
		for(int i=0;i<getBoardSize()+2;i++){
			delete checkedBefore[i];  //delete the old winCheck list
		}
		delete[] checkedBefore; //delete the old winCheck list
		delete[] lastMoves;   //delete the old undo list
		boardSize=other.boardSize;  
		gameMode=other.gameMode;
		isActive=other.isActive;
		lastMovesCap=other.lastMovesCap; 
		lastMovesUsed=other.lastMovesUsed;
		initializer();  //initialize the board for the new boardSize
		for(int i=0;i<getBoardSize()+2;i++){
			for(int j=0;j<getBoardSize()+2;j++){
				hexCells[i][j]=other.hexCells[i][j]; //fill the board with new values
			}
		}
		for(int i=0;i<getBoardSize()+2;i++){
			for(int j=0;j<getBoardSize()+2;j++){
				checkedBefore[i][j]=other.checkedBefore[i][j]; //fill the winCheck list with new values
			}
		}
		lastMoves = new Cell*[lastMovesCap];  //initialize lastMoves for new Capacity.
		for(int i=0;i<lastMovesUsed;i++){
			x=other.lastMoves[i]->getColumn();
			y=other.lastMoves[i]->getRow();
			lastMoves[i]=&hexCells[x][y];
		}
	}

	return *this;
}
ofstream& operator<<(ofstream& saveFile,const Hex &game ){
	if(saveFile.is_open()){
		saveFile<<game.boardSize;
		saveFile<<"-";
		saveFile<<game.gameMode;
		saveFile<<"-";
		for(int i=0;i<game.boardSize;i++){
			for(int j=0;j<game.boardSize;j++){
				saveFile<<static_cast<char>(game.hexCells[i][j].getState());
			}
		}
	}
	else
		cerr<<"An error has been occured while opening the file.";

	return saveFile;
}

ostream& operator<<(ostream &printer,const Hex& game){
printer << "   "; //spaces between horizontal board indexes (A B C...)
	for(int i=0;i<game.boardSize;i++){
		printer << char(97+i)<<' '; //ASCII number of letter a is 97. So this line creates the horizontal indexes(a b c d e...).
	}
	for(int i=1;i<game.boardSize+1;i++){
		printer << "\n";
		if(i<10)
			cout <<' ';
		printer << i << ' ';
		for(int k=-1;k<i-1;k+=1){ //this loop is for making the diamond shape. (shifting the dots for each cycle.)
			printer << " ";
		}
		for(int j=1;j<game.boardSize+1;j++){  //this loop prints the board.
			printer << static_cast<char>(game.hexCells[i][j].getState())<<' ';
		}
	}
	printer << "\n";
	return printer;
}

ifstream& operator>>(ifstream &loadFile,Hex& game){
	auto k=0;
	string gameSave;
	if(loadFile.is_open()){
		loadFile>>gameSave;
		game.boardSize = stoi(gameSave.substr(0,gameSave.find('-')));
		game.gameMode = stoi(gameSave.substr(gameSave.find('-')+1,gameSave.find('-')));
		while(gameSave[k]!='.' && gameSave[k]!='x' && gameSave[k]!='o') //find where saved board starts.
			k++;	

		
		for(int i=0;i<game.boardSize;i++){
			for(int j=0;j<game.boardSize;j++){
					game.hexCells[i][j].setState(static_cast<Hex::Cell::cellState>(gameSave[k]));
					k++;
			}
		}
	}
		else
			cerr<<"An error has been occured while opening the file.";
	return loadFile;
}

const bool operator==(const Hex& game1,const Hex& game2){ //returns 1 if they have equal amount of marked cells
	int x=0,y=0;
	for(int i=1;i<game1.boardSize+1;i++){
		for(int j=1;j<game1.boardSize+1;j++){
			if(game1.hexCells[i][j].getState()==Hex::Cell::p1 || game1.hexCells[i][j].getState()==Hex::Cell::p2) x++;
		}
	}
	for(int i=1;i<game2.getBoardSize()+1;i++){
			for(int j=1;j<game2.getBoardSize()+1;j++){
				if(game2.hexCells[i][j].getState()==Hex::Cell::p1|| game2.hexCells[i][j].getState()==Hex::Cell::p2) y++;
			}
	}
	if(x==y) return true;
	else return false;
}

ostream & operator << (ostream &out, const Hex::Cell &c){ //prints the last move. (used for printing bot's move at playGame function.)
	out << static_cast<char>(c.row+96);
	out << c.column<< endl; 
	return out;
}

void Hex::initializer(){
	hexCells = new Cell*[boardSize+2];
	for(int i=0;i<boardSize+2;i++){            //initializing the board
		hexCells[i]= new Cell[boardSize+2];
	}
	checkedBefore= new bool*[boardSize+2];
	for(int i=0;i<boardSize+2;i++){            //initializing the checkedBefore array
		checkedBefore[i]=new bool(boardSize+2);
		for(int j=0;j<boardSize+2;j++){
			checkedBefore[i][j]=false; 
		}
	}
}

inline void Hex::printBoardSize() const{
	cout<<"Width: " << getBoardSize() << "Height:" << getBoardSize()<<endl;
}

void Hex::playGame(){  //play functions and wincheck
	bool gameEnded=false,wrongMove=false;
	string move;
	cin.ignore();
	if(gameMode==1){ //pvp
		cout<<(*this);
		while(gameEnded==false){
			do{
				cout<<"Make your move(1st player)(or type SAVE location.txt or LOAD location.txt or type quit to exit or type undo to undo last move.): ";
			    getline(cin,move);
				if(move.substr(0,4)=="SAVE"){
					ofstream out;
					out.open(move.substr(5));
					out<<(*this);
					out.close();
					cout<<(*this);
					wrongMove=true;
				}
				else if(move.substr(0,4)=="LOAD"){
					ifstream in;
					in.open(move.substr(5));
					in>>(*this);
					in.close();
					cout<<(*this);
					wrongMove=true;
				}
				else if(move=="quit"){
					return;
				}
				else if(move=="undo"){
					(*this)--;
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
			do{
				cout<<"Make your move(2nd player)(or type SAVE location.txt or LOAD location.txt or type quit to exit or type undo to undo last move.):";
				getline(cin,move);
				if(move.substr(0,4)=="SAVE"){
					ofstream out;
					out.open(move.substr(5));
					out<<(*this);
					out.close();
					cout<<(*this);
					wrongMove=true;
				}
				else if(move.substr(0,4)=="LOAD"){
					ifstream in;
					in.open(move.substr(5));
					in>>(*this);
					in.close();
					cout<<(*this);
					wrongMove=true;
				}
				else if(move=="quit"){
					return;
				}
				else if(move=="undo"){
					(*this)--;
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
			cout<<"BOT'S MOVE:"<<play();
			if(winCheckP1(1,1)==true){
				cout<<"Computer has Won!"<<endl;
				return;
			}
			cout<<(*this);
			do{
				cout<<"Instead of making a move you can type: SAVE location.txt or LOAD location.txt or type quit to exit or type undo to undo the last move."<<endl;
				cout<<"Make your move(2nd player);";
				getline(cin,move);
				if(move.substr(0,4)=="SAVE"){
					ofstream out;
					out.open(move.substr(5));
					out<<(*this);
					out.close();
					cout<<(*this);
					wrongMove=true;
				}
				else if(move.substr(0,4)=="LOAD"){
					ifstream in;
					in.open(move.substr(5));
					in>>(*this);
					in.close();
					cout<<(*this);
					wrongMove=true;
				}
				else if(move=="quit"){
					return;
				}
				else if(move=="undo"){
					(*this)--;
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

Hex::Cell Hex::play(){
	unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count(); //create a seed for random num generation
	static std::default_random_engine eng(seed);
	int botMoveY;
	int botMoveX;
	do{
		botMoveY = eng() % (boardSize+1);
		botMoveX = eng() % (boardSize+1);
	}while(hexCells[botMoveX][botMoveY].getState() != Cell::empty || botMoveX==0 || botMoveY==0);
	hexCells[botMoveX][botMoveY].setState(Cell::p1);
	hexCells[botMoveX][botMoveY].setColumn(botMoveX);
	hexCells[botMoveX][botMoveY].setRow(botMoveY);

	if(lastMovesUsed==lastMovesCap){ //if lastMoves pointer is full, add new elements to it.
		lastMovesCap*=2;
		Cell** temp=new Cell*[lastMovesCap];
		for(int i=0;i<lastMovesUsed;i++){
			temp[i] = lastMoves[i];
		}
		delete[] lastMoves;
		lastMoves = temp;
	}

	lastMoves[lastMovesUsed]=&hexCells[botMoveX][botMoveY];  //save the last move;
	lastMovesUsed++;
	moveCounter();
	return hexCells[botMoveX][botMoveY];

}
	
void Hex::play(int yMove,int xMove,int player){//if player is 1 puts 'x', if it is 2 puts 'o'
	if(player==1){
		hexCells[xMove][yMove].setState(Cell::p1);
		if(lastMovesUsed>1){
			if(yMove==(lastMoves[lastMovesUsed-2]->getRow())+1) //if the move is to the right or upright, increment the score. (-2 because -1 is other player)
				incrementScore1();
		}
		cout<<(*this);
	}
	else if(player==2){
		hexCells[xMove][yMove].setState(Cell::p2);
		cout<<(*this);
		if(lastMovesUsed>1){
			if(xMove==(lastMoves[lastMovesUsed-2]->getColumn())+1) //if the move is to the bottom or leftbottom, increment the score. (-2 because -1 is other player)
				incrementScore2();
		}
	}
	hexCells[xMove][yMove].setColumn(xMove);
	hexCells[xMove][yMove].setRow(yMove);
	if(lastMovesUsed==lastMovesCap){ //if lastMoves pointer is full, add new elements to it.
		lastMovesCap*=2;
		Cell** temp=new Cell*[lastMovesCap];
		for(int i=0;i<lastMovesUsed;i++){
			temp[i] = lastMoves[i];
		}
		delete[] lastMoves;
		lastMoves = temp;
	}

	lastMoves[lastMovesUsed]=&hexCells[xMove][yMove];  //save the last move;
	lastMovesUsed++;
	moveCounter();
	
	
}

bool Hex::winCheckP1(int i,int j){
	if(i==1&&j==1){
		while(hexCells[i][1].getState()!=Cell::p1 && i<boardSize+1){
			i++;
		}
	}
	bool returnFlag=false;
	if(j==boardSize && hexCells[i][j].getState()==Cell::p1){
		checkedBefore[i][j]=true;
		for(int u=1;u<boardSize+1;u++){
			for(int p=1;p<boardSize+1;p++){
				if(hexCells[u][p].getState()== Cell::p1 && checkedBefore[u][p]==true)
				hexCells[u][p].setState(Cell::p1won);
			}
		}
		cout<<(*this);
		isActive = false;
		return true;
	}
	else if(hexCells[i][j].getState()==Cell::p1){
		if(hexCells[i][j+1].getState()==Cell::p1 && checkedBefore[i][j+1]==false){
			checkedBefore[i][j]=true;
			j++;
			returnFlag= true;
		}
		else if(hexCells[i-1][j+1].getState()==Cell::p1 && checkedBefore[i-1][j+1]==false){
			checkedBefore[i][j]=true;
			i--;
			j++;
			returnFlag= true;
		}
		else if(hexCells[i+1][j].getState()==Cell::p1 && checkedBefore[i+1][j]==false){
			checkedBefore[i][j]=true;
			i++;
			returnFlag= true;
		}
		else if(hexCells[i-1][j].getState()==Cell::p1 && checkedBefore[i-1][j]==false){
			checkedBefore[i][j]=true;
			i--;
			returnFlag= true;
		}
		else if(hexCells[i+1][j-1].getState()==Cell::p1 && checkedBefore[i+1][j-1]==false){
			checkedBefore[i][j]=true;
			i++;
			j--;
			returnFlag= true;
		}
		else if(hexCells[i][j-1].getState()==Cell::p1 && checkedBefore[i][j-1]==false){
			checkedBefore[i][j]=true;
			j--;
			returnFlag= true;
		}
		else{
			for(int p=1;p<boardSize+1;p++){
				for(int u=1;u<boardSize+1;u++){
					checkedBefore[p][u]=false;
					//score=0;
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
		while(hexCells[1][j].getState()!=Cell::p2 && j<boardSize+1){
			j++;
		}
	}
	bool returnFlag=false;
	if(i==boardSize && hexCells[i][j].getState()==Cell::p2){
		checkedBefore[i][j]=true;
		for(int u=1;u<boardSize+1;u++){
			for(int p=1;p<boardSize+1;p++){
				if(hexCells[u][p].getState()== Cell::p2 && checkedBefore[u][p]==true)
				hexCells[u][p].setState(Cell::p2won);
			}
		}
		cout<<(*this);
		isActive=false;
		return true;
	}
	else if(hexCells[i][j].getState()==Cell::p2){
		if(hexCells[i+1][j].getState()==Cell::p2 && checkedBefore[i+1][j]==false){
			checkedBefore[i][j]=true;
			i++;
			returnFlag= true;
		}
		else if(hexCells[i+1][j-1].getState()==Cell::p2 && checkedBefore[i+1][j-1]==false){
			checkedBefore[i][j]=true;
			i++;
			j--;
			returnFlag= true;
		}
		else if(hexCells[i][j+1].getState()==Cell::p2 && checkedBefore[i][j+1]==false){
			checkedBefore[i][j]=true;
			j++;
			returnFlag= true;
		}
		else if(hexCells[i][j-1].getState()==Cell::p2 && checkedBefore[i][j-1]==false){
			checkedBefore[i][j]=true;
			j--;
			returnFlag= true;
		}
		else if(hexCells[i-1][j+1].getState()==Cell::p2 && checkedBefore[i-1][j+1]==false){
			checkedBefore[i][j]=true;
			i--;
			j++;
			returnFlag= true;
		}
		else if(hexCells[i-1][j].getState()==Cell::p2 && checkedBefore[i-1][j]==false){
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
		if(returnFlag==true){ //if there is 'o' path ahead, proceed to next one.
			return winCheckP2(i,j);
		}
	}
	return false;
}