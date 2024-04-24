#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 1024

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[])
{
  int sock;
  char message[BUF_SIZE];
  int str_len;
  struct sockaddr_in serv_adr;

  if (argc != 3) {
    printf("Usage : %s <IP> <port> \n", argv[0]);
    exit(1);
  }

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1) error_handling("socket() error!");

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_adr.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
    error_handling("connect() error!");
  } else {
    puts("Connect .....");
  }

  while (1) {
    fputs("Input message(Q to quit): ", stdout);
    fgets(message, BUF_SIZE, stdin);

    if (!strcmp(message, "Q\n") || !strcmp(message, "q\n")) {
      break;
    }

    int str_len = write(sock, message, sizeof(message));

    int recv_len = 0;
    while (recv_len < str_len) {
      int recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
      if (recv_len == - 1) {
        error_handling("read() error!");
      }
      recv_len += recv_cnt;
    }

    message[str_len] = 0;
    printf("Message from server: %s", message);
  }

  close(sock);
  return 0;
}