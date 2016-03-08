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
#include <string>
#include "print.hxx"


#define NX_MAX 4
#define NY_MAX 2
#define NPROCX  2
#define NPROCY  1


using namespace std;

void print_int(const int *a,int length)
{
  printf("\n\n");
  for(int i=0; i<length; i++){
    cout << " " << a[i] << " ";
    if(i%10==9)
      cout << endl;
  }
  cout << endl;
}

int main(){
  MPI::Init();

  int size;
  int rank;


  int send_num_neighbors;
  int *send_neighbors=NULL;
  int *send_sizes=NULL;
  int **send_maps=NULL;

  int recv_num_neighbors;
  int *recv_neighbors=NULL;
  int *recv_sizes=NULL;
  int **recv_maps=NULL;


  size=MPI::COMM_WORLD.Get_size();
  rank=MPI::COMM_WORLD.Get_rank();

  

#ifdef DEBUG 

  cout << "My rank is " << rank << endl;

  if(rank == 0) 
    cout << "Size: " << size << endl;

#endif 
  
  Platter platter(NPROCX, NPROCY, NX_MAX, NY_MAX);
  platter.set_values();
  //  platter.print_values();
  platter.construct_rhs(0,0,0,0,1);
  //  platter.print_bcs();

  int nx = NX_MAX/NPROCX;
  int ny = NY_MAX/NPROCY;

  //One dimensional decomposition in x (for now)
  //One layer of ghost cells on either side of mpi sub-domain 
  //i.e. ny ghost cells on either side

  int ngleft=0;
  int ngright=0;
  

  if(size>1){
    ngleft=ny; 
    ngright=ny;
    if(rank==0)
      ngleft=0;
    if(rank==size-1)
      ngright=0;
  }
  
  
   Mtx mat(nx*ny,ngleft+ngright+nx*ny,ngleft,ngright);
   platter.set_mtx_from_platter(mat);

   platter.number_ghost_cells();
   platter.set_ghost_cells_in_mtx(mat);



    if(size>1){
      platter.create_send_maps(send_num_neighbors, send_neighbors, send_sizes, send_maps,0,ny);
      platter.create_recv_maps(recv_num_neighbors, recv_neighbors, recv_sizes, recv_maps,0,ny);
    }

    cout <<endl<< endl << "nnz="<<    mat.get_nnz() << endl;
    print<int>(mat.col_size(),"column size");

    struct CSR_matrix{
      double *data;
      int *col_indices; 
      int *row_ptrs;
      int nnz;
    }csr_mat;

    csr_mat.nnz = mat.get_nnz();
    csr_mat.data=new double[csr_mat.nnz];
    csr_mat.col_indices= new int[csr_mat.nnz];
    csr_mat.row_ptrs = new int[csr_mat.nnz+1];

    
    double *data = csr_mat.data;
    int *col_indices = csr_mat.col_indices;
    int *row_ptrs = csr_mat.row_ptrs;
    int nnz = csr_mat.nnz;
    
    mat.get_csr_matrix(data, col_indices, row_ptrs);

    
        double *vec_data = new double[mat.col_size()];
	platter.copy_rhs_to_array(vec_data);


    cout << mat;

    print(vec_data, mat.col_size(), "vec_data");
   
    
        delete[] vec_data;

    

    
  MPI::Finalize();
}

  

  
