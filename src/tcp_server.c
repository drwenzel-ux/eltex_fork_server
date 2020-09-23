#include "inet_socket.h"
#include <sys/wait.h>

volatile sig_atomic_t stop;

void sigchild_handler(int sig) {
  pid_t id;
  int status;

  id = wait(&status);
  if (id == -1)
    perror_die("handler: wait return -1", 1);

  printf("child process #%d terminated with status %d\n", id, status);
}

void sigint_handler(int sig) { stop = 1; }

void do_something(pid_t client_fd) {
  int ret;
  char arg[12];

  sprintf(arg, "%d", client_fd);
  char *const args[] = {"./worker", arg};

  ret = execv("./worker", args);

  if (ret == -1)
    perror_die("execv failed", 1);
}

int main(int argc, char const *argv[]) {
  int server_fd;
  int client_fd;

  set_handler(sigchild_handler, SIGCHLD, SA_RESTART);
  set_handler(sigint_handler, SIGINT, 0);

  server_fd = inet_listen("127.0.0.1", 3000, 5);

  while (1) {
    client_fd = inet_accept(server_fd);

    if (stop)
      break;

    if (client_fd == -1) {
      perror("accept failed!");
      continue;
    }

    switch (fork()) {
    case -1:
      perror("fork failed");
      close(client_fd);
      break;

    case 0:
      close(server_fd);
      do_something(client_fd);
      break;

    default:
      close(client_fd);
      break;
    }
  }

  close(server_fd);
  return 0;
}
