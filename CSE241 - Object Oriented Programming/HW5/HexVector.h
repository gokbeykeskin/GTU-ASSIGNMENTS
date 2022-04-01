#ifndef HexVector_H
#define HexVector_H
#include "AbstractHex.h"
#include <vector>

namespace gokbeysHex{
	class HexVector: public AbstractHex{
	public:
		HexVector():HexVector(6,1){/*INTENTIONALLY EMPTY*/}
		HexVector(int x):HexVector(x,1){/*INTENTIONALLY EMPTY*/}
		HexVector(int x,int y);
		virtual Cell operator()(int x,int y)const;
		virtual Cell& operator()(int x,int y);
		virtual void setSize(int x);
	private:
		vector<vector<Cell> > hexCells;
	};
}
#endif//HexVector_H