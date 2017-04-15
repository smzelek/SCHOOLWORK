/*
  Filename   : ReplaceBytes.c
  Author     : Steve Zelek
  Course     : CSCI 370
  Assignment : Lab4
  Description: Allows the user to replace a specified byte
    in a hexadecimal integer.
*/

/**********************************************************/
// System includes

#include <stdio.h>
#include <stdlib.h>

/**********************************************************/
// Function prototypes/global vars/typedefs

unsigned
replace_byte (unsigned x, int i, unsigned char b);

/**********************************************************/

int
main (int argc, char* argv[])
{
  unsigned x;
  int i;
  unsigned b;
  
  printf("x ==> ");
  scanf("%10x", &x);
 
  printf ("i ==> ");
  scanf("%i", &i);

  printf ("b ==> ");
  scanf("%4x", &b);

  printf("Replacing byte %i of 0x%X with 0x%X\n  results in 0x%X.\n", i, x, b, replace_byte(x, i, b));

  return EXIT_SUCCESS;
}

/**********************************************************/

unsigned
replace_byte (unsigned x, int i, unsigned char b)
{
  unsigned mask = 0xFF;
  mask <<= i * 8;
  mask = ~mask; 

  unsigned temp = b;
  temp <<= i * 8;

  x = x & mask;
  x += temp;

  return x;
}

/**********************************************************/
/**********************************************************/
