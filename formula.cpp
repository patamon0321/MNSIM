#include"formula.h"
#include<math.h>
void determin_sig(int xbarsize,int adderposition,int sig_bit,int cell_bit,int adposition){
	control = 2 ; //如果adder在里边，表示需要额外输入一列信号，输出一列信号
    input_sig = 2*(log((double)xbarsize)/log(2.0)) + sig_bit*xbarsize + control+ adderposition * (sig_bit+cell_bit);
    flags[0] = 1;
    output_neighbor = (sig_bit+cell_bit)*adposition;
    output_sig = cell_bit + (sig_bit+cell_bit) * (1-adposition) + flags[0];//输出到相邻块+输出到外部+输出flag
    signalsize = input_sig + output_sig + output_neighbor;
}

void determin_net(int xbarsize,int rowsize,int columnsize,int signalsize){
	netcolumn = ceil((double)columnsize/xbarsize);
    netrow = ceil((double)rowsize/xbarsize);
    bandwidth = signalsize * netrow;
}

int calWL(int tech){
	switch(tech){
	case 130:
            return 6; break;
	case 90:
            return 6; break;
    otherwise:
            return 6; break;
	}
}

double adsize(int tech,int sig_bit){
	if (tech<0)
        return (abs(tech) * 1e-9)*(abs(tech) * 1e-9) *18   * sig_bit * 20;
    else 
        return (abs(tech) * 1e-9)*(abs(tech) * 1e-9) *6   * 2800 * sig_bit/8;
}

double dasize(int tech,int sig_bit){
	if (tech<0 )
        return (abs(tech)*1e-9)*(abs(tech)*1e-9)*18*sig_bit*20;
    else
        return (abs(tech)*1e-9)*(abs(tech)*1e-9)*6*3096/8*sig_bit/8;
}

double addersize(int tech,int sig_bit){
	if (tech<0)
        return (abs(tech) * 1e-9)*(abs(tech) * 1e-9)*18*sig_bit*16;
    else
        return 296e-6 * 1e-6 *(tech/65)*(tech/65);
}

double pulsesize(int tech){
	if(tech<0)
        return (tech * 1e-9)*(tech * 1e-9) * 18 * 20;
    else
        return 182e-6 * 65e-6 * (tech/130)*(tech/130);
}

double bl_decoder_size(int tech,int celltype,int xbarsize){
	double L; int m,p,k;
	if (tech<0)
		if (celltype == 1)
			return(tech *1e-9)*(tech *1e-9) * 18 *25 * xbarsize;
	else{
	//	    if celltype == 1
			L = tech * 1e-9;
			m = ceil(log((double)xbarsize)/log(2.0));
			switch (m) {
				case 2:
					k = 0;
					p = 2; 
					break;
				case 3:
					k = 0;
					p = 3;
					break;
				case 4:
					k = 0;
					p = 4;
					break;
				case 5:
					k = 2;
					p = 3;
					break;
				case 6:
					k = 3;
					p = 3;
					break;
				case 7:
					k = 3;
					p = 4;
					break;
				case 8:
					k = 4;
					p = 4;
					break;
				case 9:
					k = 4;
					p = 5;
					break;
				case 10:
					k = 5;
					p = 5;
					break;
				otherwise:
					;
			}
			return(1+1+2+2+2+1+1+1+8+8+2+1+60+30+10)*L*L*m+(1+2+4+8+16+32)*L*L*k+(2*k+k*k*k+1+2+4+8+16+32) *L*L*k+(1+2+4+8+16+32)*L*L*p + (2*p+p*p*p+1+2+4+8+16+32) *L*L*p;
	}
}
double sl_decoder_size(int tech,int celltype,int xbarsize){
	return 0;
}
double wl_decoder_size(int tech,int celltype,int xbarsize){
	return bl_decoder_size(tech,celltype,xbarsize);
}
double readsize(int tech){
	if(tech<0)
        return (tech * 1e-9)*(tech * 1e-9) * 18  * 20;
    else
        return (tech * 1e-9)*(tech * 1e-9) * 200;
}
double writesize(int tech,int celltype){
	return (tech * 1e-9)*(tech * 1e-9) *18 *2;
}