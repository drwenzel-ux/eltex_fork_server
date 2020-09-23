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

void do_something(in_addr_t addr, in_port_t port) {
  int ret;
  char inet[INET_ADDRSTRLEN];
  char iport[6];

  inet_ntop(AF_INET, &addr, inet, INET_ADDRSTRLEN);
  sprintf(iport, "%d", htons(port));

  char *const args[] = {"./udp_worker", inet, iport, NULL};

  if ((ret = execv("./udp_worker", args)) == -1) {
    perror("execv failed");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char const *argv[]) {
  char buffer[64];
  int server_fd;
  ssize_t num;

  struct sockaddr_in st;
  socklen_t len = sizeof(struct sockaddr_in);

  set_handler(sigchild_handler, SIGCHLD, SA_RESTART);
  set_handler(sigint_handler, SIGINT, 0);

  server_fd = inet_bind("127.0.0.1", 3000, SOCK_DGRAM);

  while (1) {
    num = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&st,
                   &len);

    if (stop)
      break;

    if (num == -1) {
      perror("recvfrom failed");
      break;
    }

    switch (fork()) {
    case -1:
      perror("fork failed");
      break;

    case 0:
      close(server_fd);
      do_something(st.sin_addr.s_addr, st.sin_port);
      break;

    default:
      break;
    }
  }

  close(server_fd);
  return 0;
}
