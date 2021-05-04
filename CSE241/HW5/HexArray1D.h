#ifndef HexArray1D_H
#define HexArray1D_H
#include "AbstractHex.h"

namespace gokbeysHex{
	class HexArray1D : public AbstractHex{
	public:
		HexArray1D():HexArray1D(6,1){/*INTENTIONALLY EMPTY*/}
		explicit HexArray1D(int x):HexArray1D(x,1){/*INTENTIONALLY EMPTY*/}
		HexArray1D(int x,int y);
		HexArray1D(const HexArray1D& other):HexArray1D() {(*this)=other;}
		~HexArray1D();
		const HexArray1D& operator = (const HexArray1D& other); //copy constructor
		virtual Cell operator()(int x,int y)const;
		virtual Cell& operator()(int x,int y);
		virtual void setSize(int x);
	private:
		Cell* hexCells;
	};
}
#endif//HexArray1D_H