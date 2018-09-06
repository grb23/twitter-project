#include <iostream>
#include <cstdlib>
#include <list>
#include <stack>
#include <map>
#include <vector>


// a graph class to simplify in a way the sets of following and users
class Graph
{

public:
	Graph(int V);
	~Graph();

	void addEdge(int v, int w);   
	
	// return a vector with result
	std::vector<std::string> SCC(std::map<int, std::string> indexMap2);


private:
	std::list<int> *adjacencyList;
	int V;
	
	// dfs
	// a bunch of stuff to help like the index map
	void buildSCC(std::map<int, std::string> indexMap2,
		int u, int count, int lowlink[], int discovery[], std::stack<int> *userStack, 
		bool doesExistInStack[], std::vector<std::string>& resultV);
};