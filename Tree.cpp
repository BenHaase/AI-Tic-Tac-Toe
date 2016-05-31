#include "Tree.h"
#include <cstdlib>
#include <iostream>
#ifndef TREE_CPP
#define TREE_CPP
using namespace std;

Tree::Tree(int player, char pxo){ //creates a new tree with a blank board
	human = pxo;
	node * n = new node;
	generateIntBoard(n);
	n->p = player;
	n->d = 12;
	root = n;
	n->parent = root;
	last = n;
}

Tree::Tree(node * r){ //creates a new tree, top node is current board state, for search
	node * n = new node;
	generateIntBoard(n);
	for(int i=0; i<rows; i++){
		for(int k=0; k<columns; k++) n->b[i][k] = r->b[i][k];
	}
	n->d = r->d;
	n->child = NULL;
	n->p = r->p;
	root = n;
	n->parent = root;
	current = n;
	last=n;
}

Tree::~Tree(void){ //deletes all nodes in tree
	current = last;
	while(current->parent!=root){
		last = current->parent;
		deleteNode(current);
		current = last;
	}
	deleteNode(current);
}

void Tree::generateIntBoard(node * n){ //generates a blank board
	n->b = new int*[rows];
	for (int i=0; i<rows; i++) n->b[i]=new int[columns];
	for (int i=0; i<rows; i++){
		for (int k=0; k<columns; k++) n->b[i][k]=0;}
}

Tree::node * Tree::createNode(node * np, int * move){ //creates a new node and adds it to the tree
	node * n = new node;
	generateIntBoard(n);
	for(int i=0; i<rows; i++){
		for(int k=0; k<columns; k++) n->b[i][k] = np->b[i][k];}
	n->b[move[0]][move[1]] = np->p;
	n->parent = np;
	np->child = n;
	n->child = NULL;
	n->p = -np->p;
	n->d = np->d-1;
	last = n;
	return n;
}

void Tree::mutate(node * n, int * m){ //mutate tree for search, did this instead of new nodes because of speed
	n->b[*m][*(m+1)] = n->p;
	n->d--;
	n->p=-n->p;
}

void Tree::unMutate(node * n, int * m){ //undo the mutation
	n->b[m[0]][m[1]] = 0;
	n->d++;
	n->p=-n->p;
}

void Tree::deleteNode(node * n){ //deletes a node
	for(int i=0; i<rows; i++) delete n->b[i];
	delete [] n->b;
	delete n;
}

int Tree::checkWin(int ** ib){ //checks board state for winner
	for(int i=0; i<2; i++){
		for(int k=0; k<3; k++) if(abs(ib[i][k]+ib[i+1][k]+ib[i+2][k])==3) return -ib[i][k];}
	for(int i=0; i<4; i++) if(abs(ib[i][0]+ib[i][1]+ib[i][2])==3) return -ib[i][0];
	if(abs(ib[0][0]+ib[1][1]+ib[2][2])==3) return -ib[0][0];
	if(abs(ib[2][0]+ib[1][1]+ib[0][2])==3) return -ib[2][0];
	if(abs(ib[1][0]+ib[2][1]+ib[3][2])==3) return -ib[1][0];
	if(abs(ib[3][0]+ib[2][1]+ib[1][2])==3) return -ib[3][0];
	return 0;
}

int * Tree::aiMove(node * n){ //base method for the minimax search, returns the best move from a search of the state changes from the current board state
	int wVal = -2; //holds the highest value of search
	int w; //holds the minimax value of node being search
	int * moves = new int [2]; //holds move of node being searched
	int * bestMove = new int[2]; //holds highest values move
	int * m = new int[2]; //move for search mutations, created here and passed it for speed
	for(int i=0; i<rows; i++){
		for(int k=0; k<columns; k++){
			if(n->b[i][k]==0){ //loops through whole board and if there is an empty board space generates new state from it and searches it
				moves[0]=i;
				moves[1]=k;
				mutate(n, moves); //gen new state
				w=searchTree(n, m); //search the state
				//cout << endl << "aimove: " << w << endl; //uncomment to see top node generated values from minimax algorithm
				if(w > wVal){ //see if search result is better than current
					wVal = w;
					bestMove[0]=i;
					bestMove[1]=k;
				}
				unMutate(n, moves); //undo the state change
			}
		}
	}
	cout << endl << "Minimax Value: " << wVal << endl;
	delete moves;
	return bestMove;
}
	

int Tree::searchTree(node * n, int * m){ //this method is called recursively on the node fromt he previous method being searched
	int w = checkWin(n->b); //check if this state has a winner
	if(n->d==0 && w==0) return w; //return if terminal node and no winner
	else if(w!=0 && n->d!=0) return w; //return if node is win or loss
	else if(n->d == 0) return w; //return terminal depth node
	else{
		int * scores = new int[n->d]; //hold the minimax values of the child nodes
		int count = 0; //counter variable
		//loop through possible state changes
		for(int i=0; i<rows; i++){
			for(int k=0; k<columns; k++){
				if(n->b[i][k]==0){
					m[0]=i;
					m[1]=k;
					mutate(n, m); //create state change
					scores[count]=searchTree(n, m); //search state change and store value
					m[0]=i;
					m[1]=k;
					unMutate(n, m); //undo state change
					if(scores[count]==n->p){w=n->p; //if will return a win for player pick this node, cuts search time significantly
						i=5;
						k=4;}
					count++;
				}
			}
		}
		//select the best node for the player
		if(w==0){
			if(n->p > 0){
				w=-2;
				for(int i=0; i<n->d; i++) {
					if(scores[i] > w) w = scores[i];
				}
			}
			if(n->p < 0){
				w= 2;
				for(int i=0; i<n->d; i++) {
					if(scores[i] < w) w = scores[i];
				}
			}
		}
		delete[] scores;
	}
	return w;
}

void Tree::printNode(node * n){ //prints a given state
	char ai;
	if(human == 'X' || human == 'x') ai='O';
	else ai='X';
	char ** cBoard = new char*[rows];
	for (int i=0; i<rows; i++) cBoard[i]=new char[columns];
	for (int i=0; i<rows; i++){
		for(int k=0; k<columns; k++){
			cBoard[i][k]='_';
			if(i==rows-1) cBoard[i][k]=' ';}
	}
	for (int i=0; i<rows; i++){
		for(int k=0; k<columns; k++){
			if(n->b[i][k]==-1)cBoard[i][k]=human;
			if(n->b[i][k]==1)cBoard[i][k]=ai;}}
	for (int i=0; i<rows; i++){
		for(int k=0; k<columns; k++){
			cout << cBoard[i][k];
			if(k!=columns-1) cout << '|';}
		cout << endl;}
	}
Tree::node * Tree::rLast(){return last;} //returns last node in tree
void Tree::printBoard(){printNode(last);} //prints the last node(current board state)
#endif