CC= g++
FLAGS= 
RM = rm -f 

#OBJS: matrix_driver.o  matrix_init.o

default: matrix_driver

matrix_driver: matrix_driver.o matrix_init.o
	${CC} -o $@ $^ ${FLAGS} 

matrix_driver.o: matrix_driver.cxx matrix_init.o
	${CC} -c $< ${FLAGS}

matrix_init.o: matrix_init.cxx
	${CC} -c $< ${FLAGS}

clean:
	${RM} *.o matrix_driver