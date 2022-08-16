#ifndef FILE_SYS_OP_H
#define FILE_SYS_OP_H

#include "makeFileSystem.h"

void operationDir(uint8_t *fs,char* path);
void operationMkdir(uint8_t *fs,char* path);
void operationRmdir(uint8_t *fs,char* path);
void operationDumpe2fs(uint8_t *fs);
void operationWrite(uint8_t *fs,char* path,char* copyPath);
void operationRead(uint8_t *fs,char* path,char* pastePath);
void operationDel(uint8_t *fs,char* path);
#endif