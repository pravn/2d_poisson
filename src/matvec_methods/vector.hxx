#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string> 
#include <ostream>
#include <stdlib.h>
#include <cmath>

class Vector{
private:
  int length;
  double *vec;
  
public:
  Vector(int, double *d);
  Vector(int = 0, double d = 0);
  Vector(const Vector &);
  ~Vector(){ delete[] vec;};


  int size() const {return length;}
  Vector& operator=(const Vector &);
  Vector& operator+=(const Vector &); //v += v2
  Vector& operator-=(const Vector &); //v -= v2

  double maxnorm() const; // max
  double twonorm() const; // L2
  double& operator[](int i) const {return vec[i];} 


  friend Vector operator+(const Vector &); //unary + : v = +v2;
  friend Vector operator-(const Vector &); //unary - : v = -v2;
  friend Vector operator+(const Vector &, const Vector &); 
                                          //binary + : v = v1 + v2;
  friend Vector operator-(const Vector &, const Vector &);
                                          //binary - : v = v1 - v2;
  
  friend Vector operator*(double, const Vector &);
  friend Vector operator*(const Vector &, double);
  friend Vector operator*(const Vector &, const Vector &);
  friend double dot(const Vector &, const Vector &);
  friend ostream& operator<<(ostream &, const Vector &);
  
};

  
