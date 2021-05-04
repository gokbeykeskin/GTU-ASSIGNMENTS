#include "AbstractHex.h"
#include "HexVector.h"
#include "HexException.h"


namespace gokbeysHex{
	HexVector::HexVector(int x,int y):AbstractHex(x,y){
		hexCells.resize(boardSize+2);
			for(int i=0;i<boardSize+2;i++){            //initializing the board
				hexCells[i].resize(boardSize+2);
			}
	}

	Cell HexVector::operator()(int x,int y)const{
		if(x>boardSize+1 || y>boardSize+1 || x<-1 || y<-1){
			HexException he;
			cout<<x<<y<<endl;
			throw he;
		}
			return hexCells[x][y];
	}
	Cell& HexVector::operator()(int x,int y){
		if(x>boardSize+1 || y>boardSize+1 || x<-1 || y<-1){
			HexException he;
			cout<<x<<y<<endl;
			throw he;
		}
		return hexCells[x][y];
	}
	void HexVector::setSize(int x){
		reset();
		setBoardSize(x);
		hexCells.resize(boardSize+2);
		for(int i=0;i<boardSize+2;i++){            //initializing the board
			hexCells[i].resize(boardSize+2);
		}
	}
	
}
