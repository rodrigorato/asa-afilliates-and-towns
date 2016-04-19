#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits.h>

#define INFINITE -782973

using namespace std;

bool less_or_equal_infinity(int a, int b){
	if(b == INFINITE)
		return true;
	if(a == INFINITE)
		return false;
	return a <= b;
} 

bool less_infinity(int a, int b){
	if(a == INFINITE)
		return false;
	if(b == INFINITE)
		return true;
	return a < b;
} 
int sum_infinity(int a, int b){
	return (a == INFINITE || b == INFINITE) ? INFINITE : a+b;
}
bool greater_infinity(int a, int b){
	if(b == INFINITE)
		return false;
	if(a == INFINITE)
		return true;
	return a>b;
} 

class vec_compare{
  	const vector<int>* _dm;
public:
 	vec_compare(const vector<int>* somenumbers){
 	  _dm = somenumbers;
  	}
  
  	bool operator() (const int& lhs, const int&rhs) const {
  		// true: lhs < rhs
  		//cout << lhs+1 << " : " << (*_dm)[lhs] << " < " << rhs+1 << ": " << (*_dm)[rhs]<<endl;
  		return greater_infinity((*_dm)[lhs], (*_dm)[rhs]);
    }
};

typedef std::priority_queue<int,std::vector<int>,vec_compare> Queue;

class Edge{
public:
	int v;
	int w;

	Edge(int vert, int weight){
		v = vert;
		w = weight;
	}
};


class DirectedGraph{
private:
	int _nverts;
	int _nedges; // Maybe we don't need this (used only to get input?)
	int _nafls;
	int* _afls; // Array with all the identifiers for afilliate points
	list<Edge>* _adjLists;

	/* Aux variables for Bellman-Ford */
	vector<int> _h;
	bool _bfChanges;

	/* Aux variables for Dijkstra */
	//vector<vector<int> > _dijkstraMaster;
	vector<int> _dijkstraMaster;
	vector<int> _dijkstraN;
	vector<list<int> > _sumsMaster;

	/* Aux variables for Johnson */
	vector<int> _sums;

public:
	DirectedGraph(int nverts, int nedges, int nafls, int* afls){
		/* Class constructor */
		_nverts = nverts;
		_nedges = nedges;
		_nafls = nafls;
		_afls = afls;
		_adjLists = new list<Edge>[nverts];
		_h = vector<int>(_nverts, 0);
		_bfChanges = true;
		//_dijkstraMaster = vector< vector<int> >(_nafls, vector<int>(_nverts, INFINITE));
		_dijkstraN = vector<int>(_nverts, 0);
		_sumsMaster = vector< list<int> >(_nverts);
		_sums = vector<int>(_nafls, 0);
		
	}

	~DirectedGraph(){
		/* Class destructor */
		delete[] _adjLists;
		delete[] _afls;
	}

	void addEdge(int u, int v, int w){
		/**
		 * Adds an edge from u to v in our graph 
		 * Note that this is a directed graph so
		 * this edge will be directed from u to v. 
		 * The edge's weight will be w.
		 **/
		_adjLists[u].push_back(Edge(v, w));
	}

	void BellmanFord(){
		for(int i = 0; i < _nverts; i++){
			list<Edge>::iterator e;
			for(e = _adjLists[i].begin(); e != _adjLists[i].end(); e++){
				if((_h[i] + e->w) < _h[e->v]){
					_h[e->v] = (_h[i] + e->w);
					BellmanFord();
				}
			}
		}
	}

	int newWeight(int u, Edge e){
		return e.w + _h[u] - _h[e.v];
	}

	void Dijkstra(int act/*, int ind*/){
		vector<bool> visited = vector<bool>(_nverts, false);
		int min_index;
		_dijkstraMaster = vector<int>(_nverts,INFINITE);
		_dijkstraMaster[act] = 0;
		vec_compare comparator = vec_compare(&_dijkstraMaster);
	//	Queue priQueue(comparator);
		vector<int> q=vector<int>(_nverts);

		for(int i = 0; i < _nverts; i++){
			q[i]=i;
		}
		make_heap(q.begin(),q.end(),comparator);

		//cout << priQueue.top() <<endl ;

		while(q.size()){
			int min;

			min_index = q.front();
			min = _dijkstraMaster[min_index];
			

			//for (int i =0; i<_nverts;i++) cout << _dijkstraMaster[i] << endl;
			//cout << min_index+1 << ": "<<_dijkstraMaster[min_index] << endl;
			pop_heap(q.begin(),q.end(),comparator);
			q.pop_back();



			if(_dijkstraMaster[min_index]!=INFINITE){
				list<Edge>::iterator e;
				for(e = _adjLists[min_index].begin(); e != _adjLists[min_index].end(); e++){
					if(less_infinity((min + newWeight(min_index, *e)), _dijkstraMaster[e->v])/* || _dijkstraMaster[act][e->v] == INFINITE */){
						_dijkstraMaster[e->v] = (min + newWeight(min_index, *e));
						make_heap(q.begin(),q.end(),comparator);
					}
				}
			}
			else _dijkstraN[min_index] = INFINITE;

			_sumsMaster[min_index].push_back(sum_infinity(_dijkstraMaster[min_index], (_h[min_index] - _h[act])));
			
			//cout << "----------------"<<endl;	
		}
				
		
	}
	
	
	void Johnson(){
		int min = INFINITE, cand, pt = INFINITE, j ;
		vector<int> tempSums ;
		bool solution = false;
		BellmanFord();
		for(int i = 0; i < _nafls; i++){
			//printGraph();
			//printf("dijkstra\n");
			Dijkstra(_afls[i]/*, i*/);
			//cout << "done " << i << "out of " << _nafls << " dijkstras" << endl;
		}
		
		for(int i=0; i < _nverts ; i++){
			//printf("%d\n",_dijkstraN[i] );
			if(_dijkstraN[i]!=INFINITE){
				solution = true;
				break;
			}
		}
		if (solution){
			for(int i=0 ; i<_nverts ;i++){
				cand=0;
				tempSums = vector<int>(_nafls, 0);
				list<int>::iterator s;
				for(s = _sumsMaster[i].begin(), j=0; s != _sumsMaster[i].end(); s++, j++){
					cand= sum_infinity(cand,*s);
					tempSums[j] =*s;
				}
				//cout << i << ": " << cand << endl;
				if(less_infinity(cand, min)){
					min=cand;
					pt = i;
					_sums=tempSums;
				} 
				if(cand == min)
					pt = less_infinity(i, pt) ? i : pt;	
			}
			printf("%d %d\n",pt+1,min );
			for(int j=0; j< _nafls; j++){
				printf("%d ",_sums[j]);
			}
			printf("\n");
		}	
		else{
			printf("N\n");
		}	
	}


/*
	void printGraph(){
		printf("Directed graph with %d vertices, %d edges and %d afilliates:\n", _nverts, _nedges, _nafls);
		for(int i = 0; i < _nafls; i++){
			if(i == 0)
				printf("[%d, ", _afls[i]+1);
			else if(i == _nafls - 1)
				printf(" %d]\n", _afls[i]+1);
			else
				printf("%d, ", _afls[i]+1);
		}
		printf("\nEdges (orig: (dest, weight), ...):");
		for(int i = 0; i < _nverts; i++){
			printf("\n%d (h = %d _sums= %d) : ", i+1, _h[i],_sums[i] );
			for(int j = 0; j < (int)_adjLists[i].size(); j++){
				std::list<Edge>::iterator it = std::next(_adjLists[i].begin(), j);
				if(j == (int)_adjLists[i].size() - 1)
					printf("(%d, %d)", (it->v)+1, it->w);
				else
					printf("(%d, %d), ", (it->v)+1, it->w);
			}
		}
		printf("\n");
	}*/

};

int main(){
	int n_towns, n_afls, n_conns;

	scanf("%d %d %d", &n_towns, &n_afls, &n_conns);

	int *afls = new int[n_afls]; 
	for(int i = 0; i < n_afls; i++){
		scanf("%d", &(afls[i]));
		afls[i]--; // Translating indexes - internally we start at zero
	}
	DirectedGraph g = DirectedGraph(n_towns, n_conns, n_afls, afls);

	for(int i = 0; i < n_conns; i++){
		int u, v, w;
		scanf("%d %d %d", &u, &v, &w);
		g.addEdge(u-1, v-1, w); // Translating to internal index

	}

	g.Johnson();
	//g.printGraph();

	// TRANSLATE INDEXES BACK FROM n-1 to n!!
	return 0;
}
