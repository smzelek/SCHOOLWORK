/*
  Filename   : Testing.c
  Author     : Steve Zelek
  Course     : CSCI 370
  Assignment : Extra Credit
  Description: Testing the effects of shifting by word size.
*/

/**********************************************************/
// System includes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**********************************************************/
// Function prototypes/global vars/typedefs

/**********************************************************/

int
main (int argc, char* argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Error. Expected: %s mode\nmode = left, right\n", argv[0]);
    exit(-1);
  }

  char *mode = NULL;
  mode = argv[1];

  if (strcmp(mode,"right") && strcmp(mode, "left"))
  {
    fprintf(stderr, "Error. Expected: %s mode\nmode = left, right\n", argv[0]);
    exit(-1);
  }
  
  unsigned base;
  int shiftmax;
  
  printf("Base number in hex:  ");
  scanf("%x", &base);

  printf("Do shifts from 1 to: ");
  scanf("%i", &shiftmax);

  printf("Shift\t  Result  \n");
  printf("-----\t----------\n");
  
  if (strcmp(mode, "left") == 0)
  {
    for (int i = 1; i <= shiftmax; ++i)
    {
      //4 = size of a hex digit
      printf("<< %i\t0x%08X\n", i, base << (i * 4));
    }
  }

  if (strcmp(mode, "right") == 0)
  {
    for (int i = 1; i <= shiftmax; ++i)
    {
      //4 = size of a hex digit
      printf(">> %i\t0x%08X\n", i, base >> (i * 4));
    }
  }
  
  return EXIT_SUCCESS;
}

/**********************************************************/
/**********************************************************/
