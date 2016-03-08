using namespace std;
#include "vector.hxx"
#include "matrix.hxx"

int main(){
  int k = 4;
  double *mt = new double[k*k];
  
  for (int i=0; i<k; i++)
    for (int j=0; j<k; j++)
      mt[k*i+j]=2*i*j + i + 10;
  
  Mtx m1(k,mt);
  Mtx m2(k, 5);
  Mtx m3(k);
  Mtx m4(k);


  for(int i=0; i<k; i++)
    for(int j=0; j<k; j++)
      m3(i,j)=k*i+j;


    m4 -= m3;

    cout << m4;

  delete[] mt;

}

  
