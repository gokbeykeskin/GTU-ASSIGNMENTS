#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "makeFileSystem.h"
int main(int argc, char* argv[]){

	if(argc!=3){
		fprintf(stderr, "%s\n","Invalid/Missing arguments->..\nTerminating..." );
		exit(EXIT_FAILURE);
	}

	int remaining_space = 12*MB - sizeof(superblock);
	char* sys_file_name = argv[2];

	uint8_t *file_sys = (uint8_t*)malloc(sizeof(uint8_t)*12*MB);
	memset(file_sys,0,12*MB);

	superblock *s = (superblock*) file_sys;
	s->block_size = atoi(argv[1]); 

	if(s->block_size*KB > (99.0/100.0)*(double)remaining_space){
		fprintf(stderr, "This block size is not suitable for 12MB file system.\n");
		exit(EXIT_FAILURE);
	}

	s->blocks_amt = ((99.0/100.0)*remaining_space)/ (s->block_size*KB);
	

	remaining_space = (1.0/100.0)*remaining_space;

	s->inodes_amt = remaining_space/(sizeof(inode));

	s->first_block_addr = 12*MB-(s->blocks_amt*s->block_size*KB);
	s->first_inode_addr = s->first_block_addr - (s->inodes_amt*sizeof(inode));

	for(int i=0;i<4096;i++) s->blocks_bitmap[i]=0;
	s->blocks_bitmap[0] = 1;
	s->blocks_bitmap[1] = 1;
	inode *root_inode = (inode*)(file_sys+s->first_inode_addr);

	root_inode->type = DIRECTORY_T;
	root_inode->size = 2*sizeof(file); // . and ..
	root_inode->last_access_time = (int32_t)time(NULL);
	root_inode->creation_time = (int32_t)time(NULL);
	root_inode->occupied = 1;
	root_inode->direct_blocks[0] = 0; //block #0

	file *root_file = (file*)(file_sys + s->first_block_addr);
	root_file->inode_num = 0;
	strcpy(root_file->name,".");
	(++root_file)->inode_num = 0;
	strcpy(root_file->name,"..");

	s->dirCount=1;
	s->fileCount=0;
	s->freeNodeCount=s->inodes_amt;
	FILE *out_file = fopen(sys_file_name,"wb+");

	if(out_file==NULL){
		perror("fopen");exit(EXIT_FAILURE);
	}

	if(fwrite(file_sys,12*MB,1,out_file) != 1){
		perror("write");exit(EXIT_FAILURE);
	}
	free(file_sys);
	fclose(out_file);

	return 1;



}

