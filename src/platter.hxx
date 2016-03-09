#ifndef PLATTER_H
#define PLATTER_H
#include <stdio.h>
#include "matrix.hxx"
#include <cusp/array2d.h>
#include <cusp/array1d.h>
#include <cusp/coo_matrix.h>
#include <cusp/print.h>
class Platter
{
  double *u;
  double *v;
  double *x;
  double *y;
  double h_x;
  double h_y;
  double L;
  double L_domain; 
  
  double *top;
  double *bottom;
  double *left;
  double *right;
  double *center;
  double *f;

  double *f_left, *f_right;

  bool *boundary;
  int *gleft, *gright;
  int ngleft, ngright;

  int *gindex_left;
  int *gindex_right;
  
  
  int nx, ny, np;
  int nx_max, ny_max;

  
public:

  Platter(int nproc_x, int nproc_y, int nx_max, int ny_max, double L_dummy=1.0){

    nx = nx_max/nproc_x;
    ny = ny_max/nproc_y;
    np = nx*ny;
    L = L_dummy;
    L_domain = L_dummy*nproc_x;
    this->nx_max = nx_max;
    this->ny_max = ny_max;
    
    h_x = L/(nx-1);
    h_y = L/(ny-1);

    //allocate variables
    u = new double[np];
    v = new double[np];

    left = new double[np];
    right = new double[np];
    top = new double[np];
    bottom = new double[np];
    center = new double[np];
    f = new double[np]; //rhs

    f_left = new double[ny]; //ghost rhs
    f_right = new double[ny];//ghost rhs

    gleft = new int[np];
    gright = new int[np];
		  
    ngleft = 0;
    ngright = 0;

    gindex_left = new int[np];
    gindex_right = new int[np];

    x = new double[np];
    y = new double[np];

  };
  
  ~Platter(){
    delete[] u;
    delete[] v;
    delete[] left;
    delete[] right;
    delete[] top;
    delete[] bottom;
    delete[] center;
    delete[] x;
    delete[] y;
    delete[] f;
    delete[] gleft;
    delete[] gright;
    delete[] gindex_left;
    delete[] gindex_right;
    delete[] f_left;
    delete[] f_right;
  }

  void set_values();
  void set_stencil();
  void print_values() const;
  void print_bcs();
  void print_rhs() const;
  void construct_rhs(double, double, double, double, double);
  void number_ghost_cells();
  
  Mtx& set_mtx_from_platter(Mtx& mat) const;
  Mtx& set_ghost_cells_in_mtx(Mtx& mat) const;
  
  void copy_matrix_to_cusp(cusp::array2d<double, cusp::host_memory> &) const;
  void copy_rhs_to_cusp(cusp::array1d<double, cusp::host_memory> &) const;
  void copy_rhs_to_array(double *) const;

  void assign(int, int, double, cusp::array2d<double, cusp::host_memory> &) const;
  int outofbounds(int) const;
  
};
#endif







  
  
  
  
  
  
  
