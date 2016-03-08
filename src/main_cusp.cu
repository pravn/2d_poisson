#include "platter.hxx"
#include <mpi.h>
#include <iostream>
#include <cusp/array2d.h>
#include <cusp/array1d.h>
#include <cusp/print.h>
#include <cusp/coo_matrix.h>
#include <cusp/krylov/cg.h>
#include <cusp/multiply.h>
#include <cusp/blas.h>


using namespace std;
int main(){
  MPI::Init();

int size;
int rank;
 int NX_MAX, NY_MAX;
ifstream inputs;

size=MPI::COMM_WORLD.Get_size();
rank=MPI::COMM_WORLD.Get_rank();

 cout << "Number of ranks = " << size << endl;
 cout << "My rank is = " << rank << endl;
 
inputs.open("inputs.dat");

inputs >> NX_MAX;
inputs >> NY_MAX;

inputs.close();


  Platter platter(1, 1, NX_MAX, NY_MAX); //first two args are NPROCX=NPROCY=1
    platter.set_values();
    platter.construct_rhs(0,0,0,0,1);
    //    platter.print_bcs();

    Mtx mat(NX_MAX*NY_MAX);
    
    platter.set_mtx_from_platter(mat);

    //    cout << mat;

    //    platter.print_rhs();
	
	
    cusp::array2d<double, cusp::host_memory> A(NX_MAX*NY_MAX, NX_MAX*NY_MAX);
    
    platter.copy_matrix_to_cusp(A);

    cusp::coo_matrix<int, double, cusp::device_memory>A_d(A); 

    //    cusp::print(A);


    cusp::array1d<double, cusp::host_memory> b(NX_MAX*NY_MAX);
    cusp::array1d<double, cusp::device_memory> b_d(NX_MAX*NY_MAX);
	
    platter.copy_rhs_to_cusp(b);
	
    b_d = b;

    cusp::array1d<double, cusp::host_memory> x(NX_MAX*NY_MAX,0);
    cusp::array1d<double, cusp::device_memory> x_d(NX_MAX*NY_MAX,0);

    cusp::krylov::cg(A_d, x_d, b_d);
    x=x_d;

    cusp::print(x_d);

	
    cusp::array1d<double, cusp::device_memory> y(NX_MAX*NY_MAX);
    cusp::multiply(A_d, x_d, y);
    cusp::blas::axpy(b_d,y,-1);

    //    cusp::print(y);
	

  MPI::Finalize();
}

  

  
