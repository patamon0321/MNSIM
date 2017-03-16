#ifndef CALDECODER_H_
#define CALDECODER_H_

#include "CalculateGate.h"
#include "technology.h"
#include <stdlib.h>
#include <math.h>
#include "constant.h"

class Cal_Decoder {
	public:
		int num_1;//bit of input
		int num_2;//bit of output
		int temp1,temp2;
		int function_type;
		CalGate INV_1IN,NAND_2IN,NAND_3IN,NAND_4IN;

		Cal_Decoder(Technology tech,int function_type,int xbarnum);//tech determines widthnoms£¬widthpmos
		virtual ~Cal_Decoder(); 
		double Decoder_Area();
		double Decoder_Latency();
		double Decoder_Power_Leakage();
		double Decoder_Power_Dynamic();
};

#endif