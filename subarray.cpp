#include"subarray.h"
Cal_Subarray::Cal_Subarray(Technology tech,double width_temp, double height_temp,double celltype_temp,double xbarsize_temp){
	Tech=tech;
	width=width_temp;
	height=height_temp;
	celltype=celltype_temp;
	xbarsize=xbarsize_temp;
}

Cal_Subarray::~Cal_Subarray(){}

double Cal_Subarray::Subarray_Area(){
	if(celltype==0)
		return xbarsize*xbarsize*((3*(3+1)*Tech.featureSize*Tech.featureSize));
	else
		return xbarsize*xbarsize*4*Tech.featureSize*Tech.featureSize;
}

double Cal_Subarray::Subarray_Latency(double _rampInput) {
	 return 100*1e-9;
}