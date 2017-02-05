#include<iostream>
#ifndef FORMULA_H_
#define FORMULA_H_

#define MAX(a,b) (((a)> (b))?(a):(b))
#define MIN(a,b) (((a)< (b))?(a):(b))

void determin_sig(int xbarsize,int adderposition,int sig_bit,int cell_bit,int adposition);
void determin_net(int xbarsize,int rowsize,int columnsize,int signalsize);
int calWL(int tech);
double adsize(int tech,int sig_bit);
double dasize(int tech,int sig_bit);
double addersize(int tech,int sig_bit);
double pulsesize(int tech);
double bl_decoder_size(int tech,int celltype,int xbarsize);
double sl_decoder_size(int tech,int celltype,int xbarsize);
double wl_decoder_size(int tech,int celltype,int xbarsize);
double readsize(int tech);
double writesize(int tech,int celltype);

//global variable
int control=2;
double input_sig;
double output_neighbor;
double output_sig;
double signalsize;
int netcolumn;
int netrow;
double bandwidth;
double flags[4];
double xbar_area,techwl,ad_area,da_area,adder_area,pulse_area,decoder_area,read_area,write_area,Sub_area,area_u;

#endif