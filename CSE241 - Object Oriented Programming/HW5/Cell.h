#ifndef Cell_H
#define Cell_H
#include <iostream>
using namespace std;
namespace gokbeysHex{
	class Cell{
		public:
			Cell(int x,int y);
			Cell() : Cell(0,0){/*Intentionally empty*/}
			enum cellState{empty = '.',p1='x',p2='o',p1won='X',p2won='O'};
			void setColumn(const int& x){column=x;}
			inline const int getColumn() const {return column;}
			void setRow(const int& x){row=x;}
			inline const int getRow() const {return row;}
			inline void setState(const cellState& x){state=x;}
			inline const int getState() const{return state;}
			inline bool isChecked(){return checkedBefore;}
			inline void setChecked(bool x){checkedBefore=x;}
			friend ostream & operator << (ostream &out, const Cell &c);
			bool operator==(const Cell& other);
		private:
			int column;
			int row;
			cellState state;
			bool checkedBefore;
		};
}
#endif //Cell_H