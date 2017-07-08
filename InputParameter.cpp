#include "InputParameter.h"
#include<fstream>
#include<string>
#include <sstream>

InputParameter::InputParameter() {
	// TODO Auto-generated constructor stub
	AppScale = 0;
	rs = 3000;   
	cell_bit = 4;
	sig_bit = 6;
	rramtech = 65e-9;
	minXbarSize = 256;
	maxXbarSize = 256;
	minCell = 1;
	maxCell = 1;
	minAdPos = 1;
	maxAdPos = 1;
	minBtLv = 0;
	maxBtLv = 16;
	minRRang = 1000;
	maxRRang = 1000000;
	minAdder = 0;
	maxAdder = 0;
	minLine = 22;
	maxLine = 90;
}
InputParameter::~InputParameter() {
	// TODO Auto-generated destructor stub
}
void InputParameter::ReadInputParameterFromFile(){
    ifstream fin;
    int i = 0;						
	int j = 0;
	string temp;	
	char filename[]={"SimConfig.txt"};
    fin.open(filename,ios::in);
	if(!fin.is_open())
		cout<<"SimConfig.txt cannot be found!"<<endl;
    while(fin.good() && i < 20){            //将txt文件存入到cfgtxt[]中，cfgtxt[]为string数组
		if(i<4)								//不读前4行
			getline(fin,temp);
        else{
			getline(fin,temp);
			if(temp=="")					//不读空行
				continue;
			cfgtxt[i-4]=temp;
			}
		i++;
	}
	for(i=0;i<16;i++){						//这里目的是将string数组中的信息分别赋给变量
        if(cfgtxt[i].find("Simulation_Level")!=string::npos)      //find函数起字符串匹配作用
            SimLv.assign(cfgtxt[i],sizeof("Simulation_Level"),cfgtxt[i].length()-sizeof("Simulation_Level"));   //assign函数起复制一段子串的作用
        else if(cfgtxt[i].find("Target_Outputs")!=string::npos)
            Target_Output.assign(cfgtxt[i],sizeof("Target_Outputs"),cfgtxt[i].length()-sizeof("Target_Outputs"));
        else if(cfgtxt[i].find("Application ")!=string::npos)
            Application.assign(cfgtxt[i],sizeof("Application"),cfgtxt[i].length()-sizeof("Application"));
        else if(cfgtxt[i].find("Action_Type")!=string::npos)
            Action_type.assign(cfgtxt[i],sizeof("Action_Type"),cfgtxt[i].length()-sizeof("Action_Type"));
        else if(cfgtxt[i].find("Application_Scale")!=string::npos){
            temp.assign(cfgtxt[i],sizeof("Application_Scale"),temp.length()-sizeof("Application_Scale")); 
			stringstream stream(temp);					//这里讲string转换为int
			stream>>AppScale;
		}
        else if(cfgtxt[i].find("Network_Scale")!=string::npos){
			NetScale=new int[AppScale*2];
			for(int ii=0;ii<AppScale*2;ii++)
				NetScale[ii]=0;
			for (int ii = sizeof("Network_Scale"); ii != cfgtxt[i].length(); ++ii)
				if (cfgtxt[i].at(ii)>='0' && cfgtxt[i].at(ii)<='9')
					NetScale[j] =cfgtxt[i].at(ii)-'0'+NetScale[j]*10;
				else
					j++;
			}
        else if(cfgtxt[i].find("Weight_Polarity")!=string::npos){
            temp.assign(cfgtxt[i],sizeof("Weight_Polarity"),temp.length()-sizeof("Weight_Polarity")); 
			stringstream stream(temp);
			stream>>WeightPolar;
		}
		else if(cfgtxt[i].find("Cell_Type")!=string::npos){
            temp.assign(cfgtxt[i],sizeof("Cell_Type"),temp.length()-sizeof("Cell_Type")); 
			stringstream stream(temp);
			stream>>minCell;
			maxCell=minCell;
		}
		else if(cfgtxt[i].find("RRAM_Model")!=string::npos)
            RRAMModel.assign(cfgtxt[i],sizeof("RRAM_Model"),cfgtxt[i].length()-sizeof("RRAM_Model"));
		else if(cfgtxt[i].find("RRAM_Bit_Levels")!=string::npos){
            temp.assign(cfgtxt[i],sizeof("RRAM_Bit_Levels"),temp.length()-sizeof("RRAM_Bit_Levels")); 
			stringstream stream(temp);
			stream>>minBtLv;
			maxBtLv=minBtLv;
		}
		else if(cfgtxt[i].find("Transistor_Tech(nm)")!=string::npos){
            temp.assign(cfgtxt[i],sizeof("Transistor_Tech(nm)"),temp.length()-sizeof("Transistor_Tech(nm)")); 
			stringstream stream(temp);
			stream>>TranTech;
		}
		else if(cfgtxt[i].find("Line_Tech")!=string::npos){
            temp.assign(cfgtxt[i],sizeof("Line_Tech"),temp.length()-sizeof("Line_Tech")); 
			stringstream stream(temp);
			stream>>minLine;
			stream>>maxLine;
		}
		else if(cfgtxt[i].find("Min_Crossbar_Size")!=string::npos){
            temp.assign(cfgtxt[i],sizeof("Min_Crossbar_Size"),temp.length()-sizeof("Min_Crossbar_Size")); 
			stringstream stream(temp);
			stream>>minXbarSize;
		}
		else if(cfgtxt[i].find("Max_Crossbar_Size")!=string::npos){
            temp.assign(cfgtxt[i],sizeof("Max_Crossbar_Size"),temp.length()-sizeof("Max_Crossbar_Size")); 
			stringstream stream(temp);
			stream>>maxXbarSize;
		}
        else if(cfgtxt[i].find("Write_Method")!=string::npos)
            WriteMth.assign(cfgtxt[i],sizeof("Write_Method"),cfgtxt[i].length()-sizeof("Write_Method"));
	}
}


