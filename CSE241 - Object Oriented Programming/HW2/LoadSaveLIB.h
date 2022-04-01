#ifndef LoadSaveLIB_H
#define LoadSaveLIB_H
#include <string>
void saveGame(char Board[12][12],const std::string& fileName,const int boardSize,const int gameMode);
void loadGame(char Board[12][12],const std::string& fileName, int& boardSize, int& gameMode);


#endif // LoadSaveLIB_H
