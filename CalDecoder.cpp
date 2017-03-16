#include"CalDecoder.h"

Cal_Decoder::Cal_Decoder(Technology tech,int functiontype,int xbarnum):INV_1IN(INV,NMOS,1,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech),NAND_2IN(NAND,NMOS,2,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273,tech),NAND_3IN(NAND,NMOS,3,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech),NAND_4IN(NAND,NMOS,4,3*tech.featureSize,3*tech.featureSize,3*tech.featureSize,tech.featureSize*50,40+273.15,tech){
	num_1 = ceil(log((double)xbarnum)/log(2.0));
	num_2 = xbarnum;
	temp1=num_1;
	temp2=num_2;
	function_type= functiontype;
}

Cal_Decoder::~Cal_Decoder(){}

double Cal_Decoder::Decoder_Area(){
	if(num_1>4){
		num_1--;
		num_2=num_2/2;
		return 2*Decoder_Area();
	}
	else if(num_1==4){
		num_1=temp1;
		num_2=temp2;
		return num_2*(NAND_4IN.Area()+INV_1IN.Area()); //enable line is considered which occupies a port of NAND_4IN
	}
	else if(num_1==3){
		num_1=temp1;
		num_2=temp2;
		return num_2*(NAND_3IN.Area()+INV_1IN.Area()); 
	}
	else if(num_1==2){
		num_1=temp1;
		num_2=temp2;
		return num_2*(NAND_2IN.Area()+INV_1IN.Area()); 
	}
}

double Cal_Decoder::Decoder_Latency(){
	if(function_type==1)
		return 0;
	else{
		temp1=num_1;
		if(num_1>4){
			num_1--;
			return 2*Decoder_Area();
		}
		else if(num_1==4){
			num_1=temp1;
			return 2*INV_1IN.Latency()+NAND_4IN.Latency();
		}
		else if(num_1==3){
			num_1=temp1;
			return 2*INV_1IN.Latency()+NAND_3IN.Latency();
		}
		else if(num_1==2){
			num_1=temp1;
			return 2*INV_1IN.Latency()+NAND_2IN.Latency();
		}
	}
}

double Cal_Decoder::Decoder_Power_Leakage(){
	temp1=num_1;
	if(num_1>4){
		num_1--;
		return 2*Decoder_Power_Leakage();
	}
	else if(num_1==4){
		num_1=temp1;
		return 2*INV_1IN.Power_Leakage()+NAND_4IN.Power_Leakage();
	}
	else if(num_1==3){
		num_1=temp1;
		return 2*INV_1IN.Power_Leakage()+NAND_3IN.Power_Leakage();
	}
	else if(num_1==2){
		num_1=temp1;
		return 2*INV_1IN.Power_Leakage()+NAND_2IN.Power_Leakage();
	}
};

double Cal_Decoder::Decoder_Power_Dynamic(){
	temp1=num_1;
	if(num_1>4){
		num_1--;
		return 2*Decoder_Power_Dynamic();
	}
	else if(num_1==4){
		num_1=temp1;
		return 2*INV_1IN.Power_Dynamic()+NAND_4IN.Power_Dynamic();
	}
	else if(num_1==3){
		num_1=temp1;
		return 2*INV_1IN.Power_Dynamic()+NAND_3IN.Power_Dynamic();
	}
	else if(num_1==2){
		num_1=temp1;
		return 2*INV_1IN.Power_Dynamic()+NAND_2IN.Power_Dynamic();
	}
};