#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#ifndef what
#define what
#include "sorter.h"
#endif

#define h_addr h_addr_list[0]




void *threadDir(void *vargp);
void *threadCSV(void *vargp);


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
int sockfd;

void error(char * msg){
    perror(msg);
    exit(0);
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
    
    //sort(CSVName, sortedValue);
    
    return NULL;
}


int main(int argc, char *argv[])
{
    //Get the values for the sort value and stuff
    char* currDirectory;
    outputDirectory = NULL;

    int portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc > 6){
        sortedValue = argv[2];
        portno = atoi(argv[6]);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) 
            error("ERROR opening socket");
        server = gethostbyname(argv[4]);
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
            exit(0);
        }

    } else {
        printf("error in argument structure\n");
        return 1;
    }

    if (argc == 7){
        currDirectory = ".";
        outputDirectory = ".";
    } else if (argc == 9) {
        if (strcmp(argv[7], "-d") == 0){
            currDirectory = argv[8];
            outputDirectory = ".";
        } else if (strcmp(argv[7], "-o") == 0){
            currDirectory = ".";
            outputDirectory = argv[8];
        } else {
            printf("error in argument structure\n");
            return 1;
        }
    } else if (argc == 11){
        if ((strcmp(argv[7], "-d") == 0) && (strcmp(argv[9], "-o") == 0)){
            currDirectory = argv[8];
            outputDirectory = argv[10];
        } else if ((strcmp(argv[9], "-d") == 0) && (strcmp(argv[7], "-o") == 0)){
            currDirectory = argv[10];
            outputDirectory = argv[8];
        } else {
            printf("error in argument syntax\n");
            return 1;
        } 
    } else {
        printf("error in argument syntax\n");
        return 1;
    }

    

    char buffer[256];
    
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    printf("Please enter the message: ");
    bzero(buffer,256);
    buffer[0] = '0';
    buffer[1] = '\0';

    n = write(sockfd,buffer,strlen(buffer));
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0) 
         error("ERROR reading from socket");
     
    int sessionNum = atoi(buffer);

    

    
    printf("%d\n", sessionNum);
    
    // n = read(sockfd,buffer,255);
    // if (n < 0) 
    //      error("ERROR reading from socket");
    // printf("%s\n",buffer);
    return 0;
}
