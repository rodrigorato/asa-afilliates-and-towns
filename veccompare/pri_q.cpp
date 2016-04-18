#include <iostream>
#include <stdio.h>
#include <queue>
#include <vector>

using namespace std;

class vec_compare{
  const vector<int>* _somenumbers;
public:
 	vec_compare(const vector<int>* somenumbers){
 	  _somenumbers = somenumbers;
  }
  
  bool operator() (const int& lhs, const int&rhs) const {
    return (*_somenumbers)[lhs] < (*_somenumbers)[rhs];
  }
};

int main (){
  int howmany = 5;

  /* posicoes pares sao 1, impares sao 0 */
  vector<int> myints = vector<int>(howmany);
  for(int i = 0; i < howmany; i++){
    myints[i] = i;
  }

	typedef std::priority_queue<int,std::vector<int>,vec_compare> Queue;
  
  vec_compare someName = vec_compare(&myints);
  
  Queue priQueue(someName);

  /* preencher a fila com nrs de 0 a howmany */
  for(int i = 0; i < howmany; i++){
    priQueue.push(i);
  }

  /* esvaziar a fila */
  for(int i = 0; i < howmany; i++){
    cout << priQueue.top() << " ";
    priQueue.pop();
  }

  cout << endl;
  return 0;

}