#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

// #include <ysglfontdata.h>
// #include <fslazywindow.h>

#include "bintree.h"



int main(){

	BinaryTree <int,int> tree1;

	// first test Find prev
	srand((int)time(nullptr));
	int ct=0;

	while(ct<100){

		tree1.Insert( rand()%100 , 0 );	
		ct++;
	}

	auto prevHd = tree1.First();
	auto nextHd = tree1.FindNext( prevHd );

	for(int i=0; i<99; i++){
		if ( prevHd != tree1.FindPrev(nextHd) )
			return 1;

		prevHd = nextHd;
		nextHd = tree1.FindNext(nextHd);
	}


	// test find last
	int num_last = 100 + rand()%100; 
	tree1.Insert( num_last, 0 );
	if( tree1.GetKey(tree1.Last()) != num_last )
		return 1;


	return 0;
}