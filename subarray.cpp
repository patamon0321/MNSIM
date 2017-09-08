#include"subarray.h"
Cal_Subarray::Cal_Subarray(Technology tech,double celltype_temp,double xbarsize_temp){
	Tech=tech;
	celltype=celltype_temp;
	xbarsize=xbarsize_temp;
}

Cal_Subarray::~Cal_Subarray(){}

double Cal_Subarray::Subarray_Area(){
	if(celltype==0)
		return xbarsize*xbarsize*((3*(3+1)*Tech.featureSize*Tech.featureSize)); //MOS-accessed cell
		return xbarsize*xbarsize*4*Tech.featureSize*Tech.featureSize; //cross-point cell
}

void Cal_Subarray::Subarray_Latency(double _rampInput) {
	double bitlineDelay,senseAmp_readLatency;
	double resEquivalentOn = (double)pow(10.0,6.0) * sqrt(pow(10.0,13.0)) / (pow(10.0,6.0) + sqrt(pow(10.0,13.0)));
	//resEquivalentOn = resMemCellOn * resInSerialForSenseAmp / (resMemCellOn + resInSerialForSenseAmp);
	double capCellAccess = CalculateDrainCap(0 * Tech.featureSize, NMOS, 2 * Tech.featureSize, Tech);
	//capCellAccess = CalculateDrainCap(cell->widthAccessCMOS * tech->featureSize, NMOS, cell->widthInFeatureSize * tech->featureSize, *tech);
	double lenBitline = xbarsize * 2 * Tech.featureSize;
	//lenBitline = (double)numRow * cell->heightInFeatureSize * tech->featureSize;

	//type of wire: 45nm, local_aggressive
	double barrierThickness = 0.00e-6;
	double horizontalDielectric = 2.6;
	double wirePitch = 0.102e-6;
	double aspectRatio = 1.8;
	double ildThickness = 0.0918e-6;
	double copper_resistivity =  4.08e-8;
	double wireWidth = wirePitch / 2;
	double wireThickness = aspectRatio * wireWidth;
	double wireSpacing = wirePitch - wireWidth;
	double muxSenseAmp = 16;

	double capWirePerUnit = CalculateWireCapacitance(PERMITTIVITY, wireWidth, wireThickness, wireSpacing,
		ildThickness, 1.5 /* miller value */, horizontalDielectric, 3.9 /* Vertical Dielectric */,
		1.15e-10 /* Fringe Capacitance (Unit: F/m), TO-DO: CACTI assumes a fixed number here */);
	double capBitline = lenBitline * capWirePerUnit;

	double resWirePerUnit = CalculateWireResistance(copper_resistivity, wireWidth, wireThickness, barrierThickness,0 /* Dishing Thickness */, 1 /* Alpha Scatter */);
	double resBitline = lenBitline * resWirePerUnit;

	double tau_bitlineDelay = resEquivalentOn * (capCellAccess +capBitline/*+bitlineMux.capForPreviousDelayCalculation*/)+ resBitline * (/*bitlineMux.capForPreviousDelayCalculation +*/ capBitline / 2);
	
	
	// Voltage-in voltage sensing 
	double resCellAccess=0;
	//-ResistanceOnAtReadVoltage (ohm): 1000000
	//-ResistanceOffAtReadVoltage (ohm): 10000000
	double resMemCellOff = resCellAccess + 1000000;
	double resMemCellOn = resCellAccess + 1000000;
	double resInSerialForSenseAmp = sqrt(resMemCellOn * resMemCellOff);
	double voltageMemCellOff = 0.4 * resMemCellOff / (resMemCellOff + resInSerialForSenseAmp);//ReadVoltage (V): 0.4
	double voltageMemCellOn = 0.4 * resMemCellOn / (resMemCellOn + resInSerialForSenseAmp);//resMemCellOff
	double voltagePrecharge = (voltageMemCellOff + voltageMemCellOn) / 2;
	voltagePrecharge = MIN(Tech.vdd, voltagePrecharge);
	//senseVoltage = cell->minSenseVoltage; minSenseVoltage  = 0.08;
	double senseVoltage=0.08;
	double bitlineDelayOn = tau_bitlineDelay * log((voltagePrecharge - voltageMemCellOn)/(voltagePrecharge - voltageMemCellOn - senseVoltage));
	bitlineDelay= bitlineDelayOn;

	double widthInFeatureSize = 2;//widthInFeatureSize = sqrt(area / aspectRatio) = 2 (in RRAM);
	double lenWordline = (double)xbarsize * widthInFeatureSize * Tech.featureSize;
	double pitchSenseAmp = lenWordline / xbarsize * muxSenseAmp;//TO-DO: muxSenseAmp is a parameter given by users
	double capLoad = CalculateGateCap((W_SENSE_P + W_SENSE_N) * Tech.featureSize, Tech)
				+ CalculateDrainCap(W_SENSE_N * Tech.featureSize, NMOS, pitchSenseAmp, Tech)
				+ CalculateDrainCap(W_SENSE_P * Tech.featureSize, PMOS, pitchSenseAmp, Tech)
				+ CalculateDrainCap(W_SENSE_ISO * Tech.featureSize, PMOS, pitchSenseAmp, Tech)
				+ CalculateDrainCap(W_SENSE_MUX * Tech.featureSize, NMOS, pitchSenseAmp, Tech);
	double gm = CalculateTransconductance(W_SENSE_N * Tech.featureSize, NMOS, Tech)
				+ CalculateTransconductance(W_SENSE_P * Tech.featureSize, PMOS, Tech);

	double tau_Amp = capLoad / gm;
	senseAmp_readLatency = tau_Amp * log(Tech.vdd / senseVoltage);

	readLatency =/*decoderLatency +*/ bitlineDelay + /*bitlineMux.readLatency*/ + senseAmp_readLatency /*+senseAmpMuxLev1.readLatency + senseAmpMuxLev2.readLatency*/;
	
}

double Cal_Subarray::Subarray_Power(){
	double setEnergyPerBit = 0.6;//cell->setEnergy;
	double capCellAccess = 1e-16;
	double lenBitline = xbarsize * 2 * Tech.featureSize;

	//type of wire: 45nm, local_aggressive
	double barrierThickness = 0.00e-6;
	double horizontalDielectric = 2.6;
	double wirePitch = 0.102e-6;
	double aspectRatio = 1.8;
	double ildThickness = 0.0918e-6;
	double copper_resistivity =  4.08e-8;
	double wireWidth = wirePitch / 2;
	double wireThickness = aspectRatio * wireWidth;
	double wireSpacing = wirePitch - wireWidth;
	double setVoltage = 2.0;
	double resetenergy=0.6;
	double resetVoltage = setVoltage;
	double muxSenseAmp = 16;

	double capWirePerUnit = CalculateWireCapacitance(PERMITTIVITY, wireWidth, wireThickness, wireSpacing,
		ildThickness, 1.5 /* miller value */, horizontalDielectric, 3.9 /* Vertical Dielectric */,
		1.15e-10 /* Fringe Capacitance (Unit: F/m), TO-DO: CACTI assumes a fixed number here */);
	double capBitline = lenBitline * capWirePerUnit;
	capBitline  += capCellAccess * xbarsize / 2;
	capBitline = lenBitline * capWirePerUnit;
	setEnergyPerBit += (capCellAccess + capBitline /*+ bitlineMux.capForPreviousPowerCalculation*/) * setVoltage * setVoltage;

	double resetEnergyPerBit = resetenergy;
	resetEnergyPerBit += (capCellAccess + capBitline /*+ bitlineMux.capForPreviousPowerCalculation*/) * resetVoltage * resetVoltage;

	double cellResetEnergy = resetEnergyPerBit * xbarsize / muxSenseAmp;// / muxOutputLev1 / muxOutputLev2;
	double cellSetEnergy = setEnergyPerBit * xbarsize / muxSenseAmp;// / muxOutputLev1 / muxOutputLev2;
	double writeDynamicEnergy = cellResetEnergy + cellSetEnergy;
	return writeDynamicEnergy;
}
