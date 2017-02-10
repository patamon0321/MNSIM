#include<iostream>
#include<fstream>
#include<iomanip>
#include<math.h>
#include<time.h>
#include<string>
#include"InputParameter.h"
#include"formula.h"
#include"global.h"
#include"function.h"

using namespace std;

InputParameter *inputParameter;
int count = 0;
double minarea = 8e10;
double minenergy = 8e10;
double minpower = 8e10;
double minlatency = 8e10;
double minerr = 8e10;
double area,area_flags,latency,latency_multi,power_multi,power_flags,area_multi,power,energy,application,action_type,target;
double pulseposition,cell_bit,lantency_multi,AAestrslt[17000][16];
int layer_computation_times[] = {49284,49284,12100,12100,2916,2916,2916,676,676,676,144,144,144,1,1,1};
double mintarget[6]={0,minarea,minenergy,minlatency,minerr,minpower},AAAestrslt[1100][16];
int mincount[6];
double tech;
int err_count;

int main()
{
	clock_t tic,toc;
	tic=clock();
	inputParameter = new InputParameter();
	tech = inputParameter->TranTech;
	inputParameter->ReadInputParameterFromFile();
	if (inputParameter->Application[0] == 'a' ||inputParameter->Application[0] == 'A')
        application = 0;
    else
        application = 1;
	if (inputParameter->Action_type[0] == 'c' || inputParameter->Action_type[0] == 'C') //calculate phase
       action_type = 2;
	switch (inputParameter->Target_Output[0]){
		case 'a':
				target = 1;break;
		case 'A':
				target = 1;break;
		case 'p':
				target = 2;break;
		case 'P':
				target = 2;break;
		case 'l':
				target = 3;break;
		case 'L':
				target = 3;break;
	}
    for (double adposition=max(0,inputParameter->minAdPos);adposition<=min(1,inputParameter->maxAdPos);adposition++)
        for (double bit_level = max(0,inputParameter->minBtLv);(bit_level<=min(16,inputParameter->maxBtLv));bit_level++)
//             for min_resis_range = 500:500:1e3
//                 resis_range(1) =  min_resis_range
//                 for max_resis_range = [100e3,500e3,100e4]
//                     resis_range(2) = max_resis_range
            for (double adderposition = max(0,inputParameter->minAdder);adderposition<=min(1,inputParameter->maxAdder);adderposition++)
                //for tech = 45/*[18,22,28,36,45,65,90,130];max(22,minLine):min(22,maxLine)*/
//             tech = linetech;
                    for (double linetech = 90;linetech<=90;linetech++)//[18,22,28,36,45,65,90];
//         for celltype = max(0,minCell):min(1,maxCell)
                        for (double celltype = 1;celltype<=1;celltype++){
//             for pulseposition = 0:1
                            pulseposition = 0;
                
                            for (double xbarsize = 128;xbarsize<=1024;xbarsize+=128)//[4,8,16,32,64,128,256]%,512,1024]
                                for (double read_sep = 1;read_sep<=128;read_sep++)//xbarsize/128 : xbarsize/128 : xbarsize%8:8:xbarsize
                                    if (xbarsize<inputParameter->minXbarSize || xbarsize > inputParameter->maxXbarSize)
                                        cout<<"error:xbarsize over the limit"<<endl;
                                    else{
										double input_err[200]={0};
										for (double netlevel = 1;netlevel<=inputParameter->AppScale;netlevel++){ //-1
											if (bit_level != 0)
												cell_bit = bit_level;
											determin_sig(xbarsize,adderposition,inputParameter->sig_bit,cell_bit,adposition);
											determin_net(xbarsize,inputParameter->NetScale[2*(int)netlevel-1-1],inputParameter->NetScale[2*(int)netlevel-1],signalsize);
											unit_area_c(tech,celltype,xbarsize,adposition,adderposition,pulseposition,inputParameter->sig_bit,application,inputParameter->rramtech,read_sep);
											unit_latency_c(tech,celltype,xbarsize,adposition,adderposition,pulseposition,action_type,inputParameter->sig_bit,read_sep);
											unit_power_c(tech,celltype,xbarsize,adposition,adderposition,pulseposition,action_type,inputParameter->sig_bit,application,inputParameter->maxRRang,netrow,xbar_latency,adda_latency,adder_latency,decoder_latency,write_latency,read_latency,read_sep);
											periphery_area(tech,xbarsize, netrow, netcolumn, adderposition,pulseposition,inputParameter->sig_bit,application);
											periphery_latency_c( tech,netrow, adderposition,pulseposition,inputParameter->sig_bit,application);
										    periphery_power_c(tech,xbarsize, netrow, netcolumn, adderposition,pulseposition,inputParameter->sig_bit,application,adders_latency,neuron_latency,pulse_latency);
											accuracy_c(xbarsize,linetech,inputParameter->sig_bit,cell_bit,inputParameter->maxRRang,input_err[(int)netlevel-1]);
											input_err[(int)netlevel] = accuracy;
											
											area = area_u * netrow * netcolumn + area_l + area_p ;//+ area_r + area_w;
											area_flags = area_flags * netrow * netcolumn;
											if (adderposition == 0){
												energy = power_u * latency_u * netrow * netcolumn + power_l * latency_l + power_p * latency_p;// + power_r + power_w;
												latency = latency_u + latency_l + latency_p;
												latency_multi = latency_u;
											}
											else{
												energy = power_u * latency_u * netrow * netcolumn + power_l * latency_l + power_p * latency_p;
												latency = latency_u * netrow + latency_l + latency_p;
												lantency_multi = latency_u * netrow;
											}
											power_multi = power_u * netrow * netcolumn;
											power_flags = power_flags * netrow * netcolumn;
											area_multi = area_u * netrow * netcolumn;
											power = power_u  * netrow * netcolumn + power_l  + power_p ;
											energy = power * latency;
											equal(netlevel,area,energy,latency,power,accuracy,area_multi,power_multi,latency_multi,read_sep,adposition,bit_level,adderposition,pulseposition,linetech,celltype,xbarsize);

											if (accuracy < minerr){
												minerr = accuracy;
												err_count = count;
											}
											count = count + 1;
										}
									}
						}
    int design_space = count/inputParameter->AppScale;    
	for (int temp_count = 1;temp_count<design_space;temp_count++){
		for(int netlevel_temp=1;netlevel_temp<inputParameter->AppScale;netlevel_temp++){
			for(int i=0;i<16;i++)
				AAAestrslt[temp_count][i] = AAAestrslt[temp_count][i]+AAestrslt[((temp_count-1) * inputParameter->AppScale +netlevel_temp)][i];
			AAAestrslt[temp_count][3] =AAAestrslt[temp_count][3] + AAestrslt[((temp_count-1) * inputParameter->AppScale +netlevel_temp)][3]*(layer_computation_times[netlevel_temp]-1);
		}
                                        
		//AAAestrslt(temp_count,3) =AAestrslt(((temp_count-1) * AppScale +1),3)*layer_computation_times(1);
		AAAestrslt[temp_count][3] = AAAestrslt[temp_count][3]/inputParameter->AppScale;
		AAAestrslt[temp_count][5] = AAAestrslt[temp_count][5]/inputParameter->AppScale;
		AAAestrslt[temp_count][6] = AAAestrslt[temp_count][6]/inputParameter->AppScale;
		for(int i=10;i<16;i++)
			AAAestrslt[temp_count][9] = AAAestrslt[temp_count][9]/inputParameter->AppScale;

		AAAestrslt[temp_count][4] = max1(((temp_count-1) * inputParameter->AppScale+1),((temp_count-1) * inputParameter->AppScale+inputParameter->AppScale),4);

		for (int target_tt=2;target_tt<=6;target_tt++)
            if (AAAestrslt[temp_count][6]>1)
                break;
            else if (AAAestrslt[temp_count][target_tt] < mintarget[target_tt]){
                mintarget[target_tt] = AAAestrslt[temp_count][target_tt];
                mincount[target_tt] = temp_count;
			}
	}
	double optresult[10][16];
	if (count>2){
		for(int i=2;i<=6;i++)
			for(int j=1;j<16;j++)
				optresult[i-2][j] = AAAestrslt[mincount[i]][j];
		for(int i=0;i<5;i++)
			for(int j=1;j<=3;j++)
				optresult[i][j] = optresult[i][j]*1e6;	
	}
	toc=clock();
    cout<<"Run time: "<<(double)(toc-tic)/CLOCKS_PER_SEC<<"S"<<endl;
	delete inputParameter;
	return 0;
}

void equal(double netlevel,double area,double energy,double latency,double power,double accuracy,double area_multi,double power_multi,double latency_multi,double read_sep,double adposition,double bit_level,double adderposition,double pulseposition,double linetech,double celltype,double xbarsize){
	AAestrslt[count][0] = netlevel;
	AAestrslt[count][1] = area;
	AAestrslt[count][2] = energy;
	AAestrslt[count][3] = latency;
	AAestrslt[count][4] = power;
	AAestrslt[count][5] = accuracy;
	AAestrslt[count][6] = area_multi;
	AAestrslt[count][7] = power_multi;
	AAestrslt[count][8] = latency_multi;
	AAestrslt[count][9] = read_sep;
	AAestrslt[count][10] = adposition;
	AAestrslt[count][11] = bit_level;
	AAestrslt[count][12] = adderposition;
	AAestrslt[count][13] = pulseposition;
	AAestrslt[count][14] = linetech;
	AAestrslt[count][15] = celltype;
	AAestrslt[count][16] = xbarsize;
}
double max1(int a,int b,int c){
	double i;
	int j;
	i=AAestrslt[b][4];
	for(j=b;j<a;j++)
		if(AAestrslt[j][4]>i)
			i=AAestrslt[j][4]>i;
	return i;
}

