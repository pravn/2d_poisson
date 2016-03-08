#include "vector.hxx"
inline void error(char *v){
  cout << v << endl << "Program exited\n";
  exit(1);
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
Vector& Vector::operator+=(const Vector &v){
  if(length != v.length) error ("bad vector sizes");
  for (int i = 0; i <length; i++) vec[i] += v[i];
  return *this;
}

Vector& Vector::operator-=(const Vector &v){
  if(length != v.length) error("bad vector sizes");
  for (int i=0; i<length; i++) vec[i] -= v[i];
  return *this;
}

double Vector::twonorm() const{
  double norm = vec[0] * vec[0];
  for (int i=1; i< length; i++) norm += vec[i] * vec[i];
  return norm;
}

double Vector::maxnorm() const{
  double norm = fabs(vec[0]);
  for(int i =1; i < length; i++)
    norm = max(norm, fabs(vec[i]));
  return norm;
}

inline Vector operator+(const Vector &v){ //usage u = +v
  return v; //unary +
}

inline Vector operator-(const Vector &v){ //usage u = -v
  return Vector(v.length) - v; //unary -
}

Vector operator+(const Vector &v1, const Vector &v2){ //binary addition
  if(v1.length != v2.length) error("bad vector sizes");
  Vector sum = v1;
  sum += v2; 
  return sum;
}

Vector operator-(const Vector &v1, const Vector &v2){
  if(v1.length != v2.length) error("bad vector sizes");
  Vector sum = v1; 
  sum -= v2;
  return sum;
}

Vector operator*(double scalar, const Vector &v){
  Vector tm(v.length); 
  for (int i=0; i <v.length; i++) tm[i] = scalar*v[i];
  return tm;
}

inline Vector operator*(const Vector& v, double scalar){
  return scalar*v;
}

Vector operator*(const Vector &v1, const Vector &v2){
  if(v1.length !=v2.length) error("bad vector sizes");
  int n = v1.length;
  Vector tm(n);
  for (int i = 0; i <n; i++) tm[i] = v1[i]*v2 [i];
  return tm;
}

double dot(const Vector &v1, const Vector &v2){
  if(v1.length != v2.length) error("bad vector sizes");
  double tm = v1[0] * v2[0];
  for (int i=1; i <v1.length; i++) tm += v1[i]*v2[i];
  return tm;
}

ostream& operator<<(ostream& s, const Vector& v){
  for (int i=0; i< v.length; i++){
    s << v[i] << " ";
    if (i%10 == 9) s << "\n";
  }

  return s;
}


  

  
