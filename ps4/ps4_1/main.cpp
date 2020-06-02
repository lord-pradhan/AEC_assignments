#include "simplebitmap.h"
#include "simplebitmaptemplate.h"
#include <math.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){

	// initialize variables
	FILE* fin = fopen(argv[1], "rb");

	int total_ct = 0;

	int imgSize = 40;

	if (argc == 1) {cout << "Usage: ps4_1 "<< argv[1] << endl;}

	SimpleBitmap inputPng;

	if (!inputPng.LoadPng(fin)){cout<< "Error: Failed to read a .PNG file."<<endl;};

	int x_len = inputPng.GetWidth();
	int y_len = inputPng.GetHeight();

	for ( int j =0; j < ceil( (double) y_len / (double) imgSize ) ; j++ ){

		for (int i =0; i < ceil( (double) x_len / (double) imgSize) ; i++){

			char filename[30];
			sprintf(filename, "%d.PNG", total_ct);
			FILE* fout = fopen(filename, "wb");

			SimpleBitmap newPng = inputPng.CutOut(imgSize*i, imgSize*j, imgSize, imgSize);

			if (!newPng.SavePng(fout)){cout<< "write Failed"<<endl;}
			// else{total_ct++;}

			total_ct++;

			if (total_ct >= 200){break;}

		}

		if (total_ct >= 200){break;}
	}


	return 0;
}