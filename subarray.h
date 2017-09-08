#include <stdlib.h>
#ifndef SUBARRY_H_
#define SUBARRAY_H_

#include "CalculateGate.h"
#include "Technology.h"
#include "constant.h"
#include "formula.h"
#include <math.h>

class Cal_Subarray{
	public:
		//double width,height;
		Technology Tech;
		double celltype;
		double xbarsize;
		double readLatency;
		double writelatency;
		Cal_Subarray(Technology tech,/*double width_temp, double height_temp,*/double celltype_temp,double xbar_temp);
		virtual ~Cal_Subarray(); 
		double Subarray_Area();
		void Subarray_Latency(double _rampInput);
		double Subarray_Power();
};

#endif