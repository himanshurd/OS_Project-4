#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
  const int BUFFER_SIZE = 2048;
  
  if (argc < 3) {
    printf("Must Have Atleast Three Arguments.\nFormat:./redirout, destination file, and commands...)");
    exit(0);
  }
  
  int open_fd = open(argv[1], O_RDWR | O_CREAT, 0666);
  int pfd[2];
  pipe(pfd);
  pid_t pid = fork();

  if (pid == 0){
    dup2(pfd[1], 1);
    close(pfd[0]);
    execvp(argv[2], argv + 2);
    exit(0);
  }
  
  close(pfd[1]);
  char *buffer[BUFFER_SIZE];
  int read_bytes= read(pfd[0], buffer, BUFFER_SIZE);
  
  while(read_bytes > 0) {
    write(open_fd, buffer, BUFFER_SIZE);
    break;
  }
  
  close(pfd[0]);
  close(open_fd);
}