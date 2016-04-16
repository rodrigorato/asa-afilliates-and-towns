#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

#define INFINITE -1

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

public:
	DirectedGraph(int nverts, int nedges, int nafls, int* afls){
		/* Class constructor */
		_nverts = nverts;
		_nedges = nedges;
		_nafls = nafls;
		_afls = afls;
		_adjLists = new list<Edge>[nverts];
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
			printf("\n%d: ", i+1);
			for(int j = 0; j < _adjLists[i].size(); j++){
				std::list<Edge>::iterator it = std::next(_adjLists[i].begin(), j);
				if(j == _adjLists[i].size() - 1)
					printf("(%d, %d)", it->v, it->w);
				else
					printf("(%d, %d), ", it->v, it->w);
			}
		}
		printf("\n");
	}
};

int main(){
	int n_towns, n_afls, n_conns;
	int *afls = new int[n_afls]; 

	scanf("%d %d %d", &n_towns, &n_afls, &n_conns);
	for(int i = 0; i < n_afls; i++){
		scanf("%d", &(afls[i]));
		afls[i]--; // Translating indexes - internally we start at zero
	}
	DirectedGraph g = DirectedGraph(n_towns, n_conns, n_afls, afls);

	for(int i = 0; i < n_conns; i++){
		int u, v, w;
		scanf("%d %d %d", &u, &v, &w);
		g.addEdge(u, v, w);
	}

	g.printGraph();

	// TRANSLATE INDEXES BACK FROM n-1 to n!!
	return 0;
}
