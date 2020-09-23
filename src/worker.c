#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 64

int main(int argc, char const *argv[]) {
  int fd;
  char buffer[BUFFER_SIZE];

  if (argc < 2) {
    fprintf(stderr, "worker: need file descriptor\n");
    return -1;
  }

  fd = atoi(argv[1]);

  read(fd, &buffer, BUFFER_SIZE);
  printf("worker: connect #%d, message %s\n", getpid(), buffer);

  close(fd);
  return 0;
}
