#include "AbstractHex.h"
#include "HexAdapter.h"
#include "HexException.h"
#include <type_traits>

namespace gokbeysHex{
	template<template<typename ...>class container>
	HexAdapter<container>::HexAdapter(int x,int y):AbstractHex(x,y){
		hexCells.resize(boardSize+2);
		for(int i=0;i<boardSize+2;i++){            //initializing the board
			hexCells[i].resize(boardSize+2);
		}
	}
	template<template<typename ...>class container>
	Cell HexAdapter<container>::operator()(int x,int y)const{
		if(x>boardSize+1 || y>boardSize+1 || x<-1 || y<-1){
			HexException he;
			throw he;
		}
		return hexCells[x][y];
	}
	template<template<typename ...>class container>
	Cell& HexAdapter<container>::operator()(int x,int y){
		if(x>boardSize+1 || y>boardSize+1 || x<-1 || y<-1){
			HexException he;
			throw he;
		}
		return hexCells[x][y];
	}
	template<template<typename ...>class container>
	void HexAdapter<container>::setSize(int x){
		reset();
		setBoardSize(x);
		hexCells.resize(boardSize+2);
		for(int i=0;i<boardSize+2;i++){            //initializing the board
			hexCells[i].resize(boardSize+2);
		}
	}
}