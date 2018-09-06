#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <cstdlib>
#include <string>
#include "bst.h"

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    char getBalance () const;
    void setBalance (char balance);
    void updateBalance(char diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    char balance_; // just put in an int
};

/*
--------------------------------------------
Begin implementations for the AVLNode class.
--------------------------------------------
*/

/**
* Constructor for an AVLNode. Nodes are initialized with a balance of 0.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
    : Node<Key, Value>(key, value, parent),
      balance_(0)
{

}

/**
* Destructor.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
char AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(char balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(char diff)
{
    balance_ += diff;
}

/**
* Getter function for the parent. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mParent);
}

/**
* Getter function for the left child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mLeft);
}

/**
* Getter function for the right child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mRight);
}

/*
------------------------------------------
End implementations for the AVLNode class.
------------------------------------------
*/

/**
* A templated balanced binary search tree implemented as an AVL tree.
*/
template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    // Methods for inserting/erasing (removing) elements from the tree. 
    // You must implement both of these methods.
    virtual void insert(const std::pair<Key, Value>& keyValuePair) override;
    virtual void erase(const Key& key);

    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void eraseFix(AVLNode<Key, Value> *p, int diff);
    
    AVLNode<Key, Value>* successorFind(AVLNode<Key, Value> *n);


private:
    /* Helper functions are strongly encouraged to help separate the problem
       into smaller pieces. You should not need additional data members. */

    /* You should write these helpers for sure.  You may add others. */
    void rotateLeft (AVLNode<Key, Value> *n);
    void rotateRight (AVLNode<Key, Value> *n);

    /* A provided helper function to swap 2 nodes location in the tree */
    void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
};

/*
--------------------------------------------
Begin implementations for the AVLTree class.
--------------------------------------------
*/

template<typename Key, typename Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{

    if(p == NULL)
    {
        return;
    }

    if(p->getParent() == NULL)
    {
        return;
    }

    AVLNode<Key, Value>* g = p->getParent();

    if(g->getLeft() == p)
    {
        int hold = g->getBalance()-1;
        g->setBalance(hold);

        if(g->getBalance() == 0)
        {
            return;
        }

        if(g->getBalance() == -1)
        {   
            insertFix(g, p);
        }  

        if(g->getBalance() == -2)
        {
            if(p->getLeft() == n && g->getLeft() == p)
            {
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else if(p->getRight() == n && g->getLeft() == p)
            {
                rotateLeft(p);
                rotateRight(g);

                if(n->getBalance() == -1)
                {
                    p->setBalance(0);
                    g->setBalance(1);
                    n->setBalance(0);
                }
                else if(n->getBalance() == 0)
                {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if(n->getBalance() == 1)
                {
                    p->setBalance(-1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }   
        }
    }
    else if(g->getRight() == p)
    {
        int hold = g->getBalance()+1;
        g->setBalance(hold);

        if(g->getBalance() == 0)
        {
            return;
        }

        if(g->getBalance() == 1)
        {   
            insertFix(g, p);
        }  
        
        if(g->getBalance() == 2)
        {   
            if(p->getRight() == n && g->getRight() == p)
            {
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else if(p->getLeft() == n && g->getRight() == p)
            {
                rotateRight(p);
                rotateLeft(g);

                if(n->getBalance() == 1)
                {
                    p->setBalance(0);
                    g->setBalance(-1);
                    n->setBalance(0);
                }
                else if(n->getBalance() == 0)
                {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if(n->getBalance() == -1)
                {
                    p->setBalance(1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            } 
        }  

    }


}



/**
* Insert function for a key value pair. Finds location to insert the node and then balances the tree.
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
    // TODO

    AVLNode<Key, Value>* temp = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);//Node(keyValuePair);

    temp->setLeft(NULL);
    temp->setRight(NULL);
    temp->setParent(NULL);
    temp->setBalance(0);

    if(this->mRoot == NULL)
    {
        this->mRoot = temp;
        this->mRoot->setLeft(NULL);
        this->mRoot->setRight(NULL);
        this->mRoot->setParent(NULL);
    }
    else
    {
        AVLNode<Key, Value>* n = static_cast<AVLNode<Key,Value>*>(this->mRoot);// = new Node<Key, Value>;

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

                    if(n->getBalance() == -1)
                    {
                        n->setBalance(0);
                        return;
                    }
                    else if(n->getBalance() == 1)
                    {
                        n->setBalance(0);
                        return;
                    }
                    else if(n->getBalance() == 0)
                    {
                        n->setBalance(-1);
                        insertFix(n, temp);
                        return;
                    }
                    
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
                    
                    if(n->getBalance() == -1)
                    {
                        n->setBalance(0);
                        return;
                    }
                    else if(n->getBalance() == 1)
                    {
                        n->setBalance(0);
                        return;
                    }
                    else if(n->getBalance() == 0)
                    {
                        n->setBalance(1);
                        insertFix(n, temp);
                        return;
                    }
                    
                    return;
                }
                n = n->getRight();
            }
        }
    }

}


/**
* Erase function for a given key. Finds the node, reattaches pointers, and then balances when finished.
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::erase(const Key& key)
{
    // TODO
    int diff = 0;
    AVLNode<Key, Value>* n = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));

    if(n == NULL)
    {
        return;
    }

    if(n->getLeft() != NULL && n->getRight() != NULL)
    {
        AVLNode<Key, Value>* successor = successorFind(n);

        // successor swap
        nodeSwap(successor, n);
        //n = successor;
    }

    AVLNode<Key, Value>* p = n->getParent();

    if(p != NULL)
    {
        if(p->getLeft() == n)
        {
            diff = 1;
        }
        else if(p->getRight() == n)
        {
            diff = -1;
        }
    }

    // DELETE AND PATCH

    if(n->getLeft() == NULL && n->getRight() == NULL)
    {
        //delete(n);
        
        if(p->getRight() == n)
        {
            AVLNode<Key, Value>* temp = n;
            p->setRight(NULL);
            delete(temp);
        }
        else if(p->getLeft() == n)
        {
            AVLNode<Key, Value>* temp = n;
            p->setLeft(NULL);
            delete(temp);
        }
    }
    else if(n->getLeft() != NULL && n->getRight() == NULL)
    {        
        AVLNode<Key, Value>* childHold = n->getLeft();
        AVLNode<Key, Value>* nHold = n;

        if(p->getLeft() == n)
        {
            p->setLeft(childHold);
            childHold->setParent(p);

            delete(nHold);
        }
        else if(p->getRight() == n)
        {
            p->setRight(childHold);
            childHold->setParent(p);
            delete(nHold);
        }

    }
    else if(n->getLeft() == NULL && n->getRight() != NULL)
    {
        AVLNode<Key, Value>* childHold = n->getRight();
        AVLNode<Key, Value>* nHold = n;

        if(p->getLeft() == n)
        {
            p->setLeft(childHold);
            childHold->setParent(p);
            delete(nHold);
        }
        else if(p->getRight() == n)
        {
            p->setRight(childHold);
            childHold->setParent(p);
            delete(nHold);
        }

    }

    eraseFix(p, diff);
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::eraseFix(AVLNode<Key, Value> *n, int diff)
{
    if(n == NULL)
    {
        return;
    }

    int ndiff;
    AVLNode<Key, Value>* p = n->getParent();

    if(p != NULL)
    {
        if(p->getLeft() == n)
        {
            ndiff = 1;
        }
        else if(p->getRight() == n)
        {
            ndiff = -1;
        }
    }

    if(diff == -1)
    {
        if(n->getBalance() + diff == -2)
        {
            AVLNode<Key, Value>* c = n->getLeft();

            if(c->getBalance() == -1 || c->getBalance() == 0)
            {
                rotateRight(n);
                if(c->getBalance() == -1)
                {
                    n->setBalance(0);
                    c->setBalance(0);
                    eraseFix(p, ndiff);
                }
                else if (c->getBalance() == 0)
                {
                    n->setBalance(-1);
                    c->setBalance(1);
                    return;
                }
            }
            else if(c->getBalance() == 1)
            {
                AVLNode<Key, Value>* g = c->getRight();
                rotateLeft(c);
                rotateRight(n);

                if(g->getBalance() == 1)
                {
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 0)
                {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if (g->getBalance() == -1)
                {
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }

                eraseFix(p, ndiff);
            }
        } 
        else if(n->getBalance() + diff == -1)
        {
            n->setBalance(-1);
            return;
        }
        else if(n->getBalance() + diff == 0)
        {
            n->setBalance(0);
            eraseFix(p, ndiff);
        }
    }
    else if(diff == 1)
    {
        // mirror
        if(n->getBalance() + diff == 2)
        {
            AVLNode<Key, Value>* c = n->getRight();

            if(c->getBalance() == 1 || c->getBalance() == 0)
            {
                rotateLeft(n);
                if(c->getBalance() == 1)
                {
                    n->setBalance(0);
                    c->setBalance(0);
                    eraseFix(p, ndiff);
                }
                else if (c->getBalance() == 0)
                {
                    n->setBalance(1);
                    c->setBalance(-1);
                    return;
                }
            }
            else if(c->getBalance() == -1)
            {
                AVLNode<Key, Value>* g = c->getLeft();
                rotateRight(c);
                rotateLeft(n);

                if(g->getBalance() == -1)
                {
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 0)
                {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 1)
                {
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }

                eraseFix(p, ndiff);
            }
        }
        else if(n->getBalance() + diff == 1)
        {
            n->setBalance(1);
            return;
        }
        else if(n->getBalance() + diff == 0)
        {
            n->setBalance(0);
            eraseFix(p, ndiff);
        }

    }
}


template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::successorFind(AVLNode<Key, Value> *n)
{

    if(n->getRight() != NULL)
    {
        n = n->getRight();

        while(n->getLeft() != NULL)
        {
            n = n->getLeft();
        }
        return n;
    }

    AVLNode<Key, Value>* p = n->getParent();

    while(p != NULL && n == p->getRight())
    {
        n = p;
        p = p->getParent();
    }

    return p;
}


/**
* Rotates n down and to the left
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateLeft (AVLNode<Key, Value> *n)
{
    // TODO


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
        AVLNode<Key, Value>* temp;

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

    AVLNode<Key, Value>* temp;
        
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

/**
* Rotates n down and to the right
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateRight (AVLNode<Key, Value> *n)
{
    // TODO


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
        AVLNode<Key, Value>* temp;
        
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

    AVLNode<Key, Value>* temp;
        
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

/**
 * Given a correct AVL tree, this functions relinks the tree in such a way that
 * the nodes swap positions in the tree.  Balances are also swapped.
 */
template<typename Key, typename Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    char temp2 = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(temp2);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->mRoot == n1) {
        this->mRoot = n2;
    }
    else if(this->mRoot == n2) {
        this->mRoot = n1;
    }

}

/*
------------------------------------------
End implementations for the AVLTree class.
------------------------------------------
*/

#endif
