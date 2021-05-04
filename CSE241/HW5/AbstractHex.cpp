#include "AbstractHex.h"
#include "HexException.h"
#include <iostream>
#include <fstream>
#include <random>  //BOT STARTS FROM A RANDOM LOCATION
#include <chrono>

namespace gokbeysHex{
	int AbstractHex::totalMoves=0;
	AbstractHex::AbstractHex(int x,int y){
		isActive=true;
		score1=0;
		score2=0;
		last=Cell(0,0);
		if(x>5 && x<28)	boardSize = x;
		if(y == 1 || y== 2) 	gameMode=y;
	}

	ifstream& AbstractHex::readFromFile(ifstream &loadFile){
		auto k=0;
		string gameSave;
		if(loadFile.is_open()){
			loadFile>>gameSave;
			boardSize = stoi(gameSave.substr(0,gameSave.find('-')));
			gameMode = stoi(gameSave.substr(gameSave.find('-')+1,gameSave.find('-')));
			while(gameSave[k]!='.' && gameSave[k]!='x' && gameSave[k]!='o') //find where saved board starts.
				k++;	
			for(int i=0;i<boardSize;i++){
				for(int j=0;j<boardSize;j++){
						(*this)(i,j).setState(static_cast<Cell::cellState>(gameSave[k]));
						k++;
				}
			}
		}
		else
			cerr<<"An error has been occured while opening the file.";
		return loadFile;
	}

	ofstream& AbstractHex::writeToFile(ofstream &saveFile){
		if(saveFile.is_open()){
		saveFile<<boardSize;
		saveFile<<"-";
		saveFile<<gameMode;
		saveFile<<"-";
		for(int i=0;i<boardSize;i++){
			for(int j=0;j<boardSize;j++){
				saveFile<<static_cast<char>((*this)(i,j).getState());
			}
		}
	}
	else
		cerr<<"An error has been occured while opening the file.";

	return saveFile;
	}

	void AbstractHex::reset(){
		for(int i=0;i<boardSize;i++){
			for(int j=0;j<boardSize;j++){
				(*this)(i,j).setState(static_cast<Cell::cellState>(Cell::empty));
			}
		}
	}

	bool AbstractHex::operator==(const AbstractHex& other){
		for(int i=1;i<boardSize+1;i++){
			for(int j=1;j<other.getBoardSize()+1;j++){
				if((*this)(i,j).getState()!=other(i,j).getState()) return false;
			}
		}
		return true;
	}

	Cell AbstractHex::lastMove(){
		if(last==Cell(0,0)){
			HexException2 he2;
			throw  he2;
		}
		return last;

	}

	ostream& operator<<(ostream &printer,const AbstractHex& game){
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
				printer << static_cast<char>(game(i,j).getState())<<' '; //using the operator() overload
			}
		}
		printer << "\n";
		return printer;
	}

Cell AbstractHex::play(){
		unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count(); //create a seed for random num generation
		static std::default_random_engine eng(seed);
		int botMoveY;
		int botMoveX;
		do{
			botMoveY = eng() % (boardSize+1);
			botMoveX = eng() % (boardSize+1);
		}while((*this)(botMoveX,botMoveY).getState() != Cell::empty || botMoveX==0 || botMoveY==0); //botmovelara 1 ekleyip whileı kısalt!!!!
		(*this)(botMoveX,botMoveY).setState(Cell::p1);
		moveCounter();
		last.setColumn(botMoveX);last.setRow(botMoveY);
		return last;
	}

	void AbstractHex::play(int yMove,int xMove,int player){//if player is 1 puts 'x', if it is 2 puts 'o'
		if(player==1){
		(*this)(xMove,yMove).setState(Cell::p1);
		last.setColumn(xMove);last.setRow(yMove);
		print();
		}
		else if(player==2){
			(*this)(xMove,yMove).setState(Cell::p2);
			last.setColumn(xMove);last.setRow(yMove);
			print();
		}
	moveCounter();
	}

	void AbstractHex::playGame(){  //play functions and wincheck
		bool wrongMove=false;
		string move;
		cin.ignore();
		if(gameMode==1){ //pvp
			print();
			while(1){
				do{
					try{
						cout<<"Other players last move: "<<lastMove()<<endl;
					}
					catch(HexException2 D){
						cout<<"No last moves."<<endl;
					}
					cout<<"Instead of making a move you can type: SAVE location.txt or LOAD location.txt or SETSIZE num or type reset or type quit "<<endl;
					cout<<"Make your move(1st player): ";
				    getline(cin,move);
				    wrongMove=false;
					if(move.substr(0,4)=="SAVE"){
						ofstream out;
						out.open(move.substr(5));
						writeToFile(out);
						out.close();
						print();
						wrongMove=true;
					}
					else if(move.substr(0,4)=="LOAD"){
						ifstream in;
						in.open(move.substr(5));
						readFromFile(in);
						in.close();
						print();
						wrongMove=true;
					}
					else if(move.substr(0,7)=="SETSIZE"){
						setSize(stoi(move.substr(8,move.find('\0'))));
						print();
						wrongMove=true;
					}
					else if(move=="reset"){
						reset();
						print();
						wrongMove=true;
					}
					else if(move=="quit"){
						return;
					}
					if(wrongMove!=true){
						int yMove = static_cast<int>(move[0])-96;
						int xMove;
						try{
							xMove = stoi(move.substr(1));
						}
						catch(std::invalid_argument){
							cout<<"This is not a valid input!"<<endl;
						}
						try{
							if((*this)(xMove,yMove).getState() == Cell::empty){
								play(yMove,xMove,1);
								wrongMove=false;
							}
							else{
								wrongMove=true;
								print();
								cout<<"This place is already occupied!";
							}
							if(winCheckP1(1,1)==true){
								cout<<"Player 1 Won!"<<endl;
								return;
							}
						}
						catch(HexException E){
							print();
							cout<<"PLEASE MAKE A VALID MOVE!"<<endl;
							wrongMove=true;
						}
					}
				}while(wrongMove==true);
				do{
					try{
						cout<<"Other players last move: "<<lastMove()<<endl;
					}
					catch(HexException2 D){
						cout<<"No last moves."<<endl;
					}
					cout<<"Instead of making a move you can type: SAVE location.txt or LOAD location.txt or SETSIZE num or type reset or type quit "<<endl;
					cout<<"Make your move(2nd player):";
					getline(cin,move);
					wrongMove=false;
					if(move.substr(0,4)=="SAVE"){
						ofstream out;
						out.open(move.substr(5));
						writeToFile(out);
						out.close();
						print();
						wrongMove=true;
					}
					else if(move.substr(0,4)=="LOAD"){
						ifstream in;
						in.open(move.substr(5));
						readFromFile(in);
						in.close();
						print();
						wrongMove=true;
					}
					else if(move.substr(0,7)=="SETSIZE"){
							cout<<move.substr(8,move.find('\0'));
							setSize(stoi(move.substr(8,move.find('\0'))));
							print();
							wrongMove=true;
						}
					else if(move=="reset"){
						reset();
						print();
						wrongMove=true;
					}
					else if(move=="quit"){
						return;
					}
					if(wrongMove!=true){
						int yMove = static_cast<int>(move[0])-96;
						int xMove;
						try{
							xMove = stoi(move.substr(1));
						}
						catch(std::invalid_argument){
							cout<<"This is not a valid input!"<<endl;
						}
						try{
							if((*this)(xMove,yMove).getState() == Cell::empty){
								play(yMove,xMove,2);
								wrongMove=false;
							}
							else{
								wrongMove=true;
								print();
								cout<<"This place is already occupied!";
							}
							if(winCheckP2(1,1)==true){
								cout<<"Player 2 Won!"<<endl;
								return;
							}
						}
						catch(HexException E){
							print();
							cout<<"PLEASE MAKE A VALID MOVE!"<<endl;
							wrongMove=true;
						}
					}
				}while(wrongMove==true);
			}
		}
		else if(gameMode==2){
			while(1){
				play();
				if(winCheckP1(1,1)==true){
					cout<<"Computer has Won!"<<endl;
					return;
				}
				print();
				do{
					cout<<"BOT's last move: "<<lastMove()<<endl;
					cout<<"Instead of making a move you can type: SAVE location.txt or LOAD location.txt or SETSIZE num or type reset or type quit "<<endl;
					cout<<"Make your move(2nd player):";
					getline(cin,move);
					wrongMove=false;
						if(move.substr(0,4)=="SAVE"){
							ofstream out;
							out.open(move.substr(5));
							writeToFile(out);
							out.close();
							print();
							wrongMove=true;
						}
						else if(move.substr(0,4)=="LOAD"){
							ifstream in;
							in.open(move.substr(5));
							readFromFile(in);
							in.close();
							print();
							wrongMove=true;
						}
						else if(move.substr(0,7)=="SETSIZE"){
							setSize(stoi(move.substr(8,move.find('\0'))));
							print();
							wrongMove=true;
						}
						else if(move=="reset"){
							reset();
							print();
							wrongMove=true;
						}
						else if(move=="quit"){
						return;
						}
						if(wrongMove!=true){
							int yMove = static_cast<int>(move[0])-96;
							int xMove;
							try{
							xMove = stoi(move.substr(1));
							}
							catch(std::invalid_argument){
								cout<<"This is not a valid input!"<<endl;
							}
							try{
								if((*this)(xMove,yMove).getState() == Cell::empty){
									play(yMove,xMove,2);
									wrongMove=false;
								}
								else{
									wrongMove=true;
									print();
									cout<<"This place is already occupied!";
								}
								if(winCheckP2(1,1)==true){
									cout<<"Player Won!"<<endl;
									return;
								}
							}
							catch(HexException E){
								print();
								cout<<"PLEASE MAKE A VALID MOVE!"<<endl;
								wrongMove=true;
							}
						}
				}while(wrongMove==true);
			}
		}
	}

	void AbstractHex::print(){
		cout<<(*this);
	}
	inline void AbstractHex::printBoardSize() const{
		cout<<"Width: " << getBoardSize() << "Height:" << getBoardSize()<<endl;
	}

	void AbstractHex::moveCounter(){  //static
		totalMoves++;
	}

	int AbstractHex::getTotalMoves(){  //static
		return totalMoves;
	}
	
	bool AbstractHex::winCheckP1(int i,int j){
		if(i==1&&j==1){
			while((*this)(i,j).getState()!=Cell::p1 && i<boardSize+1){
				i++;
			}
		}
		bool returnFlag=false;
		if(j==boardSize && (*this)(i,j).getState()==Cell::p1){
			(*this)(i,j).setChecked(true);
			for(int u=1;u<boardSize+1;u++){
				for(int p=1;p<boardSize+1;p++){
					if((*this)(u,p).getState()== Cell::p1 && (*this)(u,p).isChecked()==true)
					(*this)(u,p).setState(Cell::p1won);
				}
			}
			print();
			isActive = false;
			return true;
		}
		else if((*this)(i,j).getState()==Cell::p1){
			if(i>boardSize+1 || j>boardSize+1)
				return false;
			if((*this)(i,j+1).getState()==Cell::p1 && (*this)(i,j+1).isChecked()==false){
				(*this)(i,j).setChecked(true);
				j++;
				returnFlag= true;
			}
			else if((*this)(i-1,j+1).getState()==Cell::p1 && (*this)(i-1,j+1).isChecked()==false){
				(*this)(i,j).setChecked(true);
				i--;
				j++;
				returnFlag= true;
			}
			else if((*this)(i+1,j).getState()==Cell::p1 && (*this)(i+1,j).isChecked()==false){
				(*this)(i,j).setChecked(true);
				i++;
				returnFlag= true;
			}
			else if((*this)(i-1,j).getState()==Cell::p1 && (*this)(i-1,j).isChecked()==false){
				(*this)(i,j).setChecked(true);
				i--;
				returnFlag= true;
			}
			else if((*this)(i+1,j-1).getState()==Cell::p1 && (*this)(i+1,j-1).isChecked()==false){
				(*this)(i,j).setChecked(true);
				i++;
				j--;
				returnFlag= true;
			}
			else if((*this)(i,j-1).getState()==Cell::p1 && (*this)(i,j-1).isChecked()==false){
				(*this)(i,j).setChecked(true);
				j--;
				returnFlag= true;
			}
			else{
				for(int p=1;p<boardSize+1;p++){
					for(int u=1;u<boardSize+1;u++){
						(*this)(p,u).setChecked(false);
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

	bool AbstractHex::winCheckP2(int i,int j){
		if(i==1&&j==1){
			while((*this)(1,j).getState()!=Cell::p2 && j<boardSize+1){
				j++;
			}
		}
		bool returnFlag=false;
		if(i==boardSize && (*this)(i,j).getState()==Cell::p2){
			(*this)(i,j).setChecked(true);
			for(int u=1;u<boardSize+1;u++){
				for(int p=1;p<boardSize+1;p++){
					if((*this)(u,p).getState()== Cell::p2 && (*this)(u,p).isChecked()==true)
					(*this)(u,p).setState(Cell::p2won);
				}
			}
			print();
			isActive=false;
			return true;
		}
		else if((*this)(i,j).getState()==Cell::p2){
			if(i>boardSize+1 || j>boardSize+1)
				return false;
			if((*this)(i+1,j).getState()==Cell::p2 && (*this)(i+1,j).isChecked()==false){
				(*this)(i,j).setChecked(true);
				i++;
				returnFlag= true;
			}
			else if((*this)(i+1,j-1).getState()==Cell::p2 && (*this)(i+1,j-1).isChecked()==false){
				(*this)(i,j).setChecked(true);
				i++;
				j--;
				returnFlag= true;
			}
			else if((*this)(i,j+1).getState()==Cell::p2 && (*this)(i,j+1).isChecked()==false){
				(*this)(i,j).setChecked(true);
				j++;
				returnFlag= true;
			}
			else if((*this)(i,j-1).getState()==Cell::p2 && (*this)(i,j-1).isChecked()==false){
				(*this)(i,j).setChecked(true);
				j--;
				returnFlag= true;
			}
			else if((*this)(i-1,j+1).getState()==Cell::p2 && (*this)(i-1,j+1).isChecked()==false){
				(*this)(i,j).setChecked(true);
				i--;
				j++;
				returnFlag= true;
			}
			else if((*this)(i-1,j).getState()==Cell::p2 && (*this)(i-1,j).isChecked()==false){
				(*this)(i,j).setChecked(true);
				i--;
				returnFlag= true;
			}
			else{
				for(int p=1;p<boardSize+1;p++){
					for(int u=1;u<boardSize+1;u++){
						(*this)(p,u).setChecked(false);
					}
				}
			}
			if(returnFlag==true){ //if there is 'o' path ahead, proceed to next one.
				return winCheckP2(i,j);
			}
		}
		return false;
	}
}