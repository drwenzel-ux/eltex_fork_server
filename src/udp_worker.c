#include "inet_socket.h"

int main(int argc, char const *argv[]) {
  int fd;
  char msg[] = "connect";
  struct sockaddr_in addr;

  if (argc < 3) {
    fprintf(stderr, "udp_worker: addr port\n");
    return -1;
  }

  fd = socket_create(AF_INET, SOCK_DGRAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));

  sendto(fd, msg, sizeof(msg), 0, (struct sockaddr *)&addr, sizeof(addr));

  close(fd);
  return 0;
}