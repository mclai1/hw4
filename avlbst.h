#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

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
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    virtual void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    virtual void rotateRight(AVLNode<Key, Value>* n);
    virtual void rotateLeft(AVLNode<Key, Value>* n);
    virtual void removeFix(AVLNode<Key, Value>* n, int diff);

};


// Rotate Right
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* n)
{
    if (n == NULL){
        return;
    }

    // new parent is the current left child
    AVLNode<Key, Value>* p = n->getLeft();
    
    if (p == NULL){
        return;
    }
    // move parent right child to n's left child
    n->setLeft(p->getRight());
    // update parent pointers
    if (p->getRight() != NULL){
        p->getRight()->setParent(n);
    }

    // update n's parent
    p->setParent(n->getParent());
    if (n->getParent() == NULL){
        this->root_ = p;
    }
    else if (n == n->getParent()->getLeft()) {
        n->getParent()->setLeft(p);
    }
    else {
        n->getParent()->setRight(p);
    }

    // update p's right child
    n -> setParent(p);
    p -> setRight(n);
}

// Rotate Left
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* n)
{
    if (n == NULL){
        return;
    }

    // new parent is current right child
    AVLNode<Key, Value>* p = n->getRight();

    if (p == NULL){
        return;
    }
    // move parent left child to n's right child
    n->setRight(p->getLeft());
    // update parent pointers
    if (p->getLeft() != NULL){
        p->getLeft() -> setParent(n);
    }

    // update n's parent
    p->setParent(n->getParent());
    if (n->getParent() == NULL){
        this->root_ = p;
    }
    else if (n == n->getParent()->getLeft()) {
        n->getParent()->setLeft(p);
    }
    else {
        n->getParent()->setRight(p);
    }

    // update p's left child
    n -> setParent(p);
    p -> setLeft(n);
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    // If empty tree => set n as root, b(n) = 0, done!
    if (this->root_ == NULL){
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return;
    }

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = NULL;

    // Find where to insert the node
    while (curr != NULL) {
        parent = curr;
        if (new_item.first < curr->getKey()) {
            curr = curr->getLeft();
        } 
        else if (new_item.first > curr->getKey()) {
            curr = curr->getRight();
        }
        else {
            curr->setValue(new_item.second);
            return;
        }
    }

    // create new node with key/value
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    newNode -> setBalance(0);
    
    // tree is empty, set new node as root
    if (parent == NULL){
        this->root_ = newNode;
        return;
    }
    // add node as child of parent
    else if (new_item.first < parent->getKey()){
        parent->setLeft(newNode);
    }
    else {
        parent->setRight(newNode);    
    }

    // – If b(p) was -1, then b(p) = 0. Done!
    if (parent->getBalance() == -1){
        parent->setBalance(0);
        return;
    }
    // – If b(p) was +1, then b(p) = 0. Done!
    else if (parent->getBalance() == 1){
        parent->setBalance(0);
        return;
    }
    // – If b(p) was 0, then update b(p) and call insert-fix(p, n)
    else if (parent->getBalance() == 0){
        if (parent->getLeft() == newNode){
            parent->setBalance(-1);
        }
        else{
            parent->setBalance(1);
        }
        insertFix(newNode->getParent(), newNode);
    }
}

// Insert helper function
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
    // Precondition: p and n are balanced: {-1,0,1}
    // Postcondition: g, p, and n are balanced: {-1,0,1}
    // If p is null or parent(p) is null, return
    if (p == NULL || p->getParent() == NULL){
        return;
    }
    // Let g = parent(p)
    AVLNode<Key, Value>* g = p->getParent();
    // Assume p is left child of g [For right child swap left/right, +/-]
    if (p == g->getLeft()){
        // b(g) += -1 
        g->updateBalance(-1);
        // Case 1: b(g) == 0, return
        if (g->getBalance() == 0){
            return;
        }
        // Case 2: b(g) == -1, insertFix(g, p) // recurse
        else if (g->getBalance() == -1){
            insertFix(g, p);
        }
        // Case 3: b(g) == -2
        else if (g->getBalance() == -2){
            // zig-zig
            if (n == p->getLeft()){
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            // zig-zag
            else{
                rotateLeft(p);
                rotateRight(g);
                // Case 3a: b(n) == -1 then b(p) = 0; b(g) = +1; b(n) = 0;
                if (n->getBalance() == -1){
                    p->setBalance(0);
                    g->setBalance(1);
                    n->setBalance(0);
                }
                // Case 3b: b(n) == 0 then b(p) = 0; b(g) = 0; b(n) = 0;
                else if (n->getBalance() == 0){
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                // Case 3c: b(n) == +1 then b(p)= -1; b(g) = 0; b(n) = 0;
                else if (n->getBalance() == 1){
                    p->setBalance(-1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    }
    // p is a right child of g
    else if (p == g->getRight()){
        g->updateBalance(1);
        // Case 1: b(g) == 0, return
        if (g->getBalance() == 0){
            return;
        }
        // Case 2: b(g) == +1, insertFix(g, p) // recurse
        else if (g->getBalance() == 1){
            insertFix(g, p);
        }
        // Case 3: b(g) == +2
        else if (g->getBalance() == 2){
            // zig-zig
            if (n == p->getRight()){
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            // zig-zag
            else{
                rotateRight(p);
                rotateLeft(g);
                // adjust balances
                if (n->getBalance() == 1){
                    p->setBalance(0);
                    g->setBalance(-1);
                    n->setBalance(0);
                }
                else if (n->getBalance() == 0){
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if (n->getBalance() == -1){
                    p->setBalance(1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    }
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
    if (current == NULL) {
        return;
    }

    AVLNode<Key, Value>* p = current->getParent();
    int diff = 0;
    if (p != NULL){
        if (current == p->getLeft()){
            diff = 1;
        }
        else{
            diff = -1;
        }
    }

    // Update pointers
    // Two children, swap with predecessor
    if (current->getLeft() != NULL && current->getRight() != NULL){
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(current));
        nodeSwap(current, pred);
    }
    p = current->getParent();
    if (p != NULL){
        if (current == p->getLeft()){
            diff = 1;
        }
        else{
            diff = -1;
        }
    }
    // No children
    if (current->getRight() == NULL && current->getLeft() == NULL){
        if (current->getParent() == NULL){
            this->root_ = NULL;
        }
        else{
            if (current == current->getParent()->getLeft()){
                current->getParent()->setLeft(NULL);
            }
            else{
                current->getParent()->setRight(NULL);
            }
        }
    }
    // One child
    else {
        AVLNode<Key, Value>* child = NULL;
        // Left child
        if (current->getLeft() != NULL){
            child = current->getLeft();
        }
        // Right child
        else{
            child = current->getRight();
        }
        // Update child's parent pointer
        if (child != NULL){
            child->setParent(current->getParent());
        }
        // Update parent's child pointer
        if (current == this->root_){
            this->root_ = child;
        }
        else{
            if (current == current->getParent()->getLeft()){
                current->getParent()->setLeft(child);
            }
            else{
                current->getParent()->setRight(child);
            }
        }
    }

    // Delete n
    delete current;
    // “Patch tree” by calling removeFix(p, diff)
    if (p != NULL){
        removeFix(p, diff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n, int diff){
    if (n == NULL){
        return;
    }

    int ndiff = 0;
    AVLNode<Key, Value>* p = n->getParent();

    // check if node is left or right child
    if (p != NULL){
        if (n == p->getLeft()){
            ndiff = 1;
        }
        else{
            ndiff = -1;
        }
    }

    // diff from remove
    n->setBalance(n->getBalance() + diff);

    // diff=-1
    if (diff==-1){
        // Case 1: b(n) + diff == -2
        // – [Perform the check for the mirror case where b(n) + diff == +2, flipping left/right and -1/+1]
        // – Let c = left(n), the taller of the children
        if (n->getBalance() == -2){
            AVLNode<Key, Value>* c = n->getLeft();
            // – Case 1a: b(c) == -1 // zig-zig case
            // • rotateRight(n), b(n) = b(c) = 0, removeFix(p, ndiff)
            if (c->getBalance() == -1){
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            }
            // – Case 1b: b(c) == 0 // zig-zig case
            // • rotateRight(n), b(n) = -1, b(c) = +1 // Done!
            else if (c->getBalance() == 0){
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
            }
            // – Case 1c: b(c) == +1 // zig-zag case
            // • Let g = right(c)
            // • rotateLeft(c) then rotateRight(n)
            // • If b(g) was +1 then b(n) = 0, b(c) = -1, b(g) = 0
            // • If b(g) was 0 then b(n) = 0, b(c) = 0, b(g) = 0
            // • If b(g) was -1 then b(n) = +1, b(c) = 0, b(g) = 0
            // • removeFix(p, ndiff);
            else if (c->getBalance() == 1){
                AVLNode<Key, Value>* g = c->getRight();
                rotateLeft(c);
                rotateRight(n);
                if (g->getBalance() == 1){
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 0){
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if (g->getBalance() == -1){
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }
        // • Case 2: b(n) + diff == -1: then b(n) = -1; // Done!
        else if (n->getBalance() == -1){
            n->setBalance(-1);
        }
        // • Case 3: b(n) + diff == 0: then b(n) = 0, removeFix(p, ndiff)
        else if (n->getBalance() == 0){
            removeFix(p, ndiff);
        }
    }
    // diff=+1
    else if (diff==1){
        // Case 1: b(n) + diff == +2
        // – [Perform the check for the mirror case where b(n) + diff == -2, flipping left/right and -1/+1]
        // – Let c = right(n), the taller of the children
        if (n->getBalance() == 2){
            AVLNode<Key, Value>* c = n->getRight();
            // – Case 1a: b(c) == +1 // zig-zig case
            // • rotateLeft(n), b(n) = b(c) = 0, removeFix(p, ndiff)
            if (c->getBalance() == 1){
                rotateLeft(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            }
            // – Case 1b: b(c) == 0 // zig-zig case
            // • rotateLeft(n), b(n) = +1, b(c) = -1 // Done!
            else if (c->getBalance() == 0){
                rotateLeft(n);
                n->setBalance(1);
                c->setBalance(-1);
            }
            // – Case 1c: b(c) == -1 // zig-zag case
            // • Let g = left(c)
            // • rotateRight(c) then rotateLeft(n)
            // • If b(g) was -1 then b(n) = 0, b(c) = +1, b(g) = 0
            // • If b(g) was 0 then b(n) = 0, b(c) = 0, b(g) = 0
            // • If b(g) was +1 then b(n) = -1, b(c) = 0, b(g) = 0
            // • removeFix(p, ndiff);
            else if (c->getBalance() == -1){
                AVLNode<Key, Value>* g = c->getLeft();
                rotateRight(c);
                rotateLeft(n);
                if (g->getBalance() == -1){
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 0){
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 1){
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }
        else if (n->getBalance() == 1){
            n->setBalance(1);
        }
        else if (n->getBalance() == 0){
            removeFix(p, ndiff);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
