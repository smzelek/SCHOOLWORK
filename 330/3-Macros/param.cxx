// Parameter assignment for CS 330 - Spring 2009
// Steve Zelek, 11/15/16

#include "/home/grader/hutchens330/labs/param/tvector.h"
#include "/home/grader/hutchens330/labs/param/tmatrix.h"

#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

// performs a summation using an expression over the interval start to stop
//   and stores it in sum
#define jensen(expr, index, start, stop, sum) {\
  for (index = start; index < stop; ++index){\
      sum += expr;\
  }\
}

// computes the norm for a single vector
// norm: the square root of the sum of the squares of a vector's elements
double norm(tvector<double>& vec) {
  double sum = 0;
  int index;
  jensen (vec[index] * vec[index], index, 0, vec.size(), sum);
  return sqrt(sum); 
}

// computes the dot product for two vectors
// dot product: the sum of the products of two vector's corresponding elements
// precondition: the two vectors must have the same length
double multiply(tvector<double>& vec1, tvector<double>& vec2) {
  double sum = 0;
  int index;
  assert(vec1.size() == vec2.size());
  jensen (vec1[index] * vec2[index], index, 0, vec1.size(), sum);
  return sum;
}

// performs matrix multiplication given two factors and returns the resultant matrix
// precondition: M1cols must be equal to M2rows
// postcondition: a matrix of dimension [M1rows][M2cols] is created
tmatrix<double>& multiply(tmatrix<double>& mat1, tmatrix<double>& mat2){
  assert(mat1.numcols() == mat2.numrows());
  tmatrix<double>& result = *(new tmatrix<double>(mat1.numrows(), mat2.numcols()));
  
  for (int i = 0; i < result.numrows(); ++i){
    for (int j = 0; j < result.numcols(); ++j){
      double out = 0;
      int index;
      jensen(mat1[i][index] * mat2[index][j], index, 0, mat1.numcols(), out);
      result[i][j] = out;
    }
  }
    
  return result;
}

// reads in values for a vector
void read(tvector<double>& vec){
  for (int i = 0; i < vec.size(); ++i){
      cin >> vec[i];
  }
}

// prints the values stored in a vector
void write(tvector<double>& vec){
  int size = vec.size();
  for (int i = 0; i < size; ++i){
      printf("%8.2f", vec[i]);
  }
  cout << endl;
}

// reads in values for a matrix in row major order
void read(tmatrix<double>& mat){
  for (int i = 0; i < mat.numrows(); ++i){
    for (int j = 0; j < mat.numcols(); ++j){
      cin >> mat[i][j];
    }
  }
}

// prints the values for a matrix in a format matching its dimensions
// prints with 2 decimal places of accuracy
void write(tmatrix<double>& mat)  {
  for (int i = 0; i < mat.numrows(); ++i){
    for (int j = 0; j < mat.numcols(); ++j){
      printf("%10.2f", mat[i][j]);
    }
    cout << endl;
  }
}

int main( )
{
  tvector<double> & a = *(new tvector<double>(2));
  read(a);
  
  cout << "a is ";
  write(a);
  
  cout << endl;
  cout << "Norm of a is ";
  printf("%.2f\n", norm(a));
  
  tvector<double> & b = *(new tvector<double>(2));
  read(b);
  
  cout << "b is ";
  write(b);
  
  cout << endl;
  cout << "Norm of b is ";
  printf("%.2f\n", norm(b));
  
  cout << "Dot product of a and b is ";
  printf("%.2f\n", multiply(a, b));
  
  tmatrix<double> & c = *(new tmatrix<double>(2, 3));
  tmatrix<double> & d = *(new tmatrix<double>(3, 4));
  
  read(c);
  
  cout << "c is" << endl;
  write(c);
  
  
  read(d);
  
  cout << "d is" << endl;
  write(d);
  
  tmatrix<double> & result = multiply(c, d);
  
  cout << "The product of c and d is" << endl;
  write(result);
  
  return 0;
}


