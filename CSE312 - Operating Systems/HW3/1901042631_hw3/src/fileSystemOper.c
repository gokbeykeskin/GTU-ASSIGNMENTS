#include "makeFileSystem.h"
#include "fileSystemOper.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
int main(int argc, char* argv[]){

	uint8_t *filesystem = (uint8_t*)malloc(sizeof(uint8_t)*MB*12);
	char* file_sys_name;

	if(argc<3){
		fprintf(stderr, "%s\n","Not enough arguments.\nTerminating..." );
		exit(EXIT_FAILURE);
	}

	file_sys_name = argv[1];

	FILE *file= fopen(file_sys_name,"rb");
	if(file==NULL){
		fprintf(stderr, "%s\n","fopen failed.\n" );
		exit(EXIT_FAILURE);
	}

	if(fread(filesystem,MB*12,1,file)!=1){
		perror("fread");exit(EXIT_FAILURE);
	}

	fclose(file);

	if(strcmp(argv[2],"dir")==0){
		if(argc!=4){
			fprintf(stderr, "%s\n","Missing/Invalid Parameters." );
			exit(EXIT_FAILURE);
		}
		operationDir(filesystem,argv[3]);
	}
	else if(strcmp(argv[2],"mkdir")==0){
		if(argc!=4){
			fprintf(stderr, "%s\n","Missing/Invalid Parameters." );
			exit(EXIT_FAILURE);
		}
		operationMkdir(filesystem,argv[3]);
		printf("%s\n","File system updated." );

	}
	if(strcmp(argv[2],"rmdir")==0){
		if(argc!=4){
			fprintf(stderr, "%s\n","Missing/Invalid Parameters." );
			exit(EXIT_FAILURE);
		}
		operationRmdir(filesystem,argv[3]);
		printf("%s\n","File system updated." );

	}
	if(strcmp(argv[2],"dumpe2fs")==0){
		operationDumpe2fs(filesystem);
	}
	if(strcmp(argv[2],"write")==0){
		if(argc!=5){
			fprintf(stderr, "%s\n","Missing/Invalid Parameters." );
			
			exit(EXIT_FAILURE);
		}
		operationWrite(filesystem,argv[3],argv[4]);
		printf("%s\n","File system updated." );

	}
	if(strcmp(argv[2],"read")==0){
		if(argc!=5){
			fprintf(stderr, "%s\n","Missing/Invalid Parameters." );
			exit(EXIT_FAILURE);
		}
		operationRead(filesystem,argv[3],argv[4]);
	}
	if(strcmp(argv[2],"del")==0){
		if(argc!=4){
			fprintf(stderr, "%s\n","Missing/Invalid Parameters." );
			exit(EXIT_FAILURE);
		}
		operationDel(filesystem,argv[3]);
		printf("%s\n","File system updated." );

	}
    file = fopen(file_sys_name, "wb+");

 	if (fwrite(filesystem, MB*12, 1, file)!=1) {
        fprintf(stderr, "fwrite error!\n");
        exit(EXIT_FAILURE);
    }
    fclose(file);

}

void findUpperPath(char *dest, char *src)
{
    strcpy(dest, src);
    if (dest[strlen(dest)-1]=='/') {
        fprintf(stderr, "Invalid path!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = strlen(dest) - 1; 0 < i; --i) {
        if (dest[i]!='/'){
            dest[i] = '\0';
        } else {
            dest[i] = '\0';
            break;
        }
    }
}

void operationDir(uint8_t *fs,char* path){
	superblock *s = (superblock*)fs;
	file* f;
	inode* i;
	char temp[10];
	if(strcmp(path,"/")==0){
		f = (file*)(fs + s->first_block_addr);
		i = (inode*)(fs + s->first_inode_addr + f->inode_num*sizeof(inode));
	}
	else{
		fprintf(stderr,"NOT IMPLEMENTED\n");
		exit(EXIT_FAILURE);
	}

	for(int j=0;j<i->size/sizeof(file);j++){
		printf("%s\n",(f++)->name);
	}
}


void operationMkdir(uint8_t *fs,char* path){
	superblock *s = (superblock*)fs;
	file* f;
	inode* i;
	char upperpath[64];
	findUpperPath(upperpath,path);
	if(strcmp(upperpath,"/")==0){
		f = (file*)(fs + s->first_block_addr);
		i = (inode*)(fs + s->first_inode_addr + f->inode_num*sizeof(inode));
	}
	file* new_dir = (file*)(fs + s->first_block_addr + i->size);

	for(int j=0;j<i->size/sizeof(file);j++){
		if(strcmp((f++)->name,path)==0){
			fprintf(stderr,"Directory already exists!\n");
			exit(EXIT_FAILURE);
		}
	}


	i->size += sizeof(file);
	new_dir->inode_num = s->inodes_amt-(s->freeNodeCount--);
	inode* new_dir_inode = (inode*)(fs + s->first_inode_addr + new_dir->inode_num*sizeof(inode));
	new_dir_inode->type = DIRECTORY_T;
	new_dir_inode->occupied=1;
	strcpy(new_dir->name,path);
	s->dirCount++;

}

void operationRmdir(uint8_t *fs,char* path){
	superblock *s = (superblock*)fs;
	file* f;
	inode* i;
	char upperpath[64];
	findUpperPath(upperpath,path);
	if(strcmp(upperpath,"/")==0){
		f = (file*)(fs + s->first_block_addr);
		i = (inode*)(fs + s->first_inode_addr + f->inode_num*sizeof(inode));


	}
	else{
		fprintf(stderr,"NOT IMPLEMENTED");
		exit(EXIT_FAILURE);
	}

	file* new_dir = (file*)(fs + s->first_block_addr);
	inode* new_dir_inode;


	for(int j=0;j<i->size;j++){
		if(strcmp((new_dir)->name,path)==0){
			new_dir_inode = (inode*)(fs + s->first_inode_addr + new_dir->inode_num*sizeof(inode));
			if(new_dir_inode->type!=DIRECTORY_T){
				fprintf(stderr,"NOT A DIRECTORY\n");
				exit(EXIT_FAILURE);
			}	
			for(int k=j;k<i->size;k++){
				*new_dir = *(new_dir+1);
				new_dir++;
			}
			i->size -= sizeof(file);
			s->dirCount--;
			s->freeNodeCount++;
			return;
		}
		new_dir++;
	}
	fprintf(stderr,"No such directory.\n");


}
void operationDumpe2fs(uint8_t *fs){
	superblock* s = (superblock*)fs;
	file* f = (file*)(fs + s->first_block_addr);
	inode* i = (inode*)(fs + s->first_inode_addr + f->inode_num*sizeof(inode));
	printf("Block Size:%d\n",s->block_size);
	printf("Block Amount:%d\n",s->blocks_amt);
	printf("Address of the first block:%d\n",s->first_block_addr);
	printf("I-node Amount:%d\n",s->inodes_amt);
	printf("Free I-Node amount:%d\n",s->freeNodeCount);
	printf("Address of the first I-node:%d\n",s->first_inode_addr);

	printf("Directory Count:%d\n",s->dirCount);
	printf("File Count:%d\n",s->fileCount);
}
void operationWrite(uint8_t *fs,char* path,char* copyPath){
	superblock *s = (superblock*)fs;
	file* f;
	inode* i;
	char upperpath[64];
	findUpperPath(upperpath,path);
	if(strcmp(upperpath,"/")==0){
		f = (file*)(fs + s->first_block_addr);
		i = (inode*)(fs + s->first_inode_addr + f->inode_num*sizeof(inode));
	}
	file* new_dir = (file*)(fs + s->first_block_addr + i->size);
	
	for(int j=0;j<i->size/sizeof(file);j++){
		if(strcmp((f++)->name,path)==0){
			fprintf(stderr,"File already exists!\n");
			exit(EXIT_FAILURE);
		}
	}
	i->size += sizeof(file);
	new_dir->inode_num = s->inodes_amt-(s->freeNodeCount--);
	inode* new_dir_inode = (inode*)(fs + s->first_inode_addr + new_dir->inode_num*sizeof(inode));
	new_dir_inode->type = FILE_T;
	new_dir_inode->occupied=1;
	strcpy(new_dir->name,path);
	s->fileCount++;
}
void operationRead(uint8_t *fs,char* path,char* pastePath){

}
void operationDel(uint8_t *fs,char* path){
	superblock *s = (superblock*)fs;
	file* f;
	inode* i;
	char upperpath[64];
	findUpperPath(upperpath,path);
	if(strcmp(upperpath,"/")==0){
		f = (file*)(fs + s->first_block_addr);
		i = (inode*)(fs + s->first_inode_addr + f->inode_num*sizeof(inode));


	}
	else{
		fprintf(stderr,"NOT IMPLEMENTED");
		exit(EXIT_FAILURE);
	}
	s->fileCount--;
	s->freeNodeCount++;
	file* new_dir = (file*)(fs + s->first_block_addr);
	inode* new_dir_inode;


	for(int j=0;j<i->size;j++){
		if(strcmp((new_dir)->name,path)==0){
			new_dir_inode = (inode*)(fs + s->first_inode_addr + new_dir->inode_num*sizeof(inode));
			fprintf(stderr,"type:%d\n",new_dir_inode->type);
			if(new_dir_inode->type!=FILE_T){
				fprintf(stderr,"NOT A FILE\n");
				exit(EXIT_FAILURE);
			}	
			for(int k=j;k<i->size;k++){
				*new_dir = *(new_dir+1);
				new_dir++;
			}
			i->size -= sizeof(file);

			return;
		}
		new_dir++;
	}
	fprintf(stderr,"No such FILE.\n");

}