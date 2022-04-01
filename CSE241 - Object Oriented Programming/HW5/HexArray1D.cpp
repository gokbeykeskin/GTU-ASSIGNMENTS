#include "AbstractHex.h"
#include "HexArray1D.h"
#include "HexException.h"

namespace gokbeysHex{
	HexArray1D::HexArray1D(int x,int y):AbstractHex(x,y){
		hexCells=new Cell[(boardSize+2)*(boardSize+2)];
	}
	Cell HexArray1D::operator()(int x,int y)const{
		if(x>boardSize+2 || y>boardSize+2 || x<-1 || y<-1){
				HexException he;
				throw he;
		}
		return hexCells[x*boardSize+y];
	}
	Cell& HexArray1D::operator()(int x,int y){
		if(x>boardSize+2 || y>boardSize+2 || x<-1 || y<-1){
				HexException he;
				throw he;
		}
		return hexCells[x*boardSize+y];
	}
	HexArray1D::~HexArray1D(){
		delete[] hexCells;
	}
	const HexArray1D& HexArray1D::operator = (const HexArray1D& other){
		if(this!=&other){
			delete[] hexCells;
			boardSize=other.boardSize;
			gameMode=other.gameMode;
			isActive=other.isActive;
			score1=other.score1;
			score2=other.score2;
			last=other.last;
			hexCells=new Cell[(boardSize+2)*(boardSize+2)];
			for(int i=0;i<(boardSize+2)*(boardSize+2);i++){
				hexCells[i]=other.hexCells[i]; //fill the board with new values
			}
		}
		return (*this);
	}
	void HexArray1D::setSize(int x){
		reset();
		setBoardSize(x);
		delete[] hexCells;
		hexCells=new Cell[(boardSize+2)*(boardSize+2)];

	}
}