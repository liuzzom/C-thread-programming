#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define SIZE 16

int main(void){
	pid_t pid;	
	int pide_fds[2];
	pipe(pide_fds);
	char *buffer = malloc(SIZE*sizeof(char));

	pid = fork();

	if(pid < 0){
		perror("Error on fork");
		exit(1);
	}

	if(pid == 0){
		//child process
		close(pide_fds[0]);
		int numbers[10], i, max = -999;
		//random number generation
		for(i = 0; i < 10; i++){
			numbers[i] = rand()%10 +1;
			printf("%d ", numbers[i]);
			if(max < numbers[i]){
				max = numbers[i];
			}
		}
		puts("");
		printf("max:%d\n", max);

		memset(buffer, 0, SIZE);
		sprintf(buffer, "%d", max);
		write(pide_fds[1], buffer, SIZE-1);
	}else{
		//parent process
		close(pide_fds[1]);
		memset(buffer, 0, SIZE);
		read(pide_fds[0], buffer, SIZE);
		int n = atoi(buffer);
		pid_t pid_exec;
		int i;

		for(i = 0; i < n; i++){
			pid_exec = fork();

			if(pid_exec == 0){
				execl("/bin/echo", "echo", "hello", (char*)0);
			}else{
				wait(NULL);
			}
		}
	}

	srand(time(NULL));
}