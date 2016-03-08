#include "platter.hxx"
#include <mpi.h>
#include <stdio.h>
#include <math.h>
 
#define epsilon 1e-10


void Platter::print_values() const{
  for(int i=0; i<np; i++){
         printf("%d\t%d\n", gindex_left[i], gindex_right[i]);

  }
}

void Platter::print_rhs() const{
  printf("\n");
  for(int i=0; i<np; i++)
    printf("%d\t%lf\n",i,f[i]);
}

void Platter::print_bcs(){
  for (int i = 0; i < np; i++){
    //    if(boundary[i] == true)
    //      printf("x \t y, %lf\t %lf\n", x[i], y[i]);
  }
}

void Platter::set_values(){
  int rank = MPI::COMM_WORLD.Get_rank();

  printf("my rank is %d\n", rank);

  for (int i = 0; i < np; i++){
    u[i] = 0;
    v[i] = 0;
    x[i] = i%(nx)/double(nx_max-1) + rank*double(nx)/double(nx_max-1);
    y[i] = (i/(nx))/double(ny-1);

    left[i] = -1;
    right[i] = -1;
    top[i] = -1;
    bottom[i] = -1;
    center[i]=4;

    gleft[i] = 0;
    gright[i] = 0;

    gindex_left[i] = 0;
    gindex_right[i] = 0;
    


    if(i%nx==nx-1)
      gright[i]=1;
    if(i%nx==0)
      gleft[i]=1;
    if(x[i]==0)
      gleft[i]=0;
    if(fabs(x[i]-1.0)<epsilon)
      gright[i]=0;

    if((x[i]==0))
      left[i]=0;
    if((fabs(x[i]-1.0)<epsilon))
      right[i]=0;
    if((y[i]==0))
      bottom[i]=0;
    if((fabs(y[i]-1.0)<epsilon))
      top[i]=0;

    if(gleft[i]==1) 
      left[i]=0;
    if(gright[i]==1)
      right[i]=0;

    f[i] = 0;
    
  }

  for(int i=0; i<ny; i++){
    f_left[i] = 0;
    f_right[i] = 0;
  }
    

}

void Platter::number_ghost_cells()
{
  for (int i=0; i<np; i++)
    if(gleft[i]==1)
      {
	++ngleft;
	gindex_left[i] = ngleft;
      }


  for (int i=0; i<np; i++)
    if(gright[i]==1){
      ++ngright;
      gindex_right[i] = ngright;
    }
    
  }


void Platter::construct_rhs(double s=0,double bc_l=0, double bc_r=0, double bc_b=0, double bc_t=0){
  int rank = MPI::COMM_WORLD.Get_rank();
  int size = MPI::COMM_WORLD.Get_size();
  for (int i=0; i<np; i++){
    f[i] = s;
    if(x[i]==0)
      f[i] += bc_l;
    if(fabs(x[i]-1)<epsilon)
      f[i] += bc_r;
    if(y[i]==0)
      f[i] += bc_b;
    if(fabs(y[i]-1.0)<epsilon)
      f[i] += bc_t;
  }
  for(int i=0; i<ny; i++){
    f_left[i] = s;
    f_right[i] = s;

    if(y[i*nx]==0){
      f_left[i] += bc_b;
      f_right[i] += bc_b;
    }
    
    if(fabs(y[i*nx]-1.0)<epsilon){
      f_left[i] += bc_t;
      f_right[i] += bc_t;
    }
    
    if(rank==0)
      f_left[i] =0;
    if(rank==size-1)
      f_right[i] =0;
  }
}
  

Mtx& Platter::set_mtx_from_platter(Mtx& mat) const{
  int nrows = mat.size();
  cout << "size="<<mat.size()<<endl;
  cout << "np=" << np << endl;
  for(int i=0; i<mat.size(); i++){
     if((gleft[i]==0)&&(gright[i]==0)){
      mat.assign(i,i,center[i]);
      mat.assign(i,i-1,left[i]);
      mat.assign(i,i+1,right[i]);
      mat.assign(i,i-nx,bottom[i]);
      mat.assign(i,i+nx,top[i]);
        }
          if(gleft[i]==1){
      mat.assign(i,i,center[i]);
      mat.assign(i,i+1,right[i]);
      mat.assign(i,i-nx,bottom[i]);
      mat.assign(i,i+nx,top[i]);
    }
    if(gright[i]==1){
      mat.assign(i,i,center[i]);
      mat.assign(i,i-1,left[i]);
      mat.assign(i,i-nx,bottom[i]);
      mat.assign(i,i+nx,top[i]);
    }
     
  }

}

 Mtx& Platter::set_ghost_cells_in_mtx(Mtx& mat) const{
   int nrows = mat.size();
   
   for(int i=0; i<nrows; i++){
     //fill left ghosts
     if(gindex_left[i]!=0)
       mat.assign(i,nrows+mat.gl_offset()+gindex_left[i]-1,-1);
	  
     //fill right ghosts
     if(gindex_right[i]!=0)
       mat.assign(i,nrows+mat.gr_offset()+gindex_right[i]-1,-1);
     
   }
 }   
 
 

void Platter::assign(int i, int j, double c, cusp::array2d<double, cusp::host_memory>& A) const{
  if(outofbounds(j)==0)
    A(i,j)=c;
}

int Platter::outofbounds(int i) const{
  if(i<0||i>np-1)
    return 1;
  else 
    return 0;
}

void  Platter::copy_matrix_to_cusp(cusp::array2d<double, cusp::host_memory>& A) const{
        for (int i=0; i<np; i++){
	  assign(i,i,center[i],A);
	  assign(i,i-1,left[i],A);
	  assign(i,i+1,right[i],A);
	  assign(i,i-nx,bottom[i],A);
	  assign(i,i+nx,top[i],A);

      }
  }

void Platter::copy_rhs_to_cusp(cusp::array1d<double, cusp::host_memory>& b) const{
  for(int i=0; i<np; i++)
    b[i]=f[i];
}

void Platter::copy_rhs_to_array(double* vec_data) const{
  int rank = MPI::COMM_WORLD.Get_rank();
  int size = MPI::COMM_WORLD.Get_size();

  for(int i=0; i<np; i++)
    vec_data[i]=f[i];
}
  
