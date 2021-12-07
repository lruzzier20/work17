#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define KEY 24601
#define KEY2 24602

union semun {
  int              val;
  struct semid_ds *buf;
  unsigned short  *array;
  struct seminfo  *__buf;
};


int main(){
  int semd, r, v;

  semd = semget(KEY2, 1, 0);
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  semop(semd, &sb, 1);

  int fd;
  int shmd;
  int* data;
  int in=0;
  int size=0;
  char s[50];
  fd = open("log.data", O_RDWR);
  shmd=shmget(KEY, 0, 0);
  data=shmat(shmd, 0, 0);
  //printf("RETRIEVED INT POINTER= %p\n",spointer);
  lseek(fd, -(*data), SEEK_END);
  read(fd, s, *data+1);
  printf("Previous Line: %s\n",s);
  char a[100];
  printf("Add a line: ");
  fgets(a, 100, stdin);
  while(a[in]!='\n'){
    size++;
    in++;
  }
  printf("a = %s, size of a = %d",a, size+1);
  write(fd, a, size+1);
  *data=size+1;

  sb.sem_op = 1;
  semop(semd, &sb, 1);
  return 0;
}
