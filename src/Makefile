MPI_BIN = /usr/bin/mpicxx
MPI_INC = /usr/include/openmpi
MPI_LIB = /usr/lib64/openmpi/lib 
RM =rm -f
MATVEC_DIR=./matvec_methods/
#CUDA_HOME=/usr/local/cuda
CUSP=/usr/local/include/cusplibrary
CUDA_INC=/usr/local/cuda/include
INC_PATH=-I${MATVEC_DIR} -I${CUDA_INC} -I${CUSP}
CUDA_LIBDIR= /usr/local/cuda/lib64
NVCC=nvcc -arch=sm_35

OBJS_CUSP= poisson_cusp.o methods.o matrix_init.o 

ifeq (${DEBUG},1)
	FLAGS += -DDEBUG=1
endif

default: poisson_cusp

poisson_cusp: ${OBJS_CUSP} 
	${MPI_BIN} -o $@ $^ -L${MPI_LIB} -lmpi ${FLAGS} ${INC_PATH} \
	-L${CUDA_LIBDIR} -lcudart	

poisson_cusp.o: main_cusp.cu
	${NVCC} -c $^ -o $@ -I${MPI_INC} ${FLAGS} ${INC_PATH}

methods.o : methods.cxx
	${MPI_BIN} -c $^ -o $@ -I${MPI_INC} ${FLAGS} ${INC_PATH}

matrix_init.o: ${MATVEC_DIR}/matrix_init.cxx
	${MPI_BIN} -c $^ -o $@ -I${MPI_INC} ${FLAGS} ${INC_PATH}

#print.o: print.cu
#	${NVCC} -c $^ -o $@ -I${MPI_INC} ${FLAGS} ${INC_PATH} 

clean: 
	${RM} *.o poisson_cusp
