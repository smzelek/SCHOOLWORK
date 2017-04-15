#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void 
printBits (const void* valuePtr, size_t size);

int main (int argc, char* argv[])
{
  unsigned x;
  printf("x ==> ");
  scanf("%d", &x);

  printBits(&x, sizeof(int));
}

void 
printBits (const void* valuePtr, size_t size)
{

  for (int b = size-1; b >= 0; --b)
  {
    unsigned char* byte = (unsigned char*) valuePtr + b;
    for (int i = 7; i >= 0; --i)
    {
      unsigned mask = 0x1;
      unsigned data = *byte >> i;
      printf("%x", data & mask);
      if (b*8 + i == 31 || b*8 + i == 23)
        printf(" ");
    }
  }
  printf("\n");
}
