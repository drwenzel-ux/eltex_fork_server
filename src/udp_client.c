#include "inet_socket.h"

int main(int argc, char const *argv[]) {
  int fd;
  char buffer[8];
  struct sockaddr addr;
  char msg[] = "hello from client!";

  fd = socket_create(AF_INET, SOCK_DGRAM, 0);
  set_sockaddr(&addr, AF_INET, "127.0.0.1", 3000);

  sendto(fd, msg, sizeof(msg), 0, &addr, sizeof(addr));
  recvfrom(fd, buffer, sizeof(buffer), 0, NULL, NULL);
  printf("%s\n", buffer);

  return 0;
}
