// 4300Proj2.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <cstdlib>
#include <time.h>
#include "Tree.h"
using namespace std;

int game(); //method to run the game
int * humanMove(int **); //method for human to make a move
void firstMove(int *, char *); //method to choose X or O and 1st or 2nd
int * ai(Tree *); //method to get the ai's move
int main(int argc, char* argv[]) 
{
	int winner = game();
	return 0;
}

int game()
{
	int * ip = new int; //holds the player to make the first move
	char * cp = new char; //holds the humans X or O choice
	int * aiM; //holds the ai's move
	firstMove(ip, cp); //determine who makes first move and who is X O
	Tree * g = new Tree(*ip, *cp); //creating game tree
	g->printBoard(); //print the blank board
	do{
		//get the player or ai's move based on who goes next and create that state
		if(g->rLast()->p < 0){g->createNode(g->rLast(), humanMove(g->rLast()->b));
			cout << endl << "Your move: " << endl;}
		else if(g->rLast()->p > 0){ aiM = ai(g);
			g->createNode(g->rLast(), aiM);
			cout << endl << "AI Move:" << endl;}
		g->printBoard(); //display the move
	}while(g->checkWin(g->rLast()->b) == 0 && g->rLast()->d != 0); //do until there is a winner or a tie
    if(g->checkWin(g->rLast()->b)==1) cout << "AI Wins! " << endl; //output if ai wins
	else if(g->checkWin(g->rLast()->b)==0)cout << "Tie! " << endl; //output if tie
	else if(g->checkWin(g->rLast()->b)==-1) cout << "You Win!? " << endl; //never used but just in case faulty board passed
	delete g; //delete the tree
	return 0;
}

int * humanMove(int ** ib) //get the human's move choice
{
	char c;
	int * ic = new int[2];
	ic[0]=0;
	ic[1]=0;
	cout << endl << "Enter move choice: ";
	cin >> c;
	//convert input to board move and error check
	if(c=='/') return ic;
	if(c=='*') ic[1]=1;
	else if(c=='-'){ic[1]=2;
		return ic;}
	else{
		int iIn = c - '0';
		iIn--;
		if((iIn/3)>1) ic[0]=1;
		else if((iIn/3)<1) ic[0]=3;
		else ic[0]=2;
		ic[1] = (iIn%3);
	}
	if(ib[ic[0]][ic[1]] == 0) return ic;
	else{
		cout << "Invalid choice" << endl;
		humanMove(ib);}
}

void firstMove(int * ip, char * cp) //gets players input choices to start game
{
	char human;
	int humanMove;
	//get players X O choice
	cout << "Select X or O: "; 
	cin >> human;
	cout << endl;
	*cp = human;
	//get players 1st or 2nd choice
	cout << "Go first or second(1/2): ";
	cin >> humanMove;
	cout << endl;
	if(humanMove==1) *ip=-1;
	else if(humanMove==2) *ip=1;
	else firstMove(ip, cp);
	//random move generation
	//srand(time(NULL));
	//if((rand()%100) > 49) *ip = -1;
	//else *ip = 1;
}

int * ai(Tree * g) //get ai's move, creates tree and searches it then returns best move
{
	int * c;
	Tree * s = new Tree(g->rLast());
	c = s->aiMove(s->rLast());
	delete s;
	return c;
}