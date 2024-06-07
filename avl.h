#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
using namespace std;


struct AVLNode
{
    string name;
    double timeTaken;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(string n, double t) : name(n), timeTaken(t), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree
{
public:
    AVLTree() : root(nullptr) {}
    void insert(const string &name, double timeTaken);
    void inOrderTraversal(double timeLimit) const;
    AVLNode *root;

private:
    int height(AVLNode *node);
    int getBalance(AVLNode *node);
    AVLNode *insert(AVLNode *node, const string &name, double timeTaken);
    AVLNode *rightRotate(AVLNode *y);
    AVLNode *leftRotate(AVLNode *x);
    void inOrderTraversal(AVLNode *node, double timeLimit) const;
};


int AVLTree::height(AVLNode *node)
{
    return node ? node->height : 0;
}

int AVLTree::getBalance(AVLNode *node)
{
    return node ? height(node->left) - height(node->right) : 0;
}

AVLNode *AVLTree::rightRotate(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode *AVLTree::leftRotate(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

AVLNode *AVLTree::insert(AVLNode *node, const string &name, double timeTaken)
{
    if (!node)
        return new AVLNode(name, timeTaken);

    if (timeTaken < node->timeTaken)
    {
        node->left = insert(node->left, name, timeTaken);
    }
    else if (timeTaken > node->timeTaken)
    {
        node->right = insert(node->right, name, timeTaken);
    }
    else
    {
        return node; 
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && timeTaken < node->left->timeTaken)
    {
        return rightRotate(node);
    }

    if (balance < -1 && timeTaken > node->right->timeTaken)
    {
        return leftRotate(node);
    }

    if (balance > 1 && timeTaken > node->left->timeTaken)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && timeTaken < node->right->timeTaken)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void AVLTree::insert(const string &name, double timeTaken)
{
    root = insert(root, name, timeTaken);
}

void AVLTree::inOrderTraversal(AVLNode *node, double timeLimit) const
{
    if (node)
    {
        inOrderTraversal(node->left, timeLimit);
        if (node->timeTaken <= timeLimit)
        {
            cout << "+---------------------------------------------+" << endl;
            cout << "|   Name: " << setw(10) << left << node->name << "   Time: " << setw(10) << right << fixed << setprecision(2) << node->timeTaken << " seconds   |" << endl;
            cout << "+---------------------------------------------+" << endl;
        }
        inOrderTraversal(node->right, timeLimit);
    }
}

void AVLTree::inOrderTraversal(double timeLimit = 10000) const
{
    inOrderTraversal(root, timeLimit);
}

#endif 
