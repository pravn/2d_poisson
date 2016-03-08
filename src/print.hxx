#ifndef PRINT_H
#define PRINT_H
#include <string>
#include <iostream>
#include <string>
#include <iostream>

template <typename T>
void print(const T* a, int length, const std::string mess="");
template <typename T> 
void print(const T a, const std:: string mess="");

template <typename T>
void print(const T* a,int length, const std::string mess)
{
  std::cout << std::endl;
  std::cout << mess << std::endl;
  for(int i=0; i<length; i++){
    std::cout << a[i] << "  ";
    if(i%10==9)
      std::cout << std::endl;
  }
  std::cout << std::endl;
}

template <typename T>
void print(const T a,const std::string mess){
  std::cout << std::endl << std::endl;
  std::cout << mess << std::endl;
  std::cout << a << std::endl;
}

#endif
