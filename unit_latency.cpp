#include"global.h"
#include"function.h"
#include"module.h"
double xbar_latency,adda_latency,adder_latency,decoder_latency,write_latency,read_latency,latency_u;
void unit_latency_c(int tech,int celltype,int xbarsize,int adposition,int adderposition,int pulseposition,int action_type,int sig_bit,int read_seperate){
	xbar_latency = cal_xbar_l(tech,celltype,xbarsize,action_type);
    adda_latency = adposition * (cal_ad_l(tech,sig_bit)+ cal_da_l(tech,sig_bit));
    if (action_type == 2)
        adder_latency = adderposition * cal_adder_l(tech,sig_bit);
    else
        adder_latency = 0;
    decoder_latency = cal_decoder_l(tech,xbarsize,celltype,action_type);
    if (action_type == 0)
        write_latency = pulseposition * 1.6e-9;
    else
        write_latency = 0;
    read_latency = cal_read_l(tech,xbarsize,action_type);
    
    latency_u = xbar_latency + adda_latency + adder_latency + decoder_latency + write_latency + read_latency;
    
    flags[0] = 1;
    
    if (read_seperate > 0)
        latency_u =  xbar_latency + cal_da_l(tech,sig_bit) +  write_latency+(adder_latency + decoder_latency + read_latency * ceil((double)xbarsize/read_seperate));
}