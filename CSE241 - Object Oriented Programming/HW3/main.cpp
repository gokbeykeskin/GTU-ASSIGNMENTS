#include <iostream>
#include <vector>
#include <fstream>
#include "HEX.h"

using namespace std;


int main(){
	int opt,gm,bs,gameopt;
	unsigned i;
	bool quiter=false;
	vector <Hex> games; //holds the active hex games.
	cout <<"WELCOME TO THE HEX GAME."<<endl;
	while(quiter==false){
		for(i=0;i<games.size();i++){
			if(games[i].getisActive()==false){
				games.erase(games.begin()+i);
			}
		}
		cout<<"1. Insert a new game to the active games list"<<endl<<"2. See the active games."<<endl<<"3.Quit"<<endl;
		cin >> opt;
		switch(opt){
		case 1:
			cout <<"TYPE BOARD SIZE (6 for 6x6, 7 for 7x7...):";
			do{
				cin >>bs;
				if(bs<6 || bs>27) cout <<"Board size should be between 6 and 27. Input another board size:";
			}while(bs<6 || bs>27);
			cout<<"1. PLAYER VS PLAYER"<<endl<<"2. PLAYER VS COMPUTER"<<endl;
			cin>>gm;
			games.push_back(Hex(bs,gm));
			cout<<"Game is succesfully created. You can see active games in 2nd option."<<endl;
			break;
		case 2:
			if(games.size()>0){
				cout<<"There are "<<games.size()<<" active game(s)."<<endl;
				for(i=0;i<games.size();i++){
					cout<<i+1<<')'<<endl; games[i].printBoard();
					if(games[i].getGameMode()==1) cout<<"Game Mode:PVP"<<endl;
					else cout<<"Game Mode:PVCOMP"<<endl;
				}
				cout<<"Total moves made in all games:"<<Hex::getTotalMoves()<<endl;
				cout<<"Game(s) are listed above. Which one would you like to play?(type 0 to go back to main menu.Type 12,23 etc. to compare games marked cell amount.(12 compares 1 and 2)";
				cin>>gameopt;
				if(gameopt>10){
					bool isBigger = games[(gameopt/10)-1].compareWith(games[(gameopt%10)-1]);
					if(isBigger==true) cout<<"First game has more marked USER cells."<<endl;
					else cout<<"Second Game has more marked USER cells or it's a tie."<<endl;
				}
				if(gameopt != 0) games[gameopt-1].playGame();
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