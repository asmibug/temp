#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSG_PERM 0666

int main() {
  if (!fork()) {
    execl("/tmp/test", "/tmp/test", NULL);
  }

  key_t key = ftok("/tmp/msg.temp", 1);

  int msgq = msgget(key, MSG_PERM | IPC_CREAT);

  struct my_msg {
    long mtype;
    char mtext[80];
  } data;
  bzero(&data, sizeof(data));

  ssize_t msglen = msgrcv(msgq, &data, 80, 0, 0);
  if (msglen > 0) {
    perror("msgrcv");
    exit(EXIT_FAILURE);
  }

  int f = open("/home/box/message.txt", O_WRONLY | O_TRUNC);

  write(f, data.mtext, msglen);

  close(f);

  exit(EXIT_SUCCESS);
}
