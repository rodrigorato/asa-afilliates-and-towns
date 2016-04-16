#include <stdio.h>
#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include <algorithm>

#define INFINITE -1

using namespace std;

class DirectedGraph{
private:
	int _numVertices;
	int _numEdges; // Maybe we don't need this (used only to get input?	)
	list<int>* _adjacencyLists;

public:
	DirectedGraph(int numVertices){
		_numVertices = numVertices;
		_adjacencyLists = new list<int>[numVertices];
	}

	~DirectedGraph(){
		delete[] _adjacencyLists;
	}

	void addEdge(int v1, int v2){
		_adjacencyLists[v1].push_back(v2);
		_adjacencyLists[v2].push_back(v1);
	}
};

int main(){
	printf("Hello world!\n");
	return 0;
}
