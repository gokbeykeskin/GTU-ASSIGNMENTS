#ifndef processP_H
#define processP_H


void spawnChild(char* program, char** arguments, char** matrix);
void sigintHandler(int signal_number);
void sigchldHandler(int signal_number);
void calcFrobeniusNorm(char* outputPath);
double square(double num);
void findClosestPair(double* frob_norms, int frobNormAmt, double (*matrices)[3][3]);
int length(char* str);
int getAscii(char x);

#endif