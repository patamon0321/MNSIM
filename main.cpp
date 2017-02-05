#include<iostream>
#include<fstream>
#include<iomanip>
#include<math.h>
#include<time.h>
#include<string>
#include"InputParameter.h"
#include"formula.h"

using namespace std;

InputParameter *inputParameter;

int main()
{
	clock_t tic,toc;
	tic=clock();
	inputParameter = new InputParameter();
	inputParameter->ReadInputParameterFromFile();

	toc=clock();
    cout<<"Run time: "<<(double)(toc-tic)/CLOCKS_PER_SEC<<"S"<<endl;
	delete inputParameter;
	return 0;
}
