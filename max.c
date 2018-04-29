#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#define SIZE 10

struct params{
	int *numbers;
	size_t startPos, endPos;
};

typedef struct params params;

void* max(void* args){
	params* pp = (params*) args;
	int i;
	int* max = malloc(sizeof(int));
	*max = INT_MIN;

	for(i = pp->startPos; i < pp->endPos; i++){
		if(*max < pp->numbers[i]){
			*max = pp->numbers[i];
		}
	}

	return (void*) max;
}

int main(int argc, char** argv){
	int array[SIZE], numThread = 2, maxVal = INT_MIN, i, step, *tmp;
	srand(time(NULL));

	if(argc < 2){
		printf("Verranno usati 2 Thread(opzione di default)\n");
	}else{
		numThread = atoi(argv[1]);
	}

	for(i = 0; i < SIZE; i++){
		array[i] = rand()%50 +1;
		printf("%d ", array[i]);
	}
	puts("");

	step = SIZE/numThread;

	if(SIZE%numThread == 0){
		pthread_t* threads_ids = malloc(numThread*sizeof(pthread_t));
		params* threads_args = malloc(numThread*sizeof(params));

		for(i = 0; i < numThread; i++){
			threads_args[i].numbers = array;
			threads_args[i].startPos = step*i;
			threads_args[i].endPos = step*(i+1);
			pthread_create(&threads_ids[i], NULL, &max, &threads_args[i]);
		}

		for(i = 0; i < numThread; i++){
			pthread_join(threads_ids[i], (void**)&tmp);
			if(maxVal < *tmp){
				maxVal = *tmp;
			}
		}
		printf("Max:%d\n", maxVal);
	}else{
		pthread_t* threads_ids = malloc((numThread+1)*sizeof(pthread_t));
		params* threads_args = malloc((numThread+1)*sizeof(params));

		for(i = 0; i < numThread; i++){
			threads_args[i].numbers = array;
			threads_args[i].startPos = step*i;
			threads_args[i].endPos = step*(i+1);
			pthread_create(&threads_ids[i], NULL, &max, &threads_args[i]);
		}
		threads_args[numThread].numbers = array;
		threads_args[numThread].startPos = SIZE-(SIZE%numThread);
		threads_args[numThread].endPos = SIZE;
		pthread_create(&threads_ids[numThread], NULL, &max, &threads_args[numThread]);

		for(i = 0; i < numThread; i++){
			pthread_join(threads_ids[i], (void**)&tmp);
			if(maxVal < *tmp){
				maxVal = *tmp;
			}
		}
		pthread_join(threads_ids[numThread], (void**)&tmp);
		if(maxVal < *tmp){
			maxVal = *tmp;
		}
		printf("Max:%d\n", maxVal);
	}
}