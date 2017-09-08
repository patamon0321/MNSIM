#include <stdlib.h>
#ifndef CALMUX_H_
#define CALMUX_H_

#include "CalculateGate.h"
#include "Technology.h"
#include "constant.h"

class Cal_MUX {
	public:
		int MUX_InputNum,MUX_Controlnum;
		CalGate INV_1IN,NAND_2IN,NAND_3IN,NAND_4IN,NAND_5IN,NAND_6IN,NAND_7IN,NAND_8IN,NAND_9IN,NAND_16IN,NAND_32IN,NAND_64IN,NAND_128IN,NAND_256IN;
		
		Cal_MUX(Technology tech,int inputnum);
		virtual ~Cal_MUX(); 
		double MUX_Area();
		double MUX_Latency();
		double MUX_Power_Dynamic();
};

#endif
		