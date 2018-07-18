#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct threadParams{
	int* array;
	int startPos;
	int stopPos;
}Params;

void* threadFunc(void* args){
	Params* p = (Params*) args;
	int* sommaParziale = (int*) malloc(sizeof(int));
	int i;
	*sommaParziale = 0;
	for(i = p->startPos; i < p->stopPos; i++){
		*sommaParziale += p->array[i];
	}
	return (void*)sommaParziale;
}

int* randArray(int numeroElementi, int max){
	int* array = (int*) malloc(numeroElementi*sizeof(int));
	int i;
	for(int i = 0; i < numeroElementi; i++){
		array[i] = rand() % (max + 1);
	}
	return array;
}

void printArray(int* array, int dim){
	int i;
	printf("[");
	for(int i = 0; i < dim; i++){
		printf("%d ",array[i]);
	}
	printf("]\n");
}

int main(){
	srand(time(NULL));
	int numeroThread = 10;
	int dim = 4;
	int* array = randArray(dim,10);
	//int array[4] = {1, 2, 3, 4};
	int i;
	pthread_t* threads = (pthread_t*) malloc(numeroThread*sizeof(pthread_t));
	Params* paramArray = (Params*) malloc(numeroThread*sizeof(pthread_t));
	int somma = 0;
	int* tmp = 0;
	
	printArray(array,dim);
	
	for(i = 0; i < numeroThread; i++){
		paramArray[i].array = array;
		paramArray[i].startPos = ((i*dim)/numeroThread); 
		paramArray[i].stopPos = (((i+1)*dim)/numeroThread);
		pthread_create(&threads[i], NULL, &threadFunc, (void*)&paramArray[i]);
	}
	for(i = 0; i < numeroThread; i++){
		pthread_join(threads[i],(void**)&tmp);
		somma += *tmp;
	}
	printf("La somma degli elementi dell'array è: %d\n",somma);
	printf("La media è: %.2lf\n",(double)somma/dim);
}
