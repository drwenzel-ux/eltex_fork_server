#include "inet_socket.h"

int main(int argc, char const *argv[]) {
  int fd;
  char buffer[8];
  struct sockaddr_in addr;
  char msg[] = "hello from client!";

  fd = socket_create(AF_INET, SOCK_DGRAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  addr.sin_port = htons(3000);

  sendto(fd, msg, sizeof(msg), 0, (struct sockaddr *)&addr, sizeof(addr));
  recvfrom(fd, buffer, sizeof(buffer), 0, NULL, NULL);
  printf("%s\n", buffer);
  return 0;
}
