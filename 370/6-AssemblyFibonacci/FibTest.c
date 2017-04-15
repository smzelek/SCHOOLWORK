#include <stdio.h>
#include <stdlib.h>

int fib(int n);

int main(int argc, char* argv[])
{
  printf("%d", fib(5));

}

int fib(int n)
{
  if (n < 2)
    {
      return 1;
    }

  return fib(n-1) + fib(n-2);

}
