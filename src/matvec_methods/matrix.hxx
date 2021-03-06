using namespace std;
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include "vector.hxx"
class Mtx{
private:
  int nrows;
  int ncols;
  double *ets;
  int nnz;
  int *index;
  int gleft_offset;
  int gright_offset;
  
public:
  Mtx(int n, double *);
  Mtx(int nrows, double d = 0);
  Mtx(int nrows, int ncols, int gleft_offset=0, int gright_offset=0, double d = 0);
  Mtx(const Mtx &);
  ~Mtx();

  Mtx& operator=(const Mtx&);
  Mtx& operator+=(const Mtx&);
  Mtx& operator-=(const Mtx&);
  Vector operator*(const Vector&) const;
  double& operator()(int i, int j) const{ return ets[ncols*i+j];};
  //  void element(int i, int j) { ets[nrows*i+j]=k;}
  
 double& operator[](int i){
  return(ets[i]);
 }


  int outofbounds(int);
  void assign(int, int, double);
  int size(){return nrows;};
  int col_size(){ return ncols;};
  int gl_offset(){return gleft_offset;}
  int gr_offset(){return gright_offset;}
  int get_nnz();


  Mtx& operator+(); 
  Mtx operator+(const Mtx&) const;
  friend Mtx operator-(const Mtx&); //unary -
  friend Mtx operator-(const Mtx&, const Mtx&);//binary -

  friend ostream& operator<<(ostream &, const Mtx &); 

  void get_csr_matrix(double *data, int *col_indices, int *row_ptr);
  


};

 inline void error(string v) {
  cout << v << endl << "Program exited\n";
  exit(1);
  } 

  
