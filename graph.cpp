#include <iostream>
#include <cstdlib>
#include <list>
#include <stack>
#include <map>
#include <vector>
#include "graph.h"

Graph::Graph(int V)
{
    adjacencyList = new std::list<int>[V];

    this->V = V;
}

Graph::~Graph()
{

}

void Graph::addEdge(int v, int w)
{
    adjacencyList[v].push_back(w);
}

std::vector<std::string> Graph::SCC(std::map<int, std::string> indexMap2)
{
	std::vector<std::string> resultV;

	bool *doesExistInStack = new bool[V];

    int *discovery = new int[V];

 	int *lowlink = new int[V];

 	std::stack<int> *userStack = new std::stack<int>();

    for (int i = 0; i < V; i++)
    {
    	doesExistInStack[i] = false;
        discovery[i] = -1;
        lowlink[i] = -1;
    }

    int count = 1;

	for (int i = 0; i < V; i++)
	{
		if (discovery[i] == -1)
		{
        	buildSCC(indexMap2, i, count, lowlink, discovery, userStack, doesExistInStack, resultV);
			count++;
		}
	}

	delete[] discovery;
	delete[] lowlink;
	delete[] doesExistInStack;
	delete userStack;

    return resultV;
}

void Graph::buildSCC(std::map<int, std::string> indexMap2,
		int u, int count, int lowlink[], int discovery[], std::stack<int> *userStack,
		bool doesExistInStack[], std::vector<std::string>& resultV)
{
	// u and w are the vertices

    static int time = 0;
    int hold = time++;
    int w = 0;


    lowlink[u] = hold;
    discovery[u] = hold;

    userStack->push(u);

	doesExistInStack[u] = true;

    // the adjacency list with the ints and edges we manually added earlier in twiteng
    std::list<int>::iterator i;
    for (i = adjacencyList[u].begin(); i != adjacencyList[u].end(); ++i)
    {
        int vert = *i;

		if (doesExistInStack[vert] == true)
        {
            lowlink[u] = std::min(lowlink[u], discovery[vert]);
        }
        else if (discovery[vert] == -1)
        {
        	// else recurse
        	buildSCC(indexMap2, vert, count, lowlink, discovery, userStack, doesExistInStack, resultV);

            lowlink[u] = std::min(lowlink[u], lowlink[vert]);
        }
    }

    if (lowlink[u] == discovery[u])
    {
        while (userStack->top() != u)
        {
            w = userStack->top();
           	userStack->pop();

            // add to result
            resultV.push_back(indexMap2[w]);

            doesExistInStack[w] = false;
        }

        w = userStack->top();
		userStack->pop();

        // END tells us this component is sdone
        resultV.push_back(indexMap2[w]);
		resultV.push_back("END");

        doesExistInStack[w] = false;
    }
}
