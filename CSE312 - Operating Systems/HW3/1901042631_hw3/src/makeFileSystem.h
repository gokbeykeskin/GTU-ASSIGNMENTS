#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <stdint.h>


#define KB 1024
#define MB KB*1024


typedef unsigned int f_type;
#define FILE_T 0
#define DIRECTORY_T 1



typedef struct{
	uint16_t block_size;
	uint16_t blocks_amt;
	uint32_t first_block_addr;
	uint8_t blocks_bitmap[4096];
	uint16_t inodes_amt;
	uint32_t first_inode_addr;
	uint16_t dirCount;
	uint16_t fileCount;
	uint16_t freeNodeCount;
}__attribute__ ((__packed__)) superblock;


typedef struct{
	uint32_t size;
	uint32_t creation_time;
	uint32_t last_access_time;
	uint16_t direct_blocks[13];
	uint16_t indirect1_block;
	uint16_t indirect2_block;
	uint16_t indirect3_block;
	uint8_t occupied;
	f_type type;



}__attribute__ ((__packed__)) inode;


typedef struct{
	uint16_t inode_num;
	char name[24];
}__attribute__ ((__packed__)) file;


#endif