#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <cmath>

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
  		return greater_infinity((*_dm)[lhs], (*_dm)[rhs]);
    }
};

class Heap{
public:
	int size;
	vector<int> pos;
	vector<int> q;
	vector<int>& d;

	Heap(int s, int act, vector<int>& dists): d(dists){
		size = s;
		pos = vector<int>(size);
		q = vector<int>(size);

		for(int i = 0; i < size; i++){
			pos[i] = i;
			q[i] = i;
		}

		q[0] = act;
		q[act] = 0;

		pos[0] = act;
		pos[act] = 0;
	}

	int pop(){
		int min = q[0];
		q[0] = q[size-1];
		pos[q[0]] = 0;
		size--;
		min_heapify(0);
		return min;
	}

	void min_heapify(int ind){
		int l = 2*ind+1, r = 2*ind+2, smallest = ind;
		if(less_infinity(l, size) && greater_infinity(d[q[ind]], d[q[l]])){
			smallest = l;
		} 
		if(less_infinity(r, size) && greater_infinity(d[q[smallest]], d[q[r]])){
			smallest = r;
		} 
		if(smallest != ind){
			swap(pos[q[ind]], pos[q[smallest]]);
			swap(q[ind], q[smallest]);
			min_heapify(smallest);
		}
	}
	void decreaseKey(int ind){
			/*
			A[i] ← key
			2 while i > 1 and A[Parent(i)] < A[i]
			3 do exchange A[i] ↔ A[Parent(i)]
			4 i ← Parent(i)
			*/
			ind = pos[ind];

			while(ind > 0 && greater_infinity(d[q[(int)floor((ind - 1)/2)]], d[q[ind]])){
				swap(q[ind], q[(int)floor((ind - 1)/2)]);
				swap(pos[q[ind]], pos[q[(int)floor((ind - 1)/2)]]);
				ind = (int)floor((ind - 1)/2);
			}
		}

};

class Edge{
public:
	int v;
	int w;

	Edge(int vert_dest, int weight){
		v = vert_dest;
		w = weight;
	}
};


class DirectedGraph{
public:
	int _nverts;
	int _nedges; // Maybe we don't need this (used only to get input?)
	int _nafls;
	int* _afls; // Array with all the identifiers for afilliate points
	int _solution;
	list<Edge>* _adjLists;
	bool _trans;
	
	/* Aux variables for Bellman-Ford */
	vector<int> _h;
	bool _bfChanges;

	/* Aux variables for Dijkstra */
	//vector<vector<int> > _dijkstraMaster;
	vector<int> _dijkstraMaster;
	vector<int> _dijkstraN;
	vector<int> _sumsMaster;

	/* Aux variables for Johnson */


	DirectedGraph(int nverts, int nedges, int nafls, int* afls){
		/* Class constructor */
		_nverts = nverts;
		_nedges = nedges;
		_nafls = nafls;
		_afls = afls;
		_trans = false;

		_adjLists = new list<Edge>[nverts];
		_h = vector<int>(_nverts, 0);
		_bfChanges = true;
		_dijkstraN = vector<int>(_nverts, 0);
		_sumsMaster = vector<int>(_nverts,0);
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
		bool changed = true;
		while(changed){
			changed=false;
			for(int i = 0; i < _nverts; i++){
				list<Edge>::iterator e;
				for(e = _adjLists[i].begin(); e != _adjLists[i].end(); e++){
					if((_h[i] + e->w) < _h[e->v]){
						_h[e->v] = (_h[i] + e->w);
						changed = true;
						//BellmanFord();
					}
				}
			}
		}
	}

	int newWeight(int u, Edge e){
		return e.w + _h[u] - _h[e.v];
	}


	void Dijkstra(int act){
		int min_index, min;
		_dijkstraMaster = vector<int>(_nverts, INFINITE);
		_dijkstraMaster[act] = 0;
		Heap q = Heap(_nverts, act, _dijkstraMaster);
		//vec_compare comparator = vec_compare(&_dijkstraMaster);


		/* Iterating over all the vertices */
		while(q.size != 0){
			/* Extract the min from the min-heap */
			min_index = q.pop();
			min = _dijkstraMaster[min_index];
			/*
			pop_heap(q.begin(),q.end(),comparator);
			q.pop_back();
			*/

			//cout << "min_index=" << min_index+1 << " min=" << min << endl;

			/* Iterating over all the edges of the vert and reweight adjacencies on the way */
			if(_dijkstraMaster[min_index] != INFINITE){
				list<Edge>::iterator e;
				for(e = _adjLists[min_index].begin(); e != _adjLists[min_index].end(); e++){
					if(less_infinity((min + newWeight(min_index, *e)), _dijkstraMaster[e->v])/* || _dijkstraMaster[act][e->v] == INFINITE */){
						_dijkstraMaster[e->v] = (min + newWeight(min_index, *e));
						//make_heap(q.begin(),q.end(),comparator);
						q.decreaseKey(e->v);
					}
				}
			}
			else 
				_dijkstraN[min_index] = INFINITE;
			//cout << act+1<<","<<min_index+1<<":"<<sum_infinity(_dijkstraMaster[min_index], (_h[min_index] - _h[act])) << endl;
			_sumsMaster[min_index]= sum_infinity(_sumsMaster[min_index], sum_infinity(_dijkstraMaster[min_index], (_h[min_index] - _h[act])));
			/* Translating weight values from non-negative to normal */
			//_sumsMaster[min_index].push_back(sum_infinity(_dijkstraMaster[min_index], (_h[min_index] - _h[act])));
		}
	}
	
	
	int Johnson(){
		int min = INFINITE, cand, pt = INFINITE;
		bool solvable = false;

		BellmanFord();
		if (_trans)
			Dijkstra(_solution);
		else
			for(int i = 0; i < _nafls; i++)
				Dijkstra(_afls[i]);


		for(int i = 0; i < _nverts; i++){
			//cand = 0;

			if(_dijkstraN[i] != INFINITE)
				solvable = true;
			/*
			list<int>::iterator s;
			for(s = _sumsMaster[i].begin(), j=0; s != _sumsMaster[i].end(); s++, j++)
				cand = sum_infinity(cand,*s);
			*/
			cand=_sumsMaster[i];
			if(less_infinity(cand, min)){
				min = cand;
				pt = i;
			} 
			
			if(cand == min)
				pt = less_infinity(i, pt) ? i : pt;	
		}

		if(solvable){
			if(!_trans){
				printf("%d %d\n", pt+1, min);
				return pt;
			}
			/*
			list<int>::iterator s;
			for(s = _sumsMaster[pt].begin(); s != _sumsMaster[pt].end(); s++)
				printf("%d ",*s);
			*/
			if(_trans){
				for(int i = 0; i < _nafls; i++){
					printf("%d ", _sumsMaster[_afls[i]]);
				}
				printf("\n");
			}
		}	
		else
			printf("N\n");
		return -1;
	}

	void trans(int afl){
		list<Edge>* newAdjList = new list<Edge>[_nverts];
		for(int i = 0; i < _nverts; i++){
			while(!_adjLists[i].empty()){
				Edge t = _adjLists[i].front(); _adjLists[i].pop_front();
				int u = t.v;
				t.v = i;
				newAdjList[u].push_back(t);			
			}
		}
		delete[] _adjLists; _adjLists = newAdjList;
		_h = vector<int>(_nverts, 0);
		_trans = true;
		_sumsMaster = vector<int>(_nverts,0);
		_dijkstraN = vector<int>(_nverts, 0);
		_solution=afl;
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
			printf("\n%d (h = %d) : ", i+1, _h[i]);
			for(int j = 0; j < (int)_adjLists[i].size(); j++){
				std::list<Edge>::iterator it = std::next(_adjLists[i].begin(), j);
				if(j == (int)_adjLists[i].size() - 1)
					printf("(%d, %d, %d)", (it->u)+1, (it->v)+1, it->w);
				else
					printf("(%d, %d, %d), ", (it->u)+1,  (it->v)+1, it->w);
			}
		}
		printf("\n");
	}*/
	
};


int main(){
	int n_towns, n_afls, n_conns;

	scanf("%d %d %d", &n_towns, &n_afls, &n_conns);

	int *afls = new int[n_afls]; 
	int sol;
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

	sol = g.Johnson();
	g.trans(sol);
	if(sol!=-1){
		//g.BellmanFord();
		g.Johnson();
		//g.printGraph();
	}
	//t.printGraph();

	// TRANSLATE INDEXES BACK FROM n-1 to n!!
	return 0;
}
