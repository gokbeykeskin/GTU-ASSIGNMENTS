#ifndef HexAdapter_H
#define HexAdapter_H
#include "AbstractHex.h"

namespace gokbeysHex{
	template<template<typename ...>class container>
	class HexAdapter:public AbstractHex{
	public:
		HexAdapter():HexAdapter(6,1){/*INTENTIONALLY EMPTY*/}
		HexAdapter(int x):HexAdapter(x,1){/*INTENTIONALLY EMPTY*/}
		HexAdapter(int x,int y);
		virtual Cell operator()(int x,int y)const;
		virtual Cell& operator()(int x,int y);
		virtual void setSize(int x);
	private:
		container<container<Cell> > hexCells;
	};

}
#endif //HexAdapter_H