#include <cstdlib>
#include <iostream>
#include <string>

int
main (int argc, char* argv[])
{
  int* array1 = new int[10];
  for (int i = 0; i < 11; ++i)
  {
    array1[i] = i;
    std::cout << array1[i] << ", ";
  }
}
