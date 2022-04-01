#ifndef winCheckLIB_H
#define winCheckLIB_H

enum cellState {unOccupied = '.',p1Occupied='x',p2Occupied='o',p1Won='X',p2Won='O'};

bool winCheckP1LR(char hexBoard[12][12],int boardSize,bool checkedBefore[12][12],int i=0,int j=0); //true when Player 1 won
bool winCheckP1RL(char hexBoard[12][12],int boardSize,bool checkedBefore[12][12],int i=0,int j=0); //true when Player 1 won
bool winCheckP2LR(char hexBoard[12][12],int boardSize,bool checkedBefore[12][12],int i=0,int j=0); //true when Player 2 (or comp) won
bool winCheckP2RL(char hexBoard[12][12],int boardSize,bool checkedBefore[12][12],int i=0,int j=0); //true when Player 2 (or comp) won
/*the reason I made 2 check conditions for each player is:
 	 when there is a 'x'(or 'o') on left, it wasn't controlling the other possible routes. 
 */
#endif // winCheckLIB_H
