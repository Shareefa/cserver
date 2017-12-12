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
struct movie ** movies;
pthread_mutex_t * locks;

void error(char *msg)
{
	perror(msg);
	exit(1);
}

/*void tokenize(char* line, struct movie* movie){
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

}*/

int main(int argc, char *argv[])
{   

	movies=(struct movie **) malloc(sizeof(struct movie *)*100);
	locks=(pthread_mutex_t *) malloc(sizeof(pthread_mutex_t)*100);
	int sockfd, newsockfd, portno, clilen, counter;
	counter = 0;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
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
		if (newsockfd < 0) 
			error("ERROR on accept");
		n = read(newsockfd,buffer,3);
		int req = atoi(buffer);
		printf("Request: %d\n", req);
		//fflush(stdout);
		char  num[20];
		sprintf(num,"%d",counter);
		bzero(buffer,256);
		strcpy(buffer, num);
		write(newsockfd, buffer, 20);
		counter ++;

		while(strstr(buffer,"Done")==NULL){

			bzero(buffer,256);

			read(newsockfd,buffer,255);
			if (n < 0) error("ERROR reading from socket");
			printf("Here is the message: %s\n",buffer);

		}
		//printf("%s\n",buffer);

		//counter++;
		n = write(newsockfd,buffer,2);
		if (n < 0) error("ERROR writing to socket");

	}
	return 0; 
}
