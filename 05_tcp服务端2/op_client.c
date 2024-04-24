#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[]) 
{
  int sock;
  char opmsg[BUF_SIZE];
  struct sockaddr_in serv_adr;

  if (argc != 3) {
    printf("Usage : %s<IP> <port>\n", argv[0]);
    exit(1);
  }

  sock=socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    error_handling("socket() error");
  }

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_adr.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) 
    error_handling("connect() error");
  else 
    puts("Connect.......");

  fputs("Operand count: ", stdout);
  scanf("%d", &opnd_cnt);

  for (int i = 0; i < opnd_cnt; i ++) {
    printf("Operand %")
  }
}