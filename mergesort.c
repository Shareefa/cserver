#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#ifndef what
#define what
#include "sorter.h"
#endif

char isFloat[28] = {0,0,1,1,1,1,0,1,1,0,0,0,1,1,0,1,0,0,1,0,0,0,1,1,1,1,1,1};


void merge(struct movie* movieArray, int  val, int left, int right){
	//printf("%d\n", isFloat[0]);
	if(left>=right){
		return;
	}else{
		int mid = (right + left)/2;
		merge(movieArray, val, left, mid);
		merge(movieArray, val, mid+1, right);

		//int pointer = left;
		int leftPointer = left;
		int rightPointer = mid+1;
//		char* leftArray[mid+1-left];
//		char* rightArray[right-mid];
		//make two struct movie arrays
		// left and right
		struct movie leftMovieArray[mid + 1 - left];
		struct movie rightMovieArray[right - mid];
		int i = 0;
		
		int counter = 0;
		//populate left array
		for(i = left; i <= mid; i++){
			leftMovieArray[counter] = movieArray[i];
			//leftArray[counter] = movieArray[i].values[val];
			counter++;
		}

		counter = 0;
		//populate right array
		for(i = mid+1; i<=right; i++){
			//do same thing but right
			//rightArray[counter] = movieArray[i].values[val];
			rightMovieArray[counter] = movieArray[i];
			counter++;
		}

		leftPointer = 0; //added to debug, might need to go above this loop 
		rightPointer = 0; //added to debug
		for(i = left; i <= right; i++){
			if(isFloat[val]){
				// leftmoviearray[lefpoint].valeus[val]
				// same for right
				//float leftNum = atof(leftArray[leftPointer]);
				//float rightNum =  atof(rightArray[rightPointer]);

				float leftNum = atof(leftMovieArray[leftPointer].values[val]);
				float rightNum = atof(rightMovieArray[rightPointer].values[val]);
				if(leftNum <= rightNum){
					// msovieArray [i] = left[leftpointer]
					//movieArray[i].values[val] = leftArray[leftPointer];
					movieArray[i] = leftMovieArray[leftPointer];
					leftPointer++;
				}else{
					movieArray[i] = rightMovieArray[rightPointer];
					rightPointer++;
				}
			}
			else{
				char* leftString;
				char* rightString;
			//	if(leftArray[leftPointer][0] == '"'){
				if(leftMovieArray[leftPointer].values[val][0] == '"'){
					leftString = &leftMovieArray[leftPointer].values[val][1];
				}else{
					leftString = leftMovieArray[leftPointer].values[val];
				}

				if(rightMovieArray[rightPointer].values[val][0] == '"'){
					rightString = &rightMovieArray[rightPointer].values[val][1];
				}else{
					rightString = rightMovieArray[rightPointer].values[val];
				}

				if(strcasecmp(leftString, rightString) <= 0){
					//movieArray[i].values[val] = leftArray[leftPointer];
					movieArray[i] = leftMovieArray[leftPointer];
					leftPointer++;
				}else{
					//movieArray[i].values[val] = rightArray[rightPointer];
					movieArray[i] = rightMovieArray[rightPointer];
					rightPointer++;
				}	
			}

			if(leftPointer == mid+1-left){
				i++;
				break;
			}
			if(rightPointer == right-mid){
				i++;
				break;
			}
			
		}
		//for loop to copy rest of left
		//for loop to copy rest of right
		while (leftPointer < mid+1-left){
			//movieArray[i].values[val] = leftArray[leftPointer];
			movieArray[i] = leftMovieArray[leftPointer];
			leftPointer++;
			i++;
		}
		while (rightPointer < right-mid){
			//movieArray[i].values[val] = rightArray[rightPointer];
			movieArray[i] = rightMovieArray[rightPointer];
			rightPointer++;
			i++;
		}
	}

}

void mergesort(struct movie* movieArray, int val, int count){
//	printf("%d\n", count);
	merge(movieArray, val, 0, count-1);	
}
