#include "inet_socket.h"

int main(int argc, char const *argv[]) {
  int fd;
  char msg[] = "connect";
  struct sockaddr addr;

  if (argc < 3) {
    fprintf(stderr, "udp_worker: addr port\n");
    return -1;
  }

  fd = socket_create(AF_INET, SOCK_DGRAM, 0);
  set_sockaddr(&addr, AF_INET, argv[1], atoi(argv[2]));
  sendto(fd, msg, sizeof(msg), 0, &addr, sizeof(addr));

  close(fd);
  return 0;
}