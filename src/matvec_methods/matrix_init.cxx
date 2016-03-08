using namespace std;
#include "matrix.hxx"
#include <ostream>

#define epsilon 1e-8

Mtx::Mtx(int n, double *dbp){
  nrows=n;
  ncols=n;
  nnz=0;
  

  ets = new double[nrows*ncols];
  index = new int[nrows*ncols];
  
  for(int i=0; i<nrows; i++)
    for(int j=0; j<ncols; j++)
      ets[j+nrows*i]=dbp[j+nrows*i];
}

Mtx::Mtx(int nrows, double a){
  this->nrows=nrows;
  this->ncols=ncols;

  nnz=0;

  ets = new double[nrows*ncols];
  index = new int[nrows*ncols];
  
  for (int i=0; i<nrows; i++)
    for (int j=0; j<ncols; j++)
      ets[j+ncols*i]=a;
}
  

Mtx::Mtx(int nrows, int ncols, int ngleft, int ngright, double a){
  this->nrows=nrows;
  this->ncols=ncols;
  gleft_offset=0;
  gright_offset=ngleft;

  nnz=0;

  ets = new double[nrows*ncols];
  index = new int[nrows*ncols];
  
  for (int i=0; i<nrows; i++)
    for (int j=0; j<ncols; j++)
      ets[j+ncols*i]=a;
}


 
Mtx::Mtx(const Mtx &mat){
  nrows = mat.nrows;
  ncols = mat.ncols;
  nnz=0;

  ets = new double[nrows*ncols];
  index = new int[nrows*ncols];
  
  for (int i=0; i<nrows; i++)
    for (int j=0; j< ncols; j++)
      ets[j+ncols*i] = mat.ets[j+ncols*i];
}

inline Mtx::~Mtx(){
  delete[] ets;
  delete[] index;
}

Mtx & Mtx::operator=(const Mtx& mat){
  if(this!= &mat){
    if(nrows !=mat.nrows ||ncols != mat.ncols)
      error("bad matrix sizes");
    for(int i=0; i< nrows; i++)
      for(int j=0; j<ncols; j++)
	ets[j+ncols*i]=mat.ets[j+ncols*i];
  }
  return *this;
};

Mtx& Mtx::operator+=(const Mtx& mat){
    if(nrows !=mat.nrows || ncols !=mat.ncols)
      error("bad matrix sizes");
  
    for(int i=0; i<nrows; i++)
      for(int j=0; j<ncols; j++)
	ets[j+ncols*i] += mat.ets[j+ncols*i];
  return *this;
}

Mtx& Mtx::operator-=(const Mtx& mat){
  //  if(nrows != mat.nrows || ncols !=mat.ncols)
  //    error("bad matrix sizes");

  for(int i=0; i<nrows; i++)
    for(int j=0; j<ncols; j++)
      ets[j+ncols*i] -= mat.ets[j+ncols*i];
  return *this;
}


void Mtx::assign(int i, int j,const double c){
  if(outofbounds(j)==0){
    //    printf("%d\t%d\t%d\t%lf\n",i,j, ncols*i+j, c);
    ets[ncols*i+j]=c;
     }
  
}

int Mtx::outofbounds(int i){
  if(i<0||i>ncols-1)
    return 1;
  else
    return 0;
}
 
Vector Mtx::operator*(const Vector& v) const {
  //  if(ncols  != v.size())
  //error("matrix and vector sizes do not match");
  Vector tm(nrows);
  for (int i=0; i<nrows; i++)
    for(int j=0; j<ncols; j++)
      tm[i] += ets[j+ncols*i]*v[j];
  return tm;
}


Mtx& Mtx::operator+(){
  return *this;
}

Mtx operator-(const Mtx& mat){
  return Mtx(mat.nrows) -mat;
}

Mtx Mtx::operator+(const Mtx & mat) const{
  if(nrows !=mat.nrows || ncols != mat.ncols)
    error("bad matrix sizes");
  Mtx sum = *this;
  sum += mat;
  return sum;
}

Mtx operator-(const Mtx& m1, const Mtx& m2){
  if(m1.nrows !=m2.nrows || m1.ncols !=m2.ncols)
    error("bad matrix sizes");
  
  Mtx sum = m1;
  sum -= m2;
  return sum;
}

Vector::Vector(int n, double *v){
  vec = new double[length = n];
  for (int i = 0; i < length; i++) vec[i] = v[i];
}

Vector::Vector(int n, double a){
  vec = new double[length = n];
  for(int i = 0; i < n; i++)
    vec[i] = a;
}

Vector::Vector(const Vector &v){
  vec = new double [length = v.length];
  for(int i = 0; i < length; i++) vec[i] = v[i];
}  

Vector& Vector::operator=(const Vector &v){
    if(this !=&v){
      if(length !=v.length) {
	error("bad vector sizes");
      }

	for(int i=0; i<length; i++)
	  vec[i] = v[i];
    }
    return *this;
  }


Vector& Vector::operator-=(const Vector &v){
  if(length != v.length) error("bad vector sizes");
  for (int i=0; i<length; i++) vec[i] -= v[i];
  return *this;
}



ostream& operator<<(ostream &out, const Mtx& mat){
  for (int i=0; i<mat.nrows; i++)
    for (int j=0; j<mat.ncols; j++){
      out  << " " << mat(i,j) << " ";
      if((i*mat.ncols+j)%mat.ncols==mat.ncols-1) out  << "\n";
    }
    return out;
    }


ostream& operator<<(ostream& s, const Vector& v){
  for (int i=0; i< v.length; i++){
    s << v[i] << " ";
    if (i%10 == 9) s << "\n";
  }

  return s;
}

  

int Mtx::get_nnz(){
  nnz=size()*col_size();
  //  cout << "size="<< size() << endl;
  for(int i=0; i<size(); i++)
    for(int j=0; j<col_size(); j++)
      if(fabs(ets[col_size()*i+j])<epsilon)
	nnz--;
  
  //  cout << "nnz=" << nnz << endl;
  return nnz;
}


void Mtx::get_csr_matrix(double *data, int *col_indices , int *row_indices){

  row_indices[0]=0;
  row_indices[size()]=nnz; //last entry

  int i,j;
  int ind_cols=0;

  for(i=0; i<size(); i++)
    for(j=0; j<col_size(); j++)
      if(fabs(ets[i*col_size()+j])<epsilon)
	continue;
      else{
	col_indices[ind_cols]=j;
	data[ind_cols]=ets[i*col_size()+j];
	index[i*col_size()+j]=ind_cols;
	ind_cols++;

	
      }
  

    for(i=0; i<size(); i++)
    for (j=0; j<col_size(); j++)
      if(fabs(ets[i*col_size()+j])>epsilon){
	row_indices[i]=index[i*col_size()+j];
	break;
      }
    
}
 

/*
void Mtx::get_csr_matrix_serial(double *data, int *col_indices , int *row_indices){

  row_indices[0]=0;
  row_indices[size()]=nnz; //last entry

  int i,j;
  int ind_cols=0;

  for(i=0; i<size(); i++)
    for(j=0; j<size(); j++)
      if(fabs(ets[i*size()+j])<epsilon)
	continue;
      else{
	col_indices[ind_cols]=j;
	data[ind_cols]=ets[i*size()+j];
	index[i*size()+j]=ind_cols;
	ind_cols++;

	
      }
  

    for(i=0; i<size(); i++)
    for (j=0; j<size(); j++)
      if(fabs(ets[i*size()+j])>epsilon){
	row_indices[i]=index[i*size()+j];
	break;
      }
} 
*/      
  
      
      
  
