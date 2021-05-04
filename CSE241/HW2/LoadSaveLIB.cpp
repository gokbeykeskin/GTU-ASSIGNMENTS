#include <iostream>
#include <fstream>
#include "LoadSaveLIB.h"
using namespace std;

void saveGame(char Board[12][12],const string& fileName,const int boardSize,const int gameMode){
	ofstream saveFile;
	saveFile.open(fileName);
	if(saveFile.is_open()){
		saveFile<<boardSize<<'-';
		saveFile<<gameMode<<'-';
		for(int i=0;i<boardSize;i++){
			for(int j=0;j<boardSize;j++){
				saveFile<<Board[i][j];
			}
		}
		saveFile.close();
	}
	else
		cerr<<"An error has been occured while opening the file.";
}

void loadGame(char Board[12][12],const string& fileName, int& boardSize,int& gameMode){
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
					Board[i][j]=gameSave[k];
					k++;
			}
		}
		loadFile.close();
	}
		else
			cerr<<"An error has been occured while opening the file.";
}
