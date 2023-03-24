#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  pid_t pid, my_pid;

  my_pid = getpid();
  printf("%d] A0\n", my_pid);
  printf("%d] B0\n", my_pid);
  

  pid = fork();
  printf("%d\n", pid);

  if(pid == 0) {
    my_pid = getpid();
    printf("%d] B1\n", my_pid);
    printf("%d] Bye\n", my_pid);
  }
  else {
    printf("%d] C0\n", my_pid);
    pid = fork();
    if(pid == 0) {
      my_pid = getpid();
      printf("%d] C1\n", my_pid);
    }
    printf("%d] Bye\n", my_pid);

  }

  
    


  return 1;
}
