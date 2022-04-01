#ifndef AbstractHex_H
#define AbstractHex_H

#include "Cell.h"
using namespace std;

namespace gokbeysHex{

	class AbstractHex{
	public:
		AbstractHex() : AbstractHex(6,1){/*Intentionally empty*/};
		explicit AbstractHex(int x) : AbstractHex(x,1){/*Intentionally empty */}
		AbstractHex(int x,int y); //boardSize,gameMode;
		virtual Cell operator()(int x,int y)const=0;
		virtual Cell& operator()(int x,int y)=0;
		bool operator==(const AbstractHex& other);
		Cell lastMove();
		Cell play(); //bot's play function
		void play(int yMove,int xMove,int player);//if player is 1 puts 'x', if it is 2 puts 'o'
		void playGame(); //called by main to start the game.
		inline void setBoardSize(int x){if(x>5) boardSize = x;}
		inline const int getBoardSize() const{return boardSize;}
		virtual void setSize(int x)=0;
		inline void setGameMode(const int& x){if(x ==1 || x==2) gameMode = x;}
		inline const int getGameMode() const {return gameMode;}
		void printBoardSize() const;
		static void moveCounter(); //increases total move
		static int getTotalMoves(); //returns total move
		inline const bool isEnd() const {return isActive;}
		inline void incrementScore1() {++score1;} //player 1
		inline const int getScore1() const{return score1;} //player 1
		inline void incrementScore2() {++score2;} //player 2
		inline const int getScore2() const{return score2;} //player 2
		friend ostream& operator<<(ostream &printer,const AbstractHex& game);
		void print(); //prints the board
		ifstream& readFromFile(ifstream &loadFile); //LOAD GAME
		ofstream& writeToFile(ofstream &saveFile); //SAVE GAME
		void reset(); //resets the game process
		bool winCheckP1(int i,int j); //returns true when player1 wins
		bool winCheckP2(int i,int j); //returns true when player2 wins


	protected:
		int boardSize;
		int gameMode; //if 1 PVP, else if 2 PVE
		bool isActive; //if the game has ended, deletes it from the active games screen.
		int score1; //player1's score;
		int score2; //player2's score;
		static int totalMoves;
		Cell last;

	};
}
#endif //AbstractHex_H