/* ////////////////////////////////////////////////////////////

File Name: main.cpp
Copyright (c) 2017 Soji Yamakawa.  All rights reserved.
http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////// */

#include <fslazywindow.h>
#include <iostream>
#include <simplebitmap.h>
#include "hashbase.h"
#include "hashtable.h"
#include <math.h>
#include <stdio.h>
// #include "hashset.h"
// #include <simplbitmaptemplate.h>

using namespace std;

template<class SimpleBitmap>
HashCommon::CodeType HashBase<SimpleBitmap>::HashCode(const SimpleBitmap &key) const
{
	HashCommon::CodeType code = 1;
	for (int i =0; i< key.GetTotalNumComponent(); i++){
		
		if (i%5 == 0)
			code+= 2 *( *(key.GetBitmapPointer()+i ) );
		else if (i%5==1)
			code+= 3 *( *(key.GetBitmapPointer()+i) );
		else if (i%5==2)
			code+= 5 *( *(key.GetBitmapPointer()+i) );
		else if (i%5==3)
			code+= 7 *( *(key.GetBitmapPointer()+i) );
		else if (i%5==4)
			code+= 11*( *(key.GetBitmapPointer()+i) );
	}

	return code;
}

class FsLazyWindowApplication : public FsLazyWindowApplicationBase
{
protected:
	bool needRedraw;
	int wid_window = 1200;
	int hei_window = 800;
	HashTable<SimpleBitmap, int> lookUp;

public:
	FsLazyWindowApplication();
	virtual void BeforeEverything(int argc,char *argv[]);
	virtual void GetOpenWindowOption(FsOpenWindowOption &OPT) const;
	virtual void Initialize(int argc,char *argv[]);
	virtual void Interval(void);
	virtual void BeforeTerminate(void);
	virtual void Draw(void);
	virtual bool UserWantToCloseProgram(void);
	virtual bool MustTerminate(void) const;
	virtual long long int GetMinimumSleepPerInterval(void) const;
	virtual bool NeedRedraw(void) const;
};

FsLazyWindowApplication::FsLazyWindowApplication()
{
	needRedraw=false;
}

/* virtual */ void FsLazyWindowApplication::BeforeEverything(int argc,char *argv[])
{
}

/* virtual */ void FsLazyWindowApplication::GetOpenWindowOption(FsOpenWindowOption &opt) const
{
	opt.x0=0;
	opt.y0=0;
	opt.wid=wid_window;
	opt.hei=hei_window;
}

/* virtual */ void FsLazyWindowApplication::Initialize(int argc,char *argv[])
{

	FILE* fin = fopen(argv[1], "rb");

	if (argc == 1) {cout << "Usage: ps4_1 "<< argv[1] << endl;}

	SimpleBitmap inputPng;
	if (!inputPng.LoadPng(fin)){cout<< "Error: Failed to read a .PNG file."<<endl;};

	// **** begin the hash table processes ****

	//initialize variables
	int total_ct = 0;
	int imgSize = 40;
	int id_num=0;

	// cout<< "Key: "<< *(inputPng.GetBitmapPointer()+200) << endl;

	// begin loop
	int x_len = inputPng.GetWidth();
	int y_len = inputPng.GetHeight();

	for ( int j =0; j < ceil( (double) y_len / (double) imgSize ) ; j++ ){

		for (int i =0; i < ceil( (double) x_len / (double) imgSize) ; i++){

			SimpleBitmap newPng = inputPng.CutOut(imgSize*i, imgSize*j, imgSize, imgSize);
			newPng.Invert();
			// wid_cut = newPng.GetWidth();
			// hei_cut = newPng.GetHeight();

			if (!lookUp.IsIncluded(newPng)){

				lookUp.Update(newPng, id_num);

				printf("%c",' ' + id_num);
				id_num++;
				// cout<<id_num;
			}

			else{
				// cout<< *( (lookUp[newPng])->GetBitmapPointer()+2 )  << endl;

				printf("%c", ' '+ *lookUp[newPng]);
			}

			total_ct++;

			// if (total_ct >= 2000){break;}

			// if (total_ct%30 == 0){printf("\n");}
		}
		printf(" \n");
		// if (total_ct >= 2000){break;}
	}

	// cout<<"id_num: "<< id_num<<endl;

}


/* virtual */ void FsLazyWindowApplication::Interval(void)
{
	auto key=FsInkey();
	if(FSKEY_ESC==key)
	{
		SetMustTerminate(true);
	}


	needRedraw=true;
}

/* virtual */ void FsLazyWindowApplication::Draw(void)
{	
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	auto x_init = 0; auto y_init = 40;

	for (auto hd = lookUp.First(); hd.IsNotNull()==true; hd=lookUp.Next(hd) ){

		auto bitmap = lookUp[hd];
		glRasterPos2d(x_init,y_init);
		glDrawPixels( bitmap->GetWidth(), bitmap->GetHeight(),GL_RGBA ,GL_UNSIGNED_BYTE , bitmap->GetBitmapPointer() );

		if(x_init + bitmap->GetWidth() < wid_window){
			x_init += 40;
			// cout<<"Entered 1st condition"<<endl;
		}

		else{
			x_init=0;
			y_init+=40;
			// cout<<"Entered 2nd condition"<<endl;
		}

	}

	FsSwapBuffers();
	needRedraw=false;
}

/* virtual */ bool FsLazyWindowApplication::UserWantToCloseProgram(void)
{
	return true; // Returning true will just close the program.
}

/* virtual */ bool FsLazyWindowApplication::MustTerminate(void) const
{
	return FsLazyWindowApplicationBase::MustTerminate();
}
/* virtual */ long long int FsLazyWindowApplication::GetMinimumSleepPerInterval(void) const
{
	return 10;
}
/* virtual */ void FsLazyWindowApplication::BeforeTerminate(void)
{
}
/* virtual */ bool FsLazyWindowApplication::NeedRedraw(void) const
{
	return needRedraw;
}


static FsLazyWindowApplication *appPtr=nullptr;

/* static */ FsLazyWindowApplicationBase *FsLazyWindowApplicationBase::GetApplication(void)
{
	if(nullptr==appPtr)
	{
		appPtr=new FsLazyWindowApplication;
	}
	return appPtr;
}


