#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>
#include <vector>
#include "bst.h"
#include "splay_tree.h"

using namespace std;

template<typename Key, typename Value>
void preorder(Node<Key, Value>* temp)
{
	if(temp != NULL)
	{
		if(temp->getParent() != NULL)
		{
			std::cout << temp->getKey() << "	Parent: " << temp->getParent()->getKey() << std::endl;
		}
		else
		{
			std::cout << temp->getKey() << std::endl;
		}

		if(temp->getRight() != NULL)
		{
			std::cout << "	Right: " << temp->getRight()->getKey() << std::endl;
		}

		if(temp->getLeft() != NULL)
		{
			std::cout << "	Left: " << temp->getLeft()->getKey() << std::endl;
		}


		preorder(temp->getLeft());
		preorder(temp->getRight());
	}
}


int main(int argc, char* argv[])
{
	cout << "hello" << endl;
	
	SplayTree<int, int> stree;
	
	Node<int, int>* temp;
	
	stree.insert(std::make_pair(20, 101));	
	stree.insert(std::make_pair(30, 201));
	stree.insert(std::make_pair(5, 301));
	stree.insert(std::make_pair(15, 401));
	stree.insert(std::make_pair(12, 501));
	stree.insert(std::make_pair(10, 601));
	stree.insert(std::make_pair(3, 701));
	stree.insert(std::make_pair(25, 801));
	stree.insert(std::make_pair(8, 901));

	temp = stree.getRoot();
	std::cout << "NORMAL OUTPUT-------------" << std::endl;
	preorder(temp);	

	SplayTree<int,int>::iterator it = stree.begin();
	std::cout << it->second << std::endl;

	temp = stree.getRoot();
	std::cout << "AFTER OUTPUT-------------" << std::endl;
	preorder(temp);
}