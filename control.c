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
  //char a[100];
  //fgets(a, 100, stdin);
  int controller=0;
  if(argc>1){
    if(strcmp(argv[1], "Create")==0){/*printf("I will create\n");*/ controller=1;}
    else if(strcmp(argv[1], "Remove")==0){/*printf("I will remove\n");*/ controller=-1;}
    else {printf("I do not understand\n");}
  }else{printf("No argument provided!\n");}

  if(controller==1){
    //printf("%s\n",strerror(errno));
    int shmd;
    shmd=shmget(KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0640);
    int data[2];
    int* dpointer = data;
    dpointer = shmat(shmd, 0, 0);
    dpointer[0] = open("log.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    dpointer[1]= 0;
    //printf("DATA = %d\n",*data);
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
    int n[2];
    int* npointer=n;
    char s[100];
    log=shmget(KEY, 0, 0);
    npointer=shmat(log, 0, 0);
    //printf("RETRIEVED SHM VALUE = %d\n",*n);
    read(npointer[0], s, sizeof(s));
    printf("%s\n",s);
    int semd=semget(KEY2, 1, 0);
    semctl(semd, IPC_RMID, 0);
    int shmd=shmget(KEY, 0, 0);
    shmctl(shmd, IPC_RMID, 0);
  }
  return 0;
}
