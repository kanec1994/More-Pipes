#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

void print_powers() { 
	 int i = 0, limit = 4; 
	 for(i = 0; i <= limit; i++) { 
	 	printf("%d\n", i * i); 
	 } 
}

int main(){
	pid_t pid;
	int fd;
	int status;
	int pipeDesc[2];
	char buffer[30];
	
	if(pipe(pipeDesc) == -1){
		perror("Pipe failed!\n");
		exit(1);
	}

	if((pid = fork()) < 0) {
		perror("Fork failed!\n");
		exit(1);
	}
	
	if(pid == 0){
		close(pipeDesc[1]);
		if(read(pipeDesc[0], buffer, sizeof(buffer)) <= 0 ) {
			perror("Child read failed!\n");
			exit(1);
		}
		fd = open(buffer, O_CREAT | O_WRONLY, 0666);
		if(fd < 0){
			perror("Open File Failed");
			exit(1);
		}
		if(dup2(fd, 1) == -1){
			perror("Stdout redirect failed!");
			exit(1);
		}
		print_powers();
		close(fd);
		close(pipeDesc[0]);
		exit(0);
	}
	else{
		close(pipeDesc[0]);
		scanf("%s", buffer);
		if(write(pipeDesc[1], buffer, strlen(buffer)+1) <= 0){
			perror("Parent write failed!\n");
			exit(1);
		}
		waitpid(pid, &status, 0);
		close(pipeDesc[1]);
	}
}
