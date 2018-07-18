#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>


void* is_prime(void* args){
	int* number = (int*) args;
	int i;
	int* isPrime = (int*) malloc(sizeof(int));

	for(i = 2; i <= *number/2; i++){
		if(*number%i == 0){
			*isPrime = 1;
			return(void*) isPrime;
		}
	}

	printf("%d è primo...\n", *number);
	*isPrime = 0;
	return(void*) isPrime;
}

int main(int argc, char** argv){
	int primesToFind = 0, primesFound = 0, currentNumber = 2, i;
	int numThread = 2, *isPrime;
	pthread_t* threads_ids = malloc(numThread*sizeof(pthread_t));
	int *numbers = malloc(numThread*sizeof(int));

	if(argc == 2){
		primesToFind = atoi(argv[1]);
	}
	

	while(primesFound < primesToFind){
		for(i = 0; i < numThread; i++){
			numbers[i] = currentNumber;
			pthread_create(&threads_ids[i], NULL, &is_prime, &numbers[i]);
			currentNumber++;
		}
		for(i = 0; i < numThread; i++){
			pthread_join(threads_ids[i], (void**) &isPrime);
			if(*isPrime == 0){
				primesFound++;
			}
		}

	}

	printf("numeri trovati:%d ultimo numero primo trovato:%d\n", primesFound, currentNumber-1);
}