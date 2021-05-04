#include "Cell.h"
#include <iostream>

namespace gokbeysHex{
	Cell::Cell(int x,int y){
		column=x;
		row=y;
		state = empty;
		checkedBefore=false;
	}

	ostream & operator << (ostream &out, const Cell &c){ //prints the last move. (used for printing bot's move at playGame function.)
		out << static_cast<char>(c.row+96);
		out << c.column<< endl; 
		return out;
	}

	bool Cell::operator==(const Cell& other){
		if(column==other.column && row==other.row){
			return true;
		}
		return false;
	}
}