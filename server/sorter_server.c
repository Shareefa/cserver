#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include "sorter_server.h"
#include <arpa/inet.h>
struct movie ** movies;
pthread_mutex_t * locks;
char* sortValues[100];
int counter=0;
pthread_mutex_t sessionLock;

void error(char *msg)
{
	perror(msg);
	exit(1);
}

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
			//  movie->values[valueIndex] = (char*) malloc (sizeof(char)*(back-front+2));//char arr[(back - front + 2)];
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
		//  movie -> values[valueIndex] = (char*) malloc (sizeof(char)*(back-front+2));
		movie->values[valueIndex][back-front+1] = '\0';
		strncpy(movie->values[valueIndex], &line[front], back-front+1);
		//  printf("%s\n", movie->values[valueIndex]);
		if(line[charPointer] != '\0'){
			charPointer++;
		}
		valueIndex++;

	}

	if(movie->values[27] == NULL){
		movie->values[27] = "";
	}

}

void * listenClient(void * vargp){

	char buffer[512];
	int portno, clilen;
	int n;
	int movieCounter = 0;
	int * sockfd=vargp;
	int newsockfd= *(sockfd);
	if (newsockfd < 0) 
		error("ERROR on accept");
	while(1){
		n = read(newsockfd,buffer,3);
		int req = atoi(buffer);
		//printf("Request: %d\n", req);

		if(req==-1){


			//counter++;
			char  num[20];
			sprintf(num,"%d",counter);
			bzero(buffer,512);
			strcpy(buffer, num);
			write(newsockfd, buffer, 20);
			pthread_mutex_lock(&sessionLock);
			counter ++;
			pthread_mutex_unlock(&sessionLock);
		}else if(req==0){

			char num[3];
			read(newsockfd, buffer, 20);
			int index = atoi(buffer);
			bzero(buffer,512);
			read(newsockfd,buffer,511);
				
			while(strstr(buffer,"*$$*")==NULL){
				if (n < 0) error("ERROR reading from socket");
				printf("Here is the message: %s\n",buffer);

				pthread_mutex_lock(&locks[index]);
				tokenize(buffer, &(movies[index][movieCounter]));
				movieCounter++;
				pthread_mutex_unlock(&locks[index]);
				bzero(buffer, 512);
				read(newsockfd,buffer,511);
				
			}
			bzero(buffer,512);

		}else if(req==1){

			char num[3];
			read(newsockfd, buffer, 20);
			int index = atoi(buffer);	
			pthread_mutex_lock(&locks[index]);

			read(newsockfd, buffer, 255);
			//strcpy(sortedValues[index], buffer);
			char* sortValue = (char*) malloc(255);
			strcpy(sortValue, buffer);

			printf("TESTING THE TOKENIZE \n");
			for(int i = 0; i < 26; i++){
				//printf("%s,", movies[index][0].values[i]);
			}			
			//printf("\n");

			int sortIndex = 0;
			printf("SORTING OCCURS HERE \n");
			if (strcmp(sortValue,"color") == 0) {
				mergesort(movies[index], 0, movieCounter);
				sortIndex = 0;
			} else if (strcmp(sortValue,"director_name") == 0){
				mergesort(movies[index],1, movieCounter);
				sortIndex = 1;
			} else if (strcmp(sortValue,"num_critic_for_reviews") == 0){
				mergesort(movies[index],2, movieCounter);
				sortIndex = 2;
			} else if (strcmp(sortValue,"duration") == 0){
				mergesort(movies[index],3, movieCounter);
				sortIndex = 3;
			} else if (strcmp(sortValue,"director_facebook_likes") == 0) {
				mergesort(movies[index], 4, movieCounter);
				sortIndex = 4;
			} else if (strcmp(sortValue,"actor_3_facebook_likes") == 0) {
				mergesort(movies[index], 5, movieCounter);
				sortIndex = 5;
			} else if (strcmp(sortValue,"actor_2_name") == 0) {
				mergesort(movies[index], 6, movieCounter);
				sortIndex = 6;
			} else if (strcmp(sortValue,"actor_1_facebook_likes") == 0) {
				mergesort(movies[index], 7, movieCounter);
				sortIndex = 7;
			} else if (strcmp(sortValue,"gross") == 0) {
				mergesort(movies[index], 8, movieCounter);
				sortIndex = 8;
			} else if (strcmp(sortValue,"genres") == 0) {
				mergesort(movies[index], 9, movieCounter);
				sortIndex = 9;
			} else if (strcmp(sortValue,"actor_1_name") == 0) {
				mergesort(movies[index], 10, movieCounter);
				sortIndex = 10;
			} else if (strcmp(sortValue,"movie_title") == 0) {
				mergesort(movies[index], 11, movieCounter);
				sortIndex = 11;
			} else if (strcmp(sortValue,"num_voted_users") == 0) {
				mergesort(movies[index], 12, movieCounter);
				sortIndex = 12;
			} else if (strcmp(sortValue,"cast_total_facebook_likes") == 0) {
				mergesort(movies[index], 13, movieCounter);
				sortIndex = 13;
			} else if (strcmp(sortValue,"actor_3_name") == 0) {
				mergesort(movies[index], 14, movieCounter);
				sortIndex = 14;
			} else if (strcmp(sortValue,"facenumber_in_poster") == 0) {
				mergesort(movies[index], 15, movieCounter);
				sortIndex = 15;
			} else if (strcmp(sortValue,"plot_keywords") == 0) {
				mergesort(movies[index], 16, movieCounter);
				sortIndex = 16;
			} else if (strcmp(sortValue,"movie_imdb_link") == 0) {
				mergesort(movies[index], 17, movieCounter);
				sortIndex = 17;
			} else if (strcmp(sortValue,"num_user_for_reviews") == 0) {
				mergesort(movies[index], 18, movieCounter);
				sortIndex = 18;
			} else if (strcmp(sortValue,"language") == 0) {
				mergesort(movies[index], 19, movieCounter);
				sortIndex = 19;
			} else if (strcmp(sortValue,"country") == 0) {
				mergesort(movies[index], 20, movieCounter);
				sortIndex = 29;
			} else if (strcmp(sortValue,"content_rating") == 0) {
				mergesort(movies[index], 21, movieCounter);
				sortIndex = 21;
			} else if (strcmp(sortValue,"budget") == 0) {
				mergesort(movies[index], 22, movieCounter);
				sortIndex = 22;
			} else if (strcmp(sortValue,"title_year") == 0) {
				mergesort(movies[index], 23, movieCounter);
				sortIndex = 23;
			} else if (strcmp(sortValue,"actor_2_facebook_likes") == 0) {
				mergesort(movies[index], 24, movieCounter);
				sortIndex = 24;
			} else if (strcmp(sortValue,"imdb_score") == 0) {
				mergesort(movies[index], 25, movieCounter);
				sortIndex = 25;
			} else if (strcmp(sortValue,"aspect_ratio") == 0) {
				mergesort(movies[index], 26, movieCounter);
				sortIndex = 26;
			} else if (strcmp(sortValue,"movie_facebook_likes") == 0) {
				mergesort(movies[index], 27, movieCounter);
				sortIndex = 27;
			} else {
				//printf("Please enter a valid type");
				//exit(1);
				pthread_exit(NULL);
//				return -1;
			}
			char  status[2];
		//	printf("TESTING THE TOKENIZE \n");
			for(int j = 0; j < movieCounter; j++){
				char * line = (char *) malloc(500);
				bzero(line,500);
				for(int i = 0; i < 27; i++){
					strcat(line, movies[index][j].values[i]);
					strcat(line, ",");
					//("%s,", movies[index][j].values[i]);
				}
				strcat(line,movies[index][j].values[27]);
				int s=write(newsockfd, line, 500);
				if(s<0)printf ("ERRORR");
				read(newsockfd,status,2);
			}
			//printf("\n");
			
			write(newsockfd, "*****", 500);



			pthread_mutex_unlock(&locks[index]);
			break;

		}

	}	


	//fflush(stdout);

	//printf("%s\n",buffer);

	//counter++;
	n = write(newsockfd,buffer,2);
	if (n < 0) error("ERROR writing to socket");

}

int main(int argc, char *argv[])
{   

	movies=(struct movie **) malloc(sizeof(struct movie *)*100);
	for (int i = 0; i < 100; i++){
		movies[i] = (struct movie *) malloc(sizeof(struct movie) * 12000);
		for (int j = 0; j < 12000; j++){
			for (int k = 0; k < 28; k++){
				movies[i][j].values[k] = (char*) malloc(sizeof(char));
			}
		}
	}
	locks=(pthread_mutex_t *) malloc(sizeof(pthread_mutex_t)*100);
	printf("Received connections from: ");
	fflush(stdout);
	int sockfd, newsockfd, portno, clilen, counter;
	counter = 0;
	struct sockaddr_in serv_addr, cli_addr;
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
				sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	while(1){

		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		
		struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&cli_addr;
		struct in_addr ipAddr = pV4Addr->sin_addr;
		char str[INET_ADDRSTRLEN];
		
		inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );
		printf("%s,", str);
		fflush(stdout);
		pthread_t tid;
		pthread_create(&tid,NULL,listenClient,&newsockfd);

	}
	return 0; 
}
