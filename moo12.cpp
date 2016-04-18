#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits.h>

#define INFINITE -1


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

using namespace std;

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
	vector<vector<int> > _dijkstraMaster;
	vector<int> _dijkstraN;

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
		_dijkstraMaster = vector< vector<int> >(_nafls, vector<int>(_nverts, INFINITE));
		_dijkstraN = vector<int>(_nverts, 0);
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

	void Dijkstra(int act, int ind){
		vector<bool> visited = vector<bool>(_nverts, false);
		_dijkstraMaster[ind][act] = 0;

		for(int i = 0; i < _nverts; i++){
			int min=INFINITE, min_index = -1;
			for(int j = 0; j < _nverts; j++){
				
				if((less_or_equal_infinity(_dijkstraMaster[ind][j], min) && !visited[j])/* && _dijkstraMaster[act][j] != INFINITE */){
					min = _dijkstraMaster[ind][j];
					
					min_index = j;
				}
			} 
			if(_dijkstraMaster[ind][min_index]!=INFINITE){
				list<Edge>::iterator e;
				for(e = _adjLists[min_index].begin(); e != _adjLists[min_index].end(); e++){
					if(less_infinity((min + newWeight(min_index, *e)), _dijkstraMaster[ind][e->v])/* || _dijkstraMaster[act][e->v] == INFINITE */){
						_dijkstraMaster[ind][e->v] = (min + newWeight(min_index, *e));
					}
				}
			}
			else _dijkstraN[min_index] = INFINITE;
			visited[min_index] = true;
		}
	
	}

	void Johnson(){
		int min = INFINITE, cand, pt = INFINITE;
		vector<int> tempSums = vector<int>(_nafls, 0);
		bool solution = false;
		BellmanFord();
		for(int i = 0; i < _nafls; i++){
			//printGraph();
			//printf("dijkstra\n");
			Dijkstra(_afls[i], i);
		}
		for(int i=0; i < _nverts ; i++){
			//printf("%d\n",_dijkstraN[i] );
			if(_dijkstraN[i]!=INFINITE){
				solution = true;
				//break;
			}
		}
		
		if(solution){
			for(int v = 0; v < _nverts; v++){
				cand=0;
				for(int u = 0; u < _nafls; u++){
					if(_dijkstraMaster[u][v] == INFINITE)
						break;
					tempSums[u] = _dijkstraMaster[u][v] + _h[v] - _h[_afls[u]];
					cand += tempSums[u];
				}
				//printf("PONTO %d %d\n",v+1, cand );
				if(less_infinity(cand, min)){
					min=cand;
					pt = v;
					_sums=tempSums;
				} 
				if(cand == min)
					pt = less_infinity(v, pt) ? v : pt;	
			}

			printf("%d %d\n",pt+1,min );
			for(int i=0; i< _nafls; i++){
				printf("%d ",_sums[i]);
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
			printf("\n%d (h = %d, dij1=%d, dij2=%d): ", i+1, _h[i], _dijkstraMaster[0][i], _dijkstraMaster[1][i]);
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