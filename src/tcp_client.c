#include "inet_socket.h"

int main(int argc, char const *argv[]) {
  int fd;
  char msg[] = "hello from client!";

  fd = inet_connect("127.0.0.1", 3000, SOCK_STREAM);
  write(fd, msg, sizeof(msg));

  return 0;
}
