#ifndef processR_H
#define processR_H

int strlength(char* str);
void tokenizer(double coordinates[10][3]);
void covMatrix(double covMatrix[3][3], double coord[10][3]);
void writeToFile(double covMatrix[3][3],char* outputPath);
void sigintHandler(int signal_number);

#endif