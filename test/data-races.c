#include <stdio.h>
#include <stdlib.h>

#define MAX 100000000

int foo()
{
  return 0;
}

int main(int argc, char **argv) 
{
  int *a = (int *) malloc(sizeof(int) * MAX);
  int i;

  // Vector initialization
#pragma omp parallel for
  for (i = 0; i < MAX; ++i) {
    a[i] = i;
  }

  foo();

#pragma omp parallel for
  for (i = 0; i < MAX - 1; ++i) {
    a[i] = a[i + 1] + 1;
  }

  printf("Terminated!\n");

  exit(0);
}
