using namespace std;
#include "vector.hxx"
int main(){
  int m = 5;
  double *v = new double [m];
  cout << "test" << endl;
  for (int i =0; i<m; i++) v[i] = i*i + 10;



  Vector v1(m, v);

  cout << v1; 

  Vector v2(m);


  for (int i=0; i<m; i++)  v2[i] = 5*i - 384;
  Vector v3(m, 5.8);

  //  cout << "test" << endl;


  
  Vector v4 = -v1 + 3.3*v3;
  v4 += v2;

    
  //  cout << v4;

  
  Vector v5 = v1*v4;
  double a = dot(v1, v5);
  

}  
    
  
