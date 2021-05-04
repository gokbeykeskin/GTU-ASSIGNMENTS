#include <iostream>
#include <vector>
#include <array>
#include <deque>
#include <fstream>
#include "HexVector.h"
#include "HexArray1D.h"
#include "HexAdapter.h"
#include "HexAdapter.cpp"
#include "HexException.h"
using namespace std;
using namespace gokbeysHex;

bool isValid(AbstractHex* game){
	if(game->getBoardSize()>=6&&game->getBoardSize()<=26 && (game->getGameMode()==1 || game->getGameMode()==2))
		return true;
	return false;
}

int main(){
	int opt,gm,bs,gmtype,gameopt;//option,game mode,board size, game type(HexVector,HexArray..),gameopt(start to play or compare games).
	unsigned i;
	bool quiter=false;
	vector <AbstractHex*> games; //holds the active hex games.
	AbstractHex* game; //I will add these to games vector one by one after validating.
	cout <<"WELCOME TO THE HEX GAME."<<endl;
	while(quiter==false){
		for(i=0;i<games.size();i++){
			if(games[i]->isEnd()==false){
				games.erase(games.begin()+i);
			}
		}
		cout<<"1. Insert a new game to the active games list"<<endl<<"2. See the active games."<<endl<<"3.Quit"<<endl;
		cin >> opt;
		switch(opt){
		case 1:
			cout <<"TYPE BOARD SIZE (6 for 6x6, 7 for 7x7...):";
			cin >>bs;
			cout<<"1. PLAYER VS PLAYER"<<endl<<"2. PLAYER VS COMPUTER"<<endl;
			cin>>gm;
			cout<<"Which one would you like to create?"<<endl<<"1)Hex game with vector container"<<endl<<"2)Hex game with 1D array container"<<endl<<"3)Hex game with a random access STL container"<<endl;
			cin>>gmtype;

			if(gmtype==1){
				game= new HexVector(bs,gm);
				if(isValid(game)){
					games.push_back(game);
					cout<<"Game is succesfully created. You can see active games in 2nd option."<<endl;
				}
				else
					cout<<"This game configurations are not valid! Game is not created."<<endl;
			}
			else if(gmtype==2){
				game =new HexArray1D(bs,gm);
				if(isValid(game)){
					games.push_back(game);
					cout<<"Game is succesfully created. You can see active games in 2nd option."<<endl;
				}
				else
					cout<<"This game configurations are not valid! Game is not created."<<endl;
			}
			else if(gmtype==3){
				cout<<"1) STL VECTOR"<<"2) STL DEQUE"<<endl;
				cin>>gmtype;
				if(gmtype==1){
					game = new HexAdapter<vector>(bs,gm);
					if(isValid(game)){
						games.push_back(game);
						cout<<"Game is succesfully created. You can see active games in 2nd option."<<endl;
					}
					else
						cout<<"This game configurations are not valid! Game is not created."<<endl;
				}
				else if(gmtype==2){
					game = new HexAdapter<deque>(bs,gm);
					if(isValid(game)){
						games.push_back(game);
						cout<<"Game is succesfully created. You can see active games in 2nd option."<<endl;
					}
					else
						cout<<"This game configurations are not valid! Game is not created."<<endl;
				}
				else
					cout<<"This game configurations are not valid! Game is not created."<<endl;
			}
			else
				cout<<"This game configurations are not valid! Game is not created."<<endl;
			break;
		case 2:
			if(games.size()>0){
				cout<<"There are "<<games.size()<<" active game(s)."<<endl;
				for(i=0;i<games.size();i++){
					cout<<i+1<<')'<<endl; games[i]->print();
					if(games[i]->getGameMode()==1) cout<<"Game Mode:PVP"<<endl;
					else cout<<"Game Mode:PVCOMP"<<endl;
				}
				cout<<"Total moves made in all games:"<<HexVector::getTotalMoves()<<endl;
				cout<<"Game(s) are listed above. Which one would you like to play?(type 0 to go back to main menu.Type 12,23 etc. to compare games marked cell amount.(12 compares 1 and 2)";
				cin>>gameopt;
				if(gameopt>10&& gameopt<100){
					if(games[(gameopt/10)-1]==games[(gameopt%10)-1]) cout<<"These two games are the same"<<endl; // INPUT 12 COMPARES GAME INDEX 0 AND GAME INDEX 1
					else cout<<"They are not same"<<endl;
				}
				else if(gameopt==0)
					break;
				else if(gameopt <= games.size()){
						games[gameopt-1]->playGame();
				}
				else
					cout<<"This game doesn't exist!, There are only "<<games.size()<<" active games"<<endl;
			}
			else cout<<"There is no active game. Create one from to menu to play."<<endl;
			break;
		case 3:
			quiter=true;
			cout<<"Goodbye!"<<endl;
			break;
		default:
			cout<<"This is not a valid option!"<<endl;
			break;
		}
	}
	return 0;
}