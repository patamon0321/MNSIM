/*#include"formula.h"
void unit_area_c(int tech,int celltype,int xbarsize,int adposition,int adderposition,int pulseposition,int sig_bit,int application,int rramtech,int read_seperate){
	if (read_seperate == 0){
		if (celltype == 0)
			xbar_area = xbarsize^2 * 4 * rramtech^2;
		else{
			techwl=calWL(tech);
			xbar_area = xbarsize*xbarsize * (3 * techwl +1) * rramtech*rramtech;
		}
		ad_area = adposition * (adsize(tech,sig_bit)) * xbarsize;
		da_area = adposition * (dasize(tech,sig_bit)) * xbarsize;
		adder_area = adderposition * addersize(tech,sig_bit) * xbarsize;
		pulse_area = pulseposition * pulsesize(tech);
		if (celltype == 0)
			decoder_area = bl_decoder_size(tech,celltype,xbarsize) + sl_decoder_size(tech,celltype,xbarsize);
		else
			decoder_area = bl_decoder_size(tech,celltype,xbarsize) + sl_decoder_size(tech,celltype,xbarsize) + wl_decoder_size(tech,celltype,xbarsize);
		read_area = readsize(tech) * xbarsize;
		write_area = writesize(tech,celltype);
		if (application ==0){
			Sub_area = addersize(tech,sig_bit) * xbarsize;
			area_u = xbar_area*2 + ad_area*2 + da_area + adder_area + pulse_area + decoder_area +read_area + write_area + Sub_area;
			flags[0]=xbar_area*2;flags[1]=ad_area*2;flags[2]=da_area+read_area;flags[3]=adder_area+Sub_area+pulse_area+decoder_area+write_area;
			//    flags = 1;
		}
		else{
			area_u = xbar_area + ad_area + adder_area + pulse_area + decoder_area +read_area + write_area;//adda_area未定义，改成了ad_area
			flags[0] = 1;
		}
	}
	else{		// seperate
		if (celltype == 0)
			xbar_area = xbarsize^2 * 4 * rramtech^2;
		else{
			techwl=calWL(tech);
			xbar_area = xbarsize*xbarsize * (3 * techwl +1) * rramtech*rramtech;
		}
		ad_area = adposition * (adsize(tech,sig_bit)) *read_seperate;
		da_area = adposition * (dasize(tech,sig_bit)) * xbarsize;
		adder_area = adderposition * addersize(tech,sig_bit) *read_seperate;
		pulse_area = pulseposition * pulsesize(tech);
		if (celltype == 0)
			decoder_area = bl_decoder_size(tech,celltype,xbarsize) + sl_decoder_size(tech,celltype,xbarsize);
		else
			decoder_area = bl_decoder_size(tech,celltype,xbarsize) + sl_decoder_size(tech,celltype,xbarsize) + wl_decoder_size(tech,celltype,xbarsize);
		read_area = readsize(tech) *read_seperate;
		write_area = writesize(tech,celltype);
    
		if (application ==0){
			Sub_area = addersize(tech,sig_bit)*read_seperate ;
			area_u = xbar_area*2 + ad_area*2 + da_area + adder_area + pulse_area + decoder_area +read_area + write_area + Sub_area;
			flags[0]=xbar_area*2;flags[1]=ad_area*2;flags[2]=da_area+read_area;flags[3]=adder_area+Sub_area+pulse_area+decoder_area+write_area;
		}
		else{
			area_u = xbar_area + ad_area + adder_area + pulse_area + decoder_area +read_area + write_area;
			flags[0] = 1;
		}
	}
}