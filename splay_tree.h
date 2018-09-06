#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>
#include "bst.h"
#include "avlbst.h"

template<typename Key, typename Value>
class SplayTree : public BinarySearchTree<Key, Value>
{
 public:
  SplayTree();
  ~SplayTree();

  void insert(const std::pair<Key, Value>& keyValuePair);
  void clear();
  Value& operator[](const Key& k);
  Value& get(const Key& k);

  typename SplayTree<Key, Value>::iterator find(const Key& k); 

  // used for testing when you don't want the splay action
  void insertHelp(const std::pair<Key, Value>& keyValuePair);

 protected:
	void splay(Node<Key, Value> *n);
	Node<Key, Value>* findHelp(const Key& key);


	void rotateLeft (Node<Key, Value> *n);
    void rotateRight (Node<Key, Value> *n);

 private:
};

template<typename Key, typename Value>
SplayTree<Key, Value>::SplayTree()
{
	BinarySearchTree<Key, Value>::setRoot(NULL);
}


template<typename Key, typename Value>
SplayTree<Key, Value>::~SplayTree()
{

}

template<typename Key, typename Value>
void SplayTree<Key, Value>::clear()
{
    BinarySearchTree<Key, Value>::clear();
}

template <typename Key, typename Value>
typename SplayTree<Key, Value>::iterator SplayTree<Key, Value>::find(const Key& key)
{
	Node<Key,Value>* temp;
	temp = this->BinarySearchTree<Key, Value>::internalFind(key);

	if(this->getRoot() == NULL)
	{
		return BinarySearchTree<Key, Value>::end();
	}

	if(temp != NULL)
	{
		splay(temp);
		typename BinarySearchTree<Key, Value>::iterator it(temp);
		return it;
	}
	else
	{
		temp = findHelp(key);
		splay(temp);
		return BinarySearchTree<Key, Value>::end();
	}

}

template <typename Key, typename Value>
Node<Key, Value>* SplayTree<Key, Value>::findHelp(const Key& key)
{

	Node<Key, Value>* temp;
	temp = this->getRoot();

	if(temp != NULL)
	{
		if(temp->getKey() == key)
		{
			return temp;
		}
	}

	while(temp != NULL)
	{
		if(temp->getLeft() != NULL)
		{
			if(temp->getLeft()->getKey() == key)
			{
				return temp->getLeft();
			}
		}

		if(temp->getRight() != NULL)
		{
			if(temp->getRight()->getKey() == key)
			{
				return temp->getRight();
			}
		}

		if(temp->getKey() > key)
		{
			if(temp->getLeft() == NULL)
			{
				return temp;
			}
			temp = temp->getLeft();
		}
		else if(temp->getKey() < key)
		{
			if(temp->getRight() == NULL)
			{
				return temp;
			}
			temp = temp->getRight();
		}

	}

	return NULL;

}

// done
template <typename Key, typename Value>
void SplayTree<Key, Value>::insertHelp(const std::pair<Key, Value>& keyValuePair)
{
	Node<Key, Value>* temp = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);//Node(keyValuePair);

    temp->setLeft(NULL);
    temp->setRight(NULL);
    temp->setParent(NULL);

    if(this->mRoot == NULL)
    {
        this->mRoot = temp;
        this->mRoot->setLeft(NULL);
        this->mRoot->setRight(NULL);
        this->mRoot->setParent(NULL);
    }
    else
    {
        Node<Key, Value>* n = this->mRoot;// = new Node<Key, Value>;

        bool added = false;

        while(added != true)
        {
			if(n->getKey() == temp->getKey())
            {
                added = true;
                return;
            }

            if(n->getKey() > temp->getKey())
            {
                if(n->getLeft() == NULL)
                {
                    temp->setParent(n);
                    n->setLeft(temp);
                    added = true;

                    return;
                }
                n = n->getLeft();
            }
            else if(n->getKey() < temp->getKey())
            {
                if(n->getRight() == NULL)
                {
                    temp->setParent(n);
                    n->setRight(temp);
                    added = true;

                    return;
                }
                n = n->getRight();
            }
        }
    }
}

// done
template <typename Key, typename Value>
void SplayTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{

	Node<Key,Value>* temp;
	Key key = keyValuePair.first;

	temp = this->BinarySearchTree<Key, Value>::internalFind(key);

	// if it's not in the tree, insert, splay
	if(temp == NULL)
	{
		insertHelp(keyValuePair);
		temp = this->BinarySearchTree<Key, Value>::internalFind(key);

		splay(temp);

		return;
	}
	else if(temp != NULL)
	{
        // if it's in the tree, just splay and return
		splay(temp);
		return;
	}

	return;
}

// done
template <typename Key, typename Value>
Value& SplayTree<Key, Value>::get(const Key& k)
{
    // assuming you want to splay whenever you get
	return (this->find(k))->second;
}

// done
template <typename Key, typename Value>
Value& SplayTree<Key, Value>::operator[](const Key& k)
{
    // same as get
	return get(k);
}

// done
template <typename Key, typename Value>
void SplayTree<Key, Value>::splay(Node<Key, Value> *n)
{
	// zig zig
	// zag zag
	// zig zag
	// zag zig

	if(n == NULL)
	{
		return;
	}


	while(n != this->getRoot())
	{
		Node<Key, Value>* p = n->getParent();
		Node<Key, Value>* g = p->getParent();

		if(g == NULL)
		{
			// one rotation
			if(p->getRight() == n)
			{
				rotateLeft(p);
			}
			else if(p->getLeft() == n)
			{
				rotateRight(p);
			}
		}
		else
		{
			if(g->getLeft() == p && p->getLeft() == n)
			{
				// zig zig
				rotateRight(g);
				rotateRight(p);

			}
			else if(g->getRight() == p && p->getRight() == n)
			{
				// zig zig
				rotateLeft(g);
				rotateLeft(p);

			}
			else if(g->getRight() == p && p->getLeft() == n)
			{
				// zig zag
				rotateRight(p);
				rotateLeft(g);

			}
			else if(g->getLeft() == p && p->getRight() == n)
			{
				// zig zag
				rotateLeft(p);
				rotateRight(g);
			}

		}
	}

}

// done
template<typename Key, typename Value>
void SplayTree<Key, Value>::rotateLeft (Node<Key, Value> *n)
{
    // TODO
    // from avl implementaion


    if(n == NULL)
    {
        return;
    }

    if(n->getRight() == NULL)
    {
        return;
    }

    if(n->getParent() == NULL)
    {
        Node<Key, Value>* temp;

        temp = n->getRight();
        n->setRight(temp->getLeft());

        if(temp->getLeft() != NULL)
        {
            temp->getLeft()->setParent(n);
        }

        temp->setLeft(n);
        n->setParent(temp);
        this->setRoot(temp);
        temp->setParent(NULL);

        return;
    }


    Node<Key, Value>* temp;

    temp = n->getRight();
    n->setRight(temp->getLeft());

    if(temp->getLeft() != NULL)
    {
        temp->getLeft()->setParent(n);
    }

    temp->setParent(n->getParent());

    if(n->getParent() == NULL)
    {
        this->setRoot(temp);
    }
    else if(n->getParent()->getLeft() == n)
    {
        n->getParent()->setLeft(temp);
    }
    else
    {
        n->getParent()->setRight(temp);
    }

    temp->setLeft(n);

    n->setParent(temp);

    return;
}

// done
template<typename Key, typename Value>
void SplayTree<Key, Value>::rotateRight (Node<Key, Value> *n)
{
    // TODO
    // from avl implementaion


    if(n == NULL)
    {
        return;
    }

    if(n->getLeft() == NULL)
    {
        return;
    }

    if(n->getParent() == NULL)
    {
        Node<Key, Value>* temp;

        temp = n->getLeft();
        n->setLeft(temp->getRight());

        if(temp->getRight() != NULL)
        {
            temp->getRight()->setParent(n);
        }

        temp->setRight(n);
        n->setParent(temp);
        this->setRoot(temp);
        temp->setParent(NULL);

        return;
    }

    Node<Key, Value>* temp;

    temp = n->getLeft();
    n->setLeft(temp->getRight());

    if(temp->getRight() != NULL)
    {
        temp->getRight()->setParent(n);
    }

    temp->setParent(n->getParent());

    if(n->getParent() == NULL)
    {
        this->setRoot(temp);
    }
    else if(n->getParent()->getRight() == n)
    {
        n->getParent()->setRight(temp);
    }
    else
    {
        n->getParent()->setLeft(temp);
    }

    temp->setRight(n);

    n->setParent(temp);

    return;

}

#endif
