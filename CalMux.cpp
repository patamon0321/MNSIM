#include "CalMux.h"

Cal_MUX::Cal_MUX(Technology tech,int inputnum):
INV_1IN(INV,NMOS,1,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech),
NAND_2IN(NAND,NMOS,2,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech),
NAND_3IN(NAND,NMOS,3,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech),
NAND_4IN(NAND,NMOS,4,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech),
NAND_5IN(NAND,NMOS,5,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech),
NAND_6IN(NAND,NMOS,6,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech),
NAND_7IN(NAND,NMOS,7,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech),
NAND_8IN(NAND,NMOS,8,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech),
NAND_9IN(NAND,NMOS,9,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech),
NAND_16IN(NAND,NMOS,16,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech),
NAND_32IN(NAND,NMOS,32,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech),
NAND_64IN(NAND,NMOS,64,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech),
NAND_128IN(NAND,NMOS,128,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech),
NAND_256IN(NAND,NMOS,256,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech)
{
	MUX_InputNum = inputnum;
	MUX_Controlnum = ceil(log((double)MUX_InputNum)/log(2.0));
}

Cal_MUX::~Cal_MUX(){}

double Cal_MUX::MUX_Area(){
	if(MUX_InputNum==4) return MUX_Controlnum*INV_1IN.Area()+MUX_InputNum*NAND_3IN.Area()+NAND_4IN.Area();
	if(MUX_InputNum==8) return MUX_Controlnum*INV_1IN.Area()+MUX_InputNum*NAND_4IN.Area()+NAND_8IN.Area();
	if(MUX_InputNum==16) return MUX_Controlnum*INV_1IN.Area()+MUX_InputNum*NAND_5IN.Area()+NAND_16IN.Area();
	if(MUX_InputNum==32) return MUX_Controlnum*INV_1IN.Area()+MUX_InputNum*NAND_6IN.Area()+NAND_32IN.Area();
	if(MUX_InputNum==64) return MUX_Controlnum*INV_1IN.Area()+MUX_InputNum*NAND_7IN.Area()+NAND_64IN.Area();
	if(MUX_InputNum==128) return MUX_Controlnum*INV_1IN.Area()+MUX_InputNum*NAND_8IN.Area()+NAND_128IN.Area();
	if(MUX_InputNum==256) return MUX_Controlnum*INV_1IN.Area()+MUX_InputNum*NAND_9IN.Area()+NAND_256IN.Area();
}
double Cal_MUX::MUX_Latency(){
	if(MUX_InputNum==4) return INV_1IN.Latency()+NAND_3IN.Latency()+NAND_4IN.Latency();
	if(MUX_InputNum==8) return INV_1IN.Latency()+NAND_4IN.Latency()+NAND_8IN.Latency();
	if(MUX_InputNum==16) return INV_1IN.Latency()+NAND_5IN.Latency()+NAND_16IN.Latency();
	if(MUX_InputNum==32) return INV_1IN.Latency()+NAND_6IN.Latency()+NAND_32IN.Latency();
	if(MUX_InputNum==64) return INV_1IN.Latency()+NAND_7IN.Latency()+NAND_64IN.Latency();
	if(MUX_InputNum==128) return INV_1IN.Latency()+NAND_8IN.Latency()+NAND_128IN.Latency();
	if(MUX_InputNum==256) return INV_1IN.Latency()+NAND_9IN.Latency()+NAND_256IN.Latency();
}
double Cal_MUX::MUX_Power_Dynamic(){
	if(MUX_InputNum==4) return MUX_Controlnum*INV_1IN.Power_Dynamic()+MUX_InputNum*NAND_3IN.Power_Dynamic()+NAND_4IN.Power_Dynamic();
	if(MUX_InputNum==8) return MUX_Controlnum*INV_1IN.Power_Dynamic()+MUX_InputNum*NAND_4IN.Power_Dynamic()+NAND_8IN.Power_Dynamic();
	if(MUX_InputNum==16) return MUX_Controlnum*INV_1IN.Power_Dynamic()+MUX_InputNum*NAND_5IN.Power_Dynamic()+NAND_16IN.Power_Dynamic();
	if(MUX_InputNum==32) return MUX_Controlnum*INV_1IN.Power_Dynamic()+MUX_InputNum*NAND_6IN.Power_Dynamic()+NAND_32IN.Power_Dynamic();
	if(MUX_InputNum==64) return MUX_Controlnum*INV_1IN.Power_Dynamic()+MUX_InputNum*NAND_7IN.Power_Dynamic()+NAND_64IN.Power_Dynamic();
	if(MUX_InputNum==128) return MUX_Controlnum*INV_1IN.Power_Dynamic()+MUX_InputNum*NAND_8IN.Power_Dynamic()+NAND_128IN.Power_Dynamic();
	if(MUX_InputNum==256) return MUX_Controlnum*INV_1IN.Power_Dynamic()+MUX_InputNum*NAND_9IN.Power_Dynamic()+NAND_256IN.Power_Dynamic();
};