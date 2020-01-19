/*Egemen CAGSIRLI
CSE 344 System Programming HW 1*/
#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*----control functions----*/
/*The function that find the path is directory or not*/
int isDirectory(char *path){	
    struct stat s_buffer;
    if(stat(path, &s_buffer) == -1)
        return 0;
    else
        return S_ISDIR(s_buffer.st_mode);
}

/*The function that find the path is regular or not*/
int isReg(char *path){
    struct stat s_buffer;
    if(stat(path, &s_buffer) == -1)
        return 0;
    else
        return S_ISREG(s_buffer.st_mode);
}
/*The function that find the path is pipe or not*/
int isPipe(char *path){
	struct stat s_buffer;
	if(stat(path, &s_buffer) == -1)
		return 0;
	else
		return S_ISFIFO(s_buffer.st_mode);
}

/*The function that find the path is link or not*/
int isLink(char *path){
	struct stat s_buffer;
	if(lstat(path, &s_buffer) == -1)
		return 0;
	else
		return S_ISLNK(s_buffer.st_mode);
}
/*----control functions----*/

/*sizePathFun function. This function returns size. It is necessarily for this homework*/
int sizepathfun(char *path){
    struct stat s_buffer;
    if(stat(path, &s_buffer) == -1){
        perror("Wrong File Status!");
        return -1;
    }
    //if directory return 0
	else if(S_ISDIR(s_buffer.st_mode) > 0){
        return 0;
    }
    //if regular return size
    else if(S_ISREG(s_buffer.st_mode) > 0){
        return s_buffer.st_size;
    }
}


//I assigned false zMinus in global 
//if -z will be written zMinus will be true
_Bool zMinus=false;

/*----postOrderApply function----*/
//It is our base function, recursion and traversing in files are here*/
int postOrderApply(char *path, int pathfun(char* path1)) 
{
	int sum=0;
	int sumKB=0;
	struct dirent *newPath; 
	DIR *dPtr;
	dPtr = opendir(path);

	if(isDirectory(path))
	{
		/*-----------------------------------------------------*/
		if (dPtr == NULL) 
		{
		  perror("Failed to open directory");
		  printf("\n");
		  return -1;
		}
		/*-----------------------------------------------------*/
	
		//I give 4096 characters for max size of path
		char tempPath[4096];
		strcpy(tempPath,path);
		int count=0;
		while((newPath = readdir(dPtr))!=NULL)
		{	
			  	if(strcmp(newPath->d_name, ".")!=0 && strcmp(newPath->d_name, "..")!=0)
			  	{
					strcat(path,"/");
					strcat(path, newPath->d_name);
		  			if(isDirectory(path))
		  				//recursive part of this function
			  			sum+=postOrderApply(path,pathfun);
		  			else if(isReg(path) && (!isLink(path)) && (!isPipe(path))){
		  				sum+=pathfun(path);
		  			}
		  			else {
		  				printf("Special file %s \n",newPath->d_name );
		  			}

		  			strcpy(path,tempPath);
			  	}
		}
			//to calculate as KB 
			sumKB=sum/1024;
			printf("%d \t%s\n" , sumKB, path);
	}
	
	closedir(dPtr);
	//if there is -z
	if(!zMinus)
		return 0;
	return sum;
}

/*Usages in a function*/
void usages() {
	printf("\n\nUsage...:\n");
	printf("./buNeDu \"pathname\" or buNeDu -z \"pathname\" \n");
}



/*----main function----*/
int main(int argc, char *argv[]){

	if(argc==2){
			postOrderApply(argv[1], sizepathfun);
	}
	else if(argc==3){
		/*if(strcmp(argv[1], "buNeDu") != 0)
			printf("Usage: ./execfile buNeDu -z PATH or PATH\n");*/
		if(strcmp(argv[1], "-z")!=0){
			usages();
			return EXIT_FAILURE;
		}
		else{
			zMinus=true;
			//printf("\n%s (directory)\n", argv[3]);
			postOrderApply(argv[2], sizepathfun);
		}
	}
	else
		{
			usages();
			return EXIT_FAILURE;
		}
		    return EXIT_SUCCESS;

}

