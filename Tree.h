#include <vector>
#pragma once
#ifndef TREE_H
#define TREE_H
class Tree
{
private:
	struct node{int ** b; //board state
				int p; //player to make next move
				int d; //depth
				node * parent; //previous state
				node * child;}; //next state
	node * root; //root
	node * current; //traversal pointer
	node * last; //pointer to the deepest node in the tree
	static const int rows = 4; //row size
	static const int columns = 3; //column size
	char human; //human players char for printing state

public:
	Tree(void);
	Tree(int, char);
	Tree(node * n);
	~Tree(void);
	void Tree::generateIntBoard(node * n); //generates a blank board state
	node * createNode(node *, int *); //creates a node
	void mutate(node *, int *); //change node for search
	void unMutate(node *, int *);//change back
	void deleteNode(node *); //delete nodes
	int checkWin(int **); //check if state has winner
	int * aiMove(node*); //base method for search to return move
	int searchTree(node*, int *); //recursive method for search
	void printNode(node *); //print a node
	node * rLast(); //returns last node
	void printBoard(); //print last node
};
#endif