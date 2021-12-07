#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
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

int main(int argc, char*argv[]){
  int controller=0;
  if(argc>1){
    if(strcmp(argv[1], "Create")==0){/*printf("I will create\n");*/ controller=1;}
    else if(strcmp(argv[1], "Remove")==0){/*printf("I will remove\n");*/ controller=-1;}
    else {printf("I do not understand\n");}
  }else{printf("No argument provided!\n");}

  if(controller==1){
    //printf("%s\n",strerror(errno));
    int shmd;
    shmd=shmget(KEY, 8, IPC_CREAT | IPC_EXCL | 0640);
    int *data;
    data = shmat(shmd, 0, 0);
    int fd;
    fd = open("log.data", O_RDWR | O_CREAT | O_TRUNC, 0644);
    char m[20]="Once upon a time....";
    write(fd,m,sizeof(m));
    *data = sizeof(m);
    //printf("ORIGINAL INT POINTER = %p\n",dpointer);
    int semd, v, r;
    semd = semget(KEY2, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (semd == -1) {
      printf("error %d: %s\n", errno, strerror(errno));
      semd = semget(KEY, 1, 0);
      v = semctl(semd, 0, GETVAL, 0);
      //printf("semctl returned: %d\n", v);
    } else {
      union semun us;
      us.val = 1;
      r = semctl(semd, 0, SETVAL, us);
      //printf("semctl returned: %d\n", r);
    }
  }

  if(controller==-1){
    int log;
    int fd2;
    int *data;
    char s[100];

    log=shmget(KEY, 0, 0);
    data=shmat(log, 0, 0);
    //printf("CHECK = %d\n",p2->length);
    fd2=open("log.data", 0, 0);
    read(fd2, s, sizeof(s));
    printf("%s\n",s);

    int semd=semget(KEY2, 1, 0);
    semctl(semd, IPC_RMID, 0);
    int shmd=shmget(KEY, 0, 0);
    shmctl(shmd, IPC_RMID, 0);
  }
  return 0;
}
