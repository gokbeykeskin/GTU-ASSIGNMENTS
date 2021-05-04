#ifndef HEX_H
#define HEX_H

#include <string>
#include <vector>
using namespace std;


class Hex{
public:
	Hex() : Hex(6,1){/*Intentionally empty*/};
	Hex(int x) : Hex(x,1){/*Intentionally empty */} //boardSize
	Hex(int x,int y); //boardSize,gameMode;
	inline void setBoardSize(int x){if(x>5) boardSize = x;}
	inline int getBoardSize() const{return boardSize;}
	void printBoardSize() const;
	void saveGame(const string& fileName);
	void loadGame(const string& fileName);
	void printBoard() const;
	void playGame();
	void play(); //takes 1 move from the bot
	void play(int xMove,int yMove,int player); //if player is 1 puts 'x', if it is 2 puts 'o'
	bool winCheckP1(int i,int j);
	bool winCheckP2(int i,int j);
	inline void setGameMode(int x){if(x ==1 || x==2) gameMode = x;}
	inline int getGameMode() const {return gameMode;}
	friend Hex addNewGame();
	static void moveCounter();
	static int  getTotalMoves();
	inline bool getisActive() const {return isActive;}
	bool compareWith(Hex game) const;
private:
	int boardSize;
	int gameMode; //if 1 PVP, else if 2 PVCOMP
	bool isActive; //if the game has ended, deletes it from the active games screen.
	static int totalMoves;
	class Cell{
	public:
		Cell() : Cell(0,0){/*Intentionally empty*/}
		Cell(int x,int y);
		enum cellState{empty = '.',p1='x',p2='o',p1won='X',p2won='O'};
		void setColumn(int x){column=x;}
		inline int getColumn() const {return column;}
		void setRow(int x){row=x;}
		inline int getRow() const {return row;}
		inline void setState(cellState x){state=x;}
		inline int getState() const{return state;}
	private:
		int column;
		int row;
		cellState state;
	};
	vector< vector<Cell> > hexCells;
	vector<vector<bool> > checkedBefore;
};

#endif // HEX_H