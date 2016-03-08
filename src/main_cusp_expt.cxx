#include "platter.hxx"
#include <mpi.h>
#include <iostream>
#include <cusp/array2d.h>
#include <cusp/array1d.h>
#include <cusp/print.h>
#include <cusp/coo_matrix.h>
//#include <cusp/krylov/cg.h>


#define NX_MAX 16
#define NY_MAX 16
#define NPROCX  1
#define NPROCY  1



using namespace std;
int main(){
  MPI::Init();

  int size;
  int rank;

  size=MPI::COMM_WORLD.Get_size();
  rank=MPI::COMM_WORLD.Get_rank();

  

#ifdef DEBUG 

  cout << "My rank is " << rank << endl;

  if(rank == 0) 
    cout << "Size: " << size << endl;

#endif 
  
  Platter platter(NPROCX, NPROCY, NX_MAX, NY_MAX);
    platter.set_values();
    platter.construct_rhs(0,0,0,0,1);
    //    platter.print_values();
    platter.print_bcs();

    Mtx mat(NX_MAX*NY_MAX);
    
    //    cout << "Setting matrix values";
    platter.set_mtx_from_platter(mat);

    cout << mat;

    platter.print_rhs();
	
	
    cusp::array2d<double, cusp::host_memory> A(NX_MAX*NY_MAX, NX_MAX*NY_MAX);
    cusp::array2d<double, cusp::device_memory> A_d(NX_MAX*NY_MAX, NX_MAX*NY_MAX);

    //        cusp::array2d<double, cusp::device_memory> A_d(NX_MAX*NY_MAX, NX_MAX*NY_MAX);
    //    cusp::hyb_matrix<int, double, cusp::device_memory> A_d;

    //    cusp::coo_matrix<int, double, cusp::device_memory> coo(A); 

	

    
        platter.copy_matrix_to_cusp(A);

	//       cusp::coo_matrix<int, double, cusp::host_memory>coo(A); 

	//              A_d=A;
       //       cusp::coo_matrix<int, double, cusp::device_memory>coo_d(A_d); 

       
       

	

	cusp::print(A);


	cusp::array1d<double, cusp::host_memory> b(NX_MAX*NY_MAX);
	//	cusp::array1d<double, cusp::device_memory> b_d(NX_MAX*NY_MAX);
	
	platter.copy_rhs_to_cusp(b);
	
	cusp::print(b);

	//	b_d = b;

	//	cusp::array1d<double, cusp::host_memory> x(NX_MAX*NY_MAX,0);
	//	cusp::array1d<double, cusp::device_memory> x_d(NX_MAX*NY_MAX,0);

	//	cusp::krylov::cg(A_d, x_d, b_d);
    
	//	x=x_d;

	//	cusp::print(x);

	
	
	    // create a simple example                                                                                                                                  

  MPI::Finalize();
}


  


  

  
