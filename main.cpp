#include<iostream>
#include<fstream>
#include<iomanip>
#include<math.h>
#include<time.h>
#include<string>
#include "InputParameter.h"

using namespace std;

InputParameter *inputParameter;

int main()
{
	clock_t tic,toc;
	tic=clock();

	inputParameter = new InputParameter();
	inputParameter->ReadInputParameterFromFile();
	for(int i=0;i<32;i++)
		cout<<inputParameter->NetScale[i]<<' ';
	toc=clock();
    cout<<"Run time: "<<(double)(toc-tic)/CLOCKS_PER_SEC<<"S"<<endl;
	delete inputParameter;
	return 0;
}
