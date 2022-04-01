#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> //used for malloc-realloc
#include <string.h> //used for memset and strcpy


int getSize(char* text);
int isUpper(char x);
int isLower(char x);
void contains(int* sizes,int* ocurrences,char* str1, char* str2, int case_sensitive,char* str2_alt, int lineStartFlag,int lineEndFlag, char multipleChar,char multipleChar_alt,int multipleCharIndex);
void replaceAndWrite(int* sizes, int* ocurrences, char* buffer, char *replacementText,unsigned int* textSize,char* path);
int readFile(char** buffer,unsigned short buff_size,int fd);
void changeContent(char* command,char* buffer,unsigned int textSize,char* path);

int main(int argc, char* argv[]){
	unsigned int textSize;
	char* command = argv[1];
	char* path = argv[2];
	unsigned short buff_size = 128;
	
	char* buffer = (char*)malloc(sizeof(char)*buff_size);
	struct flock lock;
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	int fd = open(path, O_RDONLY, mode);
	if(buffer==NULL) perror("malloc");
	if(fd==-1){
		perror("open");
		return -1;
	}
	memset(&lock,0,sizeof(lock));


    do{
    	fcntl(fd, F_GETLK, &lock);
    	if(lock.l_type!=F_UNLCK) write(1,"File is locked by another instance. Waiting\n",45);
    }while(lock.l_type!=F_UNLCK);
	textSize = readFile(&buffer,buff_size,fd); //sent buffers adress instead of buffer itself because otherwise realloc in readfile func doesn't apply globally.
	if(buffer==NULL){
		perror("allocation or read");
		return -1;
	}
	if(close(fd)==-1){
		perror("close");
	}
	changeContent(command,buffer,textSize,path);


	free(buffer);
	return 0;
}

void changeContent(char* command,char* buffer,unsigned int textSize,char* path){
	int ocurrences[128];
	int sizes[128];
	char str1[64];
	char str1_alt[64];
	char str2[64];
	

	int str1Start = 0;
	int str2Start = 0;

	int i=0;
	int k,j;
	char doubleCharStart;
	char multipleChar,multipleChar_alt;
	int multipleCharIndex;
	int lineStartFlag,lineEndFlag, multipleFlag, doubleCharFlag, case_sensitive;
	do{
		multipleChar='\0';
		multipleChar_alt='\0';
		k=0; j=0;
		multipleFlag=0;
		doubleCharFlag = 0;
		case_sensitive = 1;
		lineStartFlag = 0;
		lineEndFlag = 0;
		multipleCharIndex=-1;
		for(str1Start=i;command[str1Start]!='/';str1Start++); //iterate until the first '/''
		str1Start++;
		str2Start=str1Start;
		if(command[str1Start]=='^'){
			lineStartFlag = 1;
			str1Start++;
		}
		for(int i=str1Start;command[i]!='/';i++){ //iterate until the second '/'' and get str1
			if(doubleCharFlag==0) doubleCharStart = command[i];
			if(doubleCharStart=='['){
				if(doubleCharFlag==0) i++;
				if(command[i]!='$'){
					str1_alt[k++] = command[i];
					if(doubleCharFlag==0) i++;
					str1[j++] = command[i];
				}
				
				if(command[i+1]==']') i++;

				if(doubleCharFlag==0 && command[i+1]=='*'){
					multipleChar=command[i-1];
					multipleChar_alt = command[i-2];
					if(lineStartFlag==0) 
						multipleCharIndex=i-4;
					else
						multipleCharIndex=i-5;
					i++;
				}
				doubleCharFlag=1;

			}
			else if(doubleCharStart=='*'){
				multipleChar = command[i-1];
				if(lineStartFlag==0) 
					multipleCharIndex=i-2;
				else
					multipleCharIndex=i-3;
			}
			else{
				if(command[i]!='$'){
					str1[j++] = command[i];
					str1_alt[k++] = command[i];
				}

			}
			if(command[i+1]=='/'){
				if(command[i]=='$'){
					lineEndFlag = 1;
				}
				str1[j++] = '\0'; //to specify end of the string
				str1_alt[k++] = '\0'; //to specify end of the string
			}
		}
		for(str2Start=str1Start;command[str2Start]!='/';str2Start++); //iterate until the last '/ and get str2
		str2Start++;
			for(i=str2Start;command[i]!='/';i++){
			str2[i-str2Start] = command[i];
			if(command[i+1]=='/'){
				str2[i-str2Start+1] = '\0';
				i++;
				if(command[i+1]=='i'){
					case_sensitive = 0;
					i++;
				}
				if(command[i+1]==';'){
					multipleFlag=1;
					i+=2;
				}

				break;
			}
		}
		contains(sizes, ocurrences,buffer,str1,case_sensitive,str1_alt,lineStartFlag,lineEndFlag,multipleChar,multipleChar_alt,multipleCharIndex); //fills the sizes and ocurrences array
		replaceAndWrite(sizes, ocurrences, buffer, str2, &textSize, path); //edits the buffer and writes it to file using occurence and size info 
	}while(multipleFlag==1);

}

int isUpper(char x){
	if(x>='A' && x<='Z') return 1;
	return 0;
}

int isLower(char x){
	if(x>='a' && x<='z') return 1;
	return 0;
}

void contains(int* sizes, int* ocurrences,char* str1, char* str2,int case_sensitive,char* str2_alt, int lineStartFlag,int lineEndFlag, char multipleChar, char multipleChar_alt,int multipleCharIndex){
	int i=0;
	int j=0;
	int k=0;
	int q=0;
	int multipleCharAmt,multipleChar_altAmt;
	int isSubstr,isSubstr_alt;
	int index;
	char str2_copy[64],str2_alt_copy[64];
	int isValid;
	int str2_size = getSize(str2);
	int str2_alt_size = getSize(str2_alt);
	int str1_size = getSize(str1);
	strcpy(str2_copy,str2);
	strcpy(str2_alt_copy,str2_alt);
	while(str1[i]!='\0'){ //iterate through buffer
		strcpy(str2,str2_copy);
		strcpy(str2_alt,str2_alt_copy);
		int firstTime=1,firstTime_alt=1;
		isValid = 0;
		isSubstr = str1[i]==str2[j] || (case_sensitive==0 && ((isUpper(str1[i]) && str1[i]==str2[j]-32)||(isLower(str1[i]) && str1[i]==str2[j]+32)));
		isSubstr_alt = str1[i]==str2_alt[j] || (case_sensitive==0 && ((isUpper(str1[i]) && str1[i]==str2_alt[j]-32)||(isLower(str1[i]) && str1[i]==str2_alt[j]+32)));
			
		index=-1;
		if(isSubstr || isSubstr_alt){ //if current buffer index matches first char of the str2 (or str2_alt)

			index = i;
			while(str2[j]!='\0'){ //iterate through str2
				multipleCharAmt=0;
				multipleChar_altAmt=0;
				
				if(multipleChar!= '\0' && str1[i]==multipleChar && j==multipleCharIndex){ //if the '*'' operator is used
					while(firstTime==1 && str1[i++]==multipleChar){
						multipleCharAmt++; //amount of the multiple char before *
					}

					if(multipleCharAmt>1){
						int ind;
						for(ind=multipleCharIndex;str2[ind]!=multipleChar;ind++);
						for(int l=str2_size+multipleCharAmt;l>=ind;l--){
							str2[l+multipleCharAmt-1] = str2[l];
							str2[l] = multipleChar;
						}//burda
						if(firstTime==1){
							j+=multipleCharAmt+1;
							sizes[q++]=getSize(str2);

							str2[j+1]='\0';
							j-=1;
							i-=1;
						}
					}
					else{
						sizes[q++] = getSize(str2);
						i-=2;
					}
					firstTime=0;
				}
				else if(multipleChar!= '\0' && j==multipleCharIndex && str1[i]!=multipleChar && str1[i]!=multipleChar_alt){ //if the multiple char is not on the buffer (example: st*r1 and sr1)
					for(int l=multipleCharIndex;l<str2_size;l++){
						str2[l] = str2[l+1];
					}
					isSubstr = str1[i+1]==str2[j+1] || (case_sensitive==0 && ((isUpper(str1[i+1]) && str1[i+1]==str2[j+1]-32)||(isLower(str1[i+1]) && str1[i+1]==str2[j+1]+32)));
					isSubstr_alt = str1[i+1]==str2_alt[j+1] || (case_sensitive==0 && ((isUpper(str1[i+1]) && str1[i+1]==str2_alt[j+1]-32)||(isLower(str1[i+1]) && str1[i+1]==str2_alt[j+1]+32)));
					if(isSubstr){
						sizes[q++] = getSize(str2);
					}
				}
				else if(multipleChar=='\0'&& multipleChar_alt=='\0'){ //if the '*' operator isn't used
					sizes[q++] = getSize(str2);

				}

				if(multipleChar_alt!='\0' && str1[i]==multipleChar_alt && j==multipleCharIndex){ //same as above but for str2_alt
					while(firstTime_alt==1 && str1[i++]==multipleChar_alt){
						multipleChar_altAmt++;

					}
					if(multipleChar_altAmt>1){
						int ind;
						for(ind=multipleCharIndex;str2_alt[ind]!=multipleChar_alt;ind++);
						for(int l=str2_alt_size+multipleChar_altAmt;l>=ind;l--){
							str2_alt[l+multipleChar_altAmt-1] = str2_alt[l];
							str2_alt[l] = multipleChar_alt;

						}
						if(firstTime_alt==1){
							j+=multipleChar_altAmt+1;
							sizes[q++]=getSize(str2_alt);
							str2_alt[j+1]='\0';
							j-=1;
							i-=1;
						}
					}
					else{
						sizes[q++] = getSize(str2_alt);
						i-=2;
					}
					
					firstTime_alt=0;
				}
				isSubstr = str1[i]==str2[j] || (case_sensitive==0 && ((isUpper(str1[i]) && str1[i]==str2[j]-32)||(isLower(str1[i]) && str1[i]==str2[j]+32)));
				isSubstr_alt = str1[i]==str2_alt[j] || (case_sensitive==0 && ((isUpper(str1[i]) && str1[i]==str2_alt[j]-32)||(isLower(str1[i]) && str1[i]==str2_alt[j]+32)));
				if(!isSubstr && !isSubstr_alt){ //if the whole word didn't match, set the index to -1
					index = -1;
					i--;
					break;
					
					
				}
				i++;
				j++;
			}
			j=0;
		}
		if(index != -1){ //if the word is matched add its starting index to ocurrences array
			i--;
			isValid = (lineStartFlag==0 && lineEndFlag==0) || (lineStartFlag==1 && (index==0||str1[index-1] == '\n')) || (lineEndFlag==1 && (index+str2_size==str1_size ||str1[index+str2_size]=='\n'));
			if(isValid){
				ocurrences[k++]=index;

			}
		}
		i++;
	}
	ocurrences[k]=-1; //to specify end of the array
	sizes[q] = -1; //to specify end of the array

}

/*
Text size is a pointer because it is returned. It is not used as an array.
Other pointers on this function are used as arrays.
*/
void replaceAndWrite(int* sizes,int* ocurrences, char* buffer,char *replacementText,unsigned int* textSize,char* path){
	if(ocurrences[0]==-1){ //for efficiency purposes: if there are no ocurrences of the str1 on the buffer, don't run this function.
		return;
	}
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	int fd = open(path, O_RDWR | O_TRUNC, mode);

	struct flock lock;
	memset(&lock,0,sizeof(lock));
	lock.l_type = F_WRLCK;

	while(fcntl(fd,F_SETLK,&lock)==-1) perror("Waiting:");
	write(1,"Write:Locked\n",14);
	//sleep(5); //uncomment to test file read lock with multiple instances
	int j;
	//int oldSize = getSize(oldText);
	int newSize = getSize(replacementText);

	if(fd==-1){//error handling
		perror("open");
	}

	for(int i=0;ocurrences[i]!=-1;i++){ //iterate through the ocurrences

		if(newSize>sizes[i]){ //if the newsize is bigger than the old one, shift right the buffer elements which are after the index by newSize-oldSize 
			
			for(int m=*textSize;m>=ocurrences[i]+sizes[i];m--){
				buffer[m+(newSize-sizes[i])-1] = buffer[m-1];

			}
			*textSize+=newSize-sizes[i]; //update the textsize with new size of the used portion of the buffer
			for(j=i+1;ocurrences[j]!=-1;j++){
				ocurrences[j]+=newSize-sizes[i]; //update the later occurrences accordingly 
			}
		}


		for(j=0;replacementText[j]!='\0';j++){ //replace the old text with new text
			buffer[ocurrences[i]+j] = replacementText[j];
		}

		if(sizes[i]>newSize){ //if the old size is bigger than the new one, shift left the buffer elements which are after the index by oldSize-newSize 
			for(int m=ocurrences[i]+newSize;m<=*textSize-(sizes[i]-newSize);m++){
				buffer[m] = buffer[m+(sizes[i]-newSize)];

			}
			*textSize-=sizes[i]-newSize; //update the textsize with new size of the used portion of the buffer
			for(j=i+1;ocurrences[j]!=-1;j++){
				ocurrences[j]-=sizes[i]-newSize; //update the later occurrences accordingly 
			}
		}

	}
	write(fd,buffer,*textSize);
	lock.l_type = F_UNLCK;
	if(fcntl(fd,F_SETLK,&lock)==-1) perror("unlock");
	else write(1,"Write:Unlocked\n",16);
	if(close(fd)==-1){
		perror("close");
	}
}

int getSize(char* text){
	int i=0;
	while(text[i]!='\0') i++;
	return i;
}

int readFile(char** buffer,unsigned short buff_size,int fd) {
    int bytes_read;
    int k = 0;
    do {
        char t = 0;
        bytes_read = read(fd, &t, 1);
        if(bytes_read!=-1){
        	(*buffer)[k++] = t;
			if(k==buff_size){ //if buffer is full, double its capacity.
	        	buff_size*=2;
	        	*buffer = (char*)realloc(*buffer,sizeof(char)*buff_size);
	        	if(*buffer==NULL) perror("realloc");
        	}    
        }
        
        
    }
    while (bytes_read!=0);
    return k-1; //size of the used portion of the buffer
}