#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#ifndef what
#define what
#include "sorter.h"
#endif

//put these in headerfiles
int checkCSV(char*);
int checkDir(char*); 
void *threadfunc(void *vargp); //for testing purposes, delete
void *threadDir(void *vargp);
void *threadCSV(void *vargp);
void megaSort();
//void megamerge(struct movie*, struct movie*, int, int);
//void megamerge(struct movieNode, struct movieNode);
struct movie* megamerge(struct movie*, struct movie*);

pthread_mutex_t totalFilesLock;
pthread_mutex_t totalThreadsLock;
int globTotalSortedFiles = 0;
int globTotalThreads = 0;
int currFileArraySize = 6000;
int currentMovieArraySize = 20000; //so we can realloc the movieArray if the TMAC exceeds CMAS 	
char** fileArray;
char* sortedValue;
int sortIndex;
char* outputDirectory;

void tokenize(char* line, struct movie* movie){
	int front = 0;
	int back = 0;

	int charPointer = 0;
	int valueIndex = 0;
	movie->values[27] = NULL;

	while(line[charPointer] != '\0'){
		if(line[charPointer] == ' '){
			charPointer++;
			while(line[charPointer] == ' '){
				charPointer++;
			}
		}
		front = charPointer;
		if(line[charPointer] == '"'){
			charPointer++;
			while(line[charPointer] != '"'){
				charPointer++;
			}
		}
		if(line[charPointer] == '"'){

			back = charPointer;
			movie->values[valueIndex] = (char*) realloc(movie->values[valueIndex], sizeof(char)*(back-front+2));
		//	movie->values[valueIndex] = (char*) malloc (sizeof(char)*(back-front+2));//char arr[(back - front + 2)];
			movie->values[valueIndex][back-front+1] = '\0';
			strncpy(movie->values[valueIndex], &line[front], back-front+1);
			charPointer++;
			charPointer++;
			valueIndex++;
			continue;
		}


		if(line[front] == ','){
			movie->values[valueIndex] = "";
			valueIndex++;
			front = 0;
			charPointer++;
			continue;
		}

		while(line[charPointer] != ',' && line[charPointer] != '\0'){
			charPointer++;
		}

		back = charPointer-1;

		while(line[back] == ' ' || line[back] == '\r' || line[back] == '\n'){
			back--;
		}
		movie->values[valueIndex] = (char*) realloc(movie->values[valueIndex], sizeof(char)*(back-front+2));
	//	movie -> values[valueIndex] = (char*) malloc (sizeof(char)*(back-front+2));
		movie->values[valueIndex][back-front+1] = '\0';
		strncpy(movie->values[valueIndex], &line[front], back-front+1);
	//	printf("%s\n", movie->values[valueIndex]);
		if(line[charPointer] != '\0'){
			charPointer++;
		}
		valueIndex++;

	}
	
	if(movie->values[27] == NULL){
		movie->values[27] = "";
	}

}

int sort(char* inputCSV, char* sortValue){

    struct movie* movieArray=(struct movie*) malloc(12000*sizeof(struct movie));
    int i = 0;
    int j = 0;
    
    for(i = 0; i < 12000;i++){
        for(j = 0; j<28;j++){
            movieArray[i].values[j] =(char*) malloc(sizeof(char));
        }
    }
    
    int counter = 0;
    char* equal = NULL;
    size_t a = 1;
    
    char* firstLine = NULL;
    FILE* read = fopen(inputCSV, "r");
    getline(&firstLine, &a, read);
     
    if(strcmp(firstLine, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\r\n") == 0){
        //printf("good\n");
    } else {
 	//printf("invalid CSV");
	pthread_exit(NULL);
	return 1; //switch to exit(1) 
 //       exit(1);
    }
    
    while(getline(&equal, &a, read) != EOF){
        //while(fgets(equal, 12000, read) != EOF){
        tokenize(equal, &(movieArray[counter]));
        counter++;
    }
    
    //Selector to see
    //what the value is
    if (strcmp(sortValue,"color") == 0) {
        mergesort(movieArray, 0, counter);
        sortIndex = 0;
    } else if (strcmp(sortValue,"director_name") == 0){
        mergesort(movieArray,1, counter);
        sortIndex = 1;
    } else if (strcmp(sortValue,"num_critic_for_reviews") == 0){
        mergesort(movieArray,2, counter);
        sortIndex = 2;
    } else if (strcmp(sortValue,"duration") == 0){
        mergesort(movieArray,3, counter);
        sortIndex = 3;
    } else if (strcmp(sortValue,"director_facebook_likes") == 0) {
        mergesort(movieArray, 4, counter);
        sortIndex = 4;
    } else if (strcmp(sortValue,"actor_3_facebook_likes") == 0) {
        mergesort(movieArray, 5, counter);
        sortIndex = 5;
    } else if (strcmp(sortValue,"actor_2_name") == 0) {
        mergesort(movieArray, 6, counter);
        sortIndex = 6;
    } else if (strcmp(sortValue,"actor_1_facebook_likes") == 0) {
        mergesort(movieArray, 7, counter);
        sortIndex = 7;
    } else if (strcmp(sortValue,"gross") == 0) {
        mergesort(movieArray, 8, counter);
        sortIndex = 8;
    } else if (strcmp(sortValue,"genres") == 0) {
        mergesort(movieArray, 9, counter);
        sortIndex = 9;
    } else if (strcmp(sortValue,"actor_1_name") == 0) {
        mergesort(movieArray, 10, counter);
        sortIndex = 10;
    } else if (strcmp(sortValue,"movie_title") == 0) {
        mergesort(movieArray, 11, counter);
        sortIndex = 11;
    } else if (strcmp(sortValue,"num_voted_users") == 0) {
        mergesort(movieArray, 12, counter);
        sortIndex = 12;
    } else if (strcmp(sortValue,"cast_total_facebook_likes") == 0) {
        mergesort(movieArray, 13, counter);
        sortIndex = 13;
    } else if (strcmp(sortValue,"actor_3_name") == 0) {
        mergesort(movieArray, 14, counter);
        sortIndex = 14;
    } else if (strcmp(sortValue,"facenumber_in_poster") == 0) {
        mergesort(movieArray, 15, counter);
        sortIndex = 15;
    } else if (strcmp(sortValue,"plot_keywords") == 0) {
        mergesort(movieArray, 16, counter);
        sortIndex = 16;
    } else if (strcmp(sortValue,"movie_imdb_link") == 0) {
        mergesort(movieArray, 17, counter);
        sortIndex = 17;
    } else if (strcmp(sortValue,"num_user_for_reviews") == 0) {
        mergesort(movieArray, 18, counter);
        sortIndex = 18;
    } else if (strcmp(sortValue,"language") == 0) {
        mergesort(movieArray, 19, counter);
        sortIndex = 19;
    } else if (strcmp(sortValue,"country") == 0) {
        mergesort(movieArray, 20, counter);
        sortIndex = 29;
    } else if (strcmp(sortValue,"content_rating") == 0) {
        mergesort(movieArray, 21, counter);
        sortIndex = 21;
    } else if (strcmp(sortValue,"budget") == 0) {
        mergesort(movieArray, 22, counter);
        sortIndex = 22;
    } else if (strcmp(sortValue,"title_year") == 0) {
        mergesort(movieArray, 23, counter);
        sortIndex = 23;
    } else if (strcmp(sortValue,"actor_2_facebook_likes") == 0) {
        mergesort(movieArray, 24, counter);
        sortIndex = 24;
    } else if (strcmp(sortValue,"imdb_score") == 0) {
        mergesort(movieArray, 25, counter);
        sortIndex = 25;
    } else if (strcmp(sortValue,"aspect_ratio") == 0) {
        mergesort(movieArray, 26, counter);
        sortIndex = 26;
    } else if (strcmp(sortValue,"movie_facebook_likes") == 0) {
        mergesort(movieArray, 27, counter);
        sortIndex = 27;
    } else {
        //printf("Please enter a valid type");
        //exit(1);
        pthread_exit(NULL);
	return -1;
    }
   
    fclose(read);
    
    //inputCSV[(strlen(inputCSV) -6)] = 'x';

    FILE* write = fopen(inputCSV, "w");
    fprintf(write, "%s", firstLine);
    
    for(i = 0; i < counter; i++){
	for(j = 0; j < 26; j++){
		fprintf(write, "%s,", movieArray[i].values[j]);
	}
	fprintf(write, "%s\n", movieArray[i].values[27]);
    }

    fclose(write);

    if (globTotalSortedFiles > currFileArraySize){
    	currFileArraySize += 3000;
	fileArray = realloc(fileArray, (currFileArraySize*sizeof(char*)));
    }
    pthread_mutex_lock(&totalFilesLock);
    fileArray[globTotalSortedFiles] = inputCSV;
    globTotalSortedFiles++;
    pthread_mutex_unlock(&totalFilesLock);
   
    return 0;
}

int checkCSV(char* filename){
	if (strlen(filename) < 5){
		return 0;
	} 
	int length = strlen(filename);
	if (filename[length - 1] == 'v' && filename[length - 2] == 's' && filename[length - 3] == 'c' && filename[length - 4] == '.' ){
		return 1;
	} else {
		return 0;
	}
} 

int checkDir(char* filename){
	if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0 || strcmp(filename, ".git") == 0){
		return 0;
	} else {
		return 1;
	}
	
}

void *threadDir(void *vargp){
	char* currDirectory =  vargp;
	//printf("%s ", currDirectory);
	printf("%d,", (long) pthread_self());
	
	char* toOpen = malloc(strlen(currDirectory) + 1);
	strcpy(toOpen, currDirectory);

	int currSize = 100;	
	int numThreads = 0;
	int numDirectories = 0;
	pthread_t* threadSpawns = (pthread_t*) malloc (currSize * sizeof(pthread_t));

	DIR *dir;
	dir = opendir(toOpen);
	

	struct dirent *dp;
	while ((dp = readdir(dir)) != NULL){
		
		if (numThreads > currSize) {
			currSize *= 2;
			threadSpawns = realloc(threadSpawns, (currSize*sizeof(pthread_t)));
		}

		struct stat stats;
		char* filePath = malloc(strlen(currDirectory) + strlen(dp -> d_name) + 2);
		strcpy(filePath, currDirectory);
		strcat(filePath, "/");
		strcat(filePath, dp -> d_name);
		stat(filePath, &stats);
		if (S_ISREG(stats.st_mode)) {
			if(checkCSV(dp -> d_name)){
				char* filePath1 = malloc(strlen(currDirectory) + strlen(dp -> d_name) + 2);
				strcpy(filePath1, currDirectory);
				strcat(filePath1, "/");
				strcat(filePath1, dp -> d_name);
				pthread_create(&(threadSpawns[numThreads]), NULL, threadCSV, filePath1);
				numThreads++;
			} else {
				continue;
			}
		} else if (S_ISDIR(stats.st_mode)){
			if(checkDir(dp -> d_name)){
				char* filePath2 = malloc(strlen(currDirectory) + strlen(dp -> d_name) + 2);
				strcpy(filePath2, currDirectory);
				strcat(filePath2, "/");
				strcat(filePath2, dp -> d_name);
				pthread_create(&(threadSpawns[numThreads]), NULL, threadDir, filePath2);
				numThreads++;
			}
		}
	}
	
	pthread_mutex_lock(&totalThreadsLock);
	globTotalThreads += numThreads;			
	pthread_mutex_unlock(&totalThreadsLock);
		
	int i = 0;
	for(i = 0; i < numThreads; i++) {
		pthread_join(threadSpawns[i], NULL);
	}
	free(threadSpawns);
	return NULL;
}

void *threadCSV(void *vargp){
	char* CSVName = vargp;
	//printf("%s ", CSVName);
	printf("%d,", (long) pthread_self());
	
	sort(CSVName, sortedValue);
	
	return NULL;
}

void megaSort(){
	int fileArrayCounter = 0;
	struct movie* movieListFront = (struct movie*) malloc(sizeof(struct movie));
	struct movie* moviePtr = movieListFront;

    	int j = 0;
    	char* equal = NULL;
	size_t a = 1;
    
	char* firstLine = NULL;
	FILE* read = fopen(fileArray[fileArrayCounter], "r");
	getline(&firstLine, &a, read);
  	//skip the firstline
	
	getline(&equal, &a, read);

	for(j = 0; j<28;j++){
		(moviePtr ->values[j]) =(char*) malloc(sizeof(char));
    	}
	
	tokenize(equal, moviePtr);
	moviePtr -> next = (struct movie*) malloc(sizeof(struct movie));
	
	moviePtr = moviePtr -> next;

    while(getline(&equal, &a, read) != EOF){
       	//	tokenize(equal, &(movieArray[totalMovieArrayCounter]));
        	//totalMovieArrayCounter++;
		
	
		for(j = 0; j<28;j++){
        		moviePtr -> values[j] =(char*) malloc(sizeof(char));
    		}

		tokenize(equal, moviePtr);
		moviePtr -> next = (struct movie*) malloc(sizeof(struct movie));
		moviePtr = moviePtr -> next;
	
	}
	moviePtr = NULL;
/*
	//debug code
	printf("first file: %s \n", fileArray[fileArrayCounter]);
	struct movie* point = movieListFront;
	while (point -> next != NULL){
		for(j = 0; j < 26; j++){
			printf("%s,", point -> values[j]);
		}
		printf("%s\n", point -> values[27]);
		point = point -> next;
	}
*/
	//end debug code, delete above


	fileArrayCounter++;
	fclose(read);
	//now the large Movie Array is populated with the contents of the first file. 
	//now go through each consecutive file, and just merge the contents of that array with the existing movieArray
	
	while(fileArrayCounter < globTotalSortedFiles){
		FILE* toRead = fopen(fileArray[fileArrayCounter], "r");
		
		struct movie* toSortListFront = (struct movie*) malloc(sizeof(struct movie));
		struct movie* toSortPtr = toSortListFront;

		char* str = NULL;
		char* firstL = NULL;
		getline(&firstL, &a, toRead);

		getline(&str, &a, toRead);
  		
		//initialize values of array
		for(j = 0; j<28;j++){
        		toSortPtr -> values[j] =(char*) malloc(sizeof(char));
    		}
		
		tokenize(str, toSortPtr);
		
		toSortPtr -> next = (struct movie*) malloc(sizeof(struct movie));
		toSortPtr = toSortPtr -> next;
	
		while(getline(&str, &a, toRead) != EOF){
			
			for(j = 0; j<28;j++){
        			toSortPtr -> values[j] =(char*) malloc(sizeof(char));
    			}
				
			tokenize(str, toSortPtr);
			toSortPtr -> next = (struct movie*) malloc(sizeof(struct movie));
			toSortPtr = toSortPtr -> next;

		}	
		toSortPtr = NULL;
	/*	
		//debug code
		printf("file: %s \n", fileArray[fileArrayCounter]);
		struct movie* pointer = toSortListFront;
		while (pointer -> next != NULL){
			for(j = 0; j < 26; j++){
				printf("%s,", pointer -> values[j]);
			}
			printf("%s\n", pointer -> values[27]);
			pointer = pointer -> next;
		}	

	//end debug code, delete above
	*/
		//sortIndex = 0
		
		movieListFront = megamerge(movieListFront, toSortListFront);
		
		/*
		//debug code
		printf("PRINTING MOVIELIST AFTER MERGE \n");
		struct movie* point = movieListFront;
		while (point -> next != NULL){
			for(j = 0; j < 26; j++){
				printf("%s, ", point -> values[j]);
			}
			printf("%s  ---------- \n ", point -> values[27]);
			point = point -> next;
		}
		printf("\n\n\n");
		*/
		fclose(toRead);
		fileArrayCounter++;
	}

	//now print MovieArray to the output file
	
	
	char* out = "AllFiles-sorted-<";
	//printf("skhskhs");
	//fflush(stdout);
	char* outputfilepath = (char*) malloc(10000);//malloc(strlen(outputDirectory) + strlen(out) + strlen(sortedValue) + 7);
	
	//printf("%s\n", outputDirectory);
	//fflush(stdout);
	strcpy(outputfilepath, outputDirectory);	
	strcat(outputfilepath, "/");
	strcat(outputfilepath, out);
	strcat(outputfilepath, sortedValue);
	strcat(outputfilepath, ">.csv\0");
	//strcat(outputfilepath, "\0");
	
	//printf("outputfilepath: %s\n", outputfilepath);
	FILE* output = fopen(outputfilepath, "w");

	char* fL = "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\r\n";

	fprintf(output, "%s", fL);
    	int i = 0;
	
	moviePtr = movieListFront;
	while (moviePtr -> next != NULL){
		for(j = 0; j < 26; j++){
       			fprintf(output, "%s,", moviePtr -> values[j]);
        	}
        	fprintf(output, "%s\n", moviePtr -> values[27]);  
		moviePtr = moviePtr -> next;
	}

	fclose(output);
	
	//OutputDirectory + AllFiles-sorted-<sortedvalue>.csv

}


struct movie* megamerge(struct movie* movieListFront, struct movie* toSortListFront){
	//printf("how many errors? \n");

	struct movie* newPtr;
	struct movie* head;

	if(isFloat[sortIndex]){
		// leftmoviearray[lefpoint].valeus[sortIndex]
		// same for right
		//float leftNum = atof(leftArray[leftPointer]);
		//float rightNum =  atof(rightArray[rightPointer]);

		float movieNum;

		if(strlen(movieListFront->values[sortIndex]) == 0){
			movieNum = -1;
		}
		else{
			movieNum = atof(movieListFront->values[sortIndex]);
		}
		

		float toSortNum;
		if(strlen(toSortListFront->values[sortIndex]) == 0){
			toSortNum = -1;
		}
		else {
			toSortNum = atof(toSortListFront->values[sortIndex]);
		}
		
		if(movieNum <= toSortNum){
			// msovieArray [i] = left[leftpointer]
			//movieArray[i].values[sortIndex] = leftArray[leftPointer];
			newPtr = movieListFront;
			head = movieListFront;
			movieListFront = movieListFront->next;
			
		}else{
			newPtr = toSortListFront;
			head = toSortListFront;
			toSortListFront = toSortListFront->next;
		}
	}
	else{
		char* movieString;
		char* toSortString;
	//	if(leftArray[leftPointer][0] == '"'){
		if(strlen(movieListFront->values[sortIndex]) != 0 && movieListFront->values[sortIndex][0] == '"'){
			movieString = &movieListFront->values[sortIndex][1];
		}else{
			movieString = movieListFront->values[sortIndex];
		}

		if(strlen(toSortListFront->values[sortIndex]) != 0 && toSortListFront->values[sortIndex][0] == '"'){
			toSortString = &toSortListFront->values[sortIndex][1];
		}else{
			toSortString = toSortListFront->values[sortIndex];
		}

		if(strcasecmp(movieString, toSortString) <= 0){
			//movieArray[i].values[sortIndex] = leftArray[leftPointer];
			newPtr = movieListFront;
			head = movieListFront;
			movieListFront = movieListFront->next;
		}else{
			//movieArray[i].values[sortIndex] = rightArray[rightPointer];
			newPtr = toSortListFront;
			head = toSortListFront;
			toSortListFront = toSortListFront->next;
		}	
	}


	while(toSortListFront != NULL && movieListFront != NULL){

		if(toSortListFront->values[sortIndex] == NULL){
			toSortListFront = toSortListFront ->next;
			continue;
		}
		if(movieListFront->values[sortIndex] == NULL){
			movieListFront = movieListFront->next;
			continue;

		}

		if(isFloat[sortIndex]){
			// leftmoviearray[lefpoint].valeus[sortIndex]
			// same for right
			//float leftNum = atof(leftArray[leftPointer]);
			//float rightNum =  atof(rightArray[rightPointer]);
			float movieNum;

			if(strlen(movieListFront->values[sortIndex]) == 0){
				movieNum = -1;
			}
			else{
				movieNum = atof(movieListFront->values[sortIndex]);
			}
			

			float toSortNum;
			if(strlen(toSortListFront->values[sortIndex]) == 0){
				toSortNum = -1;
			}
			else {
				toSortNum = atof(toSortListFront->values[sortIndex]);
			}

			if(movieNum <= toSortNum){
				// msovieArray [i] = left[leftpointer]
				//movieArray[i].values[sortIndex] = leftArray[leftPointer];
				newPtr->next = movieListFront;
				newPtr = newPtr->next;
				movieListFront = movieListFront->next;
				
			}else{
				newPtr->next = toSortListFront;
				newPtr = newPtr->next;
				
				toSortListFront = toSortListFront->next;
			}
		}
		else{
			char* movieString;
			char* toSortString;
		//	if(leftArray[leftPointer][0] == '"'){
			if(strlen(movieListFront->values[sortIndex]) != 0 && movieListFront->values[sortIndex][0] == '"'){
				movieString = &movieListFront->values[sortIndex][1];
			}else{
				movieString = movieListFront->values[sortIndex];
			}

			if(strlen(toSortListFront->values[sortIndex]) != 0 && toSortListFront->values[sortIndex][0] == '"'){
				toSortString = &toSortListFront->values[sortIndex][1];
			}else{
				toSortString = toSortListFront->values[sortIndex];
			}

			if(strcasecmp(movieString, toSortString) <= 0){
				//movieArray[i].values[sortIndex] = leftArray[leftPointer];
				newPtr->next = movieListFront;
				newPtr = newPtr->next;
				movieListFront = movieListFront->next;
			}else{
				//movieArray[i].values[sortIndex] = rightArray[rightPointer];
				newPtr->next = toSortListFront;
				newPtr = newPtr->next;
				toSortListFront = toSortListFront->next;
			}	
		}

	}

	if(toSortListFront == NULL){
		newPtr->next = movieListFront;
	}
	else if(movieListFront == NULL){
		newPtr->next = toSortListFront;
	}

	return head;

}

int main(int argc, char** argv){
 
	fileArray = (char**) malloc(currFileArraySize * sizeof(char*));

	char* currDirectory;
	outputDirectory = NULL;

	if (argc > 1){
		sortedValue = argv[2];
	} else {
		printf("error in argument structure\n");
		return 1;
	}

	if (argc == 3){
		currDirectory = ".";
		outputDirectory = ".";
	} else if (argc == 5) {
		if (strcmp(argv[3], "-d") == 0){
			currDirectory = argv[4];
			outputDirectory = ".";
		} else if (strcmp(argv[3], "-o") == 0){
			currDirectory = ".";
			outputDirectory = argv[4];
		} else {
			printf("error in argument structure\n");
			return 1;
		}
	} else if (argc == 7){
		if ((strcmp(argv[3], "-d") == 0) && (strcmp(argv[5], "-o") == 0)){
			currDirectory = argv[4];
			outputDirectory = argv[6];
		} else if ((strcmp(argv[5], "-d") == 0) && (strcmp(argv[3], "-o") == 0)){
			currDirectory = argv[6];
			outputDirectory = argv[4];
		} else {
			printf("error in argument syntax\n");
			return 1;
		} 
	} else {
		printf("error in argument syntax\n");
		return 1;
	}

	//Validate Directories
	
	DIR *dir;
	DIR *checkDirPointer;
	if(strcmp(currDirectory, ".") != 0){
				
		if((dir = opendir(currDirectory)) == NULL){
			char* newDir = malloc(3+strlen(currDirectory));
			strcpy(newDir, "./");
			strcat(newDir, currDirectory);
			currDirectory = newDir;
		
			if((dir = opendir(currDirectory)) == NULL){
				printf("cannot open start directory\n");
				return 1;
			}
		}

	} else{
		dir = opendir(currDirectory);
	} if(outputDirectory != NULL){
			
		if((checkDirPointer = opendir(outputDirectory)) == NULL){
			char* newDir = malloc(3+strlen(outputDirectory));	
			strcpy(newDir, "./");
			strcat(newDir, outputDirectory);
			outputDirectory = newDir;
		
			if((checkDirPointer = opendir(outputDirectory)) == NULL){
				printf("cannot open output directory\n");
				return 1;
			}
		}
		closedir(checkDirPointer);
	}
	pid_t initialPid = getpid();
	//printf("Initial TID: %d\n", (long) pthread_self());
	printf("Initial PID: %d\n", initialPid);
	printf("TIDS of all child processes: "); 
	fflush(stdout);

	pthread_t tid = 0;
	//printf("%s \n", currDirectory);
	pthread_create(&tid, NULL, threadDir, currDirectory);
	pthread_join(tid, NULL);
	printf("\ntotal number of threads: %d\n", globTotalThreads);	
	printf("total CSVs: %d\n",globTotalSortedFiles);
	if (globTotalSortedFiles > 0) {
		megaSort();	
	}
}

