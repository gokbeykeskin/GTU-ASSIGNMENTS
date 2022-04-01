#ifndef HEX_H
#define HEX_H
#include <string>
using namespace std;

class Hex{
public:
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
		friend ostream & operator << (ostream &out, const Cell &c);
	private:
		int column;
		int row;
		cellState state;
	};
	Hex() : Hex(6,1){/*Intentionally empty*/};
	explicit Hex(int x) : Hex(x,1){/*Intentionally empty */}
	Hex(int x,int y); //boardSize,gameMode;
	Hex(const Hex& other):Hex() {(*this)=other;}
	~Hex();
	Hex operator -- ();//pre decrement
	Hex operator -- (int);//post decrement
	const Hex& operator = (const Hex& other); //copy constructor
	friend ofstream& operator<<(ofstream &saveFile,const Hex& game); //saves the current game to a file.
	friend ostream& operator<<(ostream &printer,const Hex& game);    //prints the board.
	friend ifstream& operator>>(ifstream &loadFile,Hex& game);       //Loads the game from a file.
	friend const bool operator==(const Hex& game1,const Hex& game2);  //compares 2 games. returns true if they have the same amount of marked cells.
	inline void setBoardSize(int x){if(x>5) boardSize = x;}
	inline int getBoardSize() const{return boardSize;}
	void printBoardSize() const;
	//void printBoard() const; //dont need this anymore because I overloaded << for printing board.
	//bool compareWith(Hex game) const //dont need this anymore because I overloaded == for printing board.
	void playGame();
	Cell play(); //takes 1 move from the bot
	void play(int xMove,int yMove,int player); //if player is 1 puts 'x', if it is 2 puts 'o'
	bool winCheckP1(int i,int j);
	bool winCheckP2(int i,int j);
	inline void setGameMode(int x){if(x ==1 || x==2) gameMode = x;}
	inline int getGameMode() const {return gameMode;}
	static void moveCounter();
	static int  getTotalMoves();
	inline bool getisActive() const {return isActive;}
	void initializer();
	inline void incrementScore1() {++score1;} //player 1
	inline int getScore1() const{return score1;} //player 1
	inline void incrementScore2() {++score2;} //player 2
	inline int getScore2() const{return score2;} //player 2
private:
	int boardSize;
	int gameMode; //if 1 PVP, else if 2 PVE
	bool isActive; //if the game has ended, deletes it from the active games screen.
	static int totalMoves;
	Cell** hexCells;
	bool** checkedBefore;
	Cell** lastMoves;  //for undo feature.
	int lastMovesCap; //capacity
	int lastMovesUsed;
	int score1; //player1's score;
	int score2; //player2's score;
};

#endif // HEX_H