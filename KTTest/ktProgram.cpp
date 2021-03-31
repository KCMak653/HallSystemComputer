#include "ktCmd.h"
#include "ktSweep.h"
#include "ktConst.h"
#include "ktProgram.h"
#include <string.h>
#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>
#include<time.h>
#include<string.h>
#include<string>
#include <fstream>
#include "ni4882.h"

namespace KT
{
	sweepProgram::sweepProgram(const sweepProgramParameters &entries)
	{
		//Set all private member variables to inputs
		//sweepP_.sweepSMU = entries.sweepSMU;
		//sweepP_.constSMU = entries.constSMU;
		//sweepP_.measSMU = entries.measSMU;
		
		for (int i = 0; i<4; i++){
			sweepP_.appV[i] = entries.appV[i];
			sweepP_.range[i] = entries.range[i];
			sweepP_.comp[i] = entries.comp[i];
			sweepP_.forceMode[i] = entries.forceMode[i];
			sweepP_.measMode[i] = entries.measMode[i];
			sweepP_.sweepSMU[i] = entries.sweepSMU[i];
			sweepSMU_[i]=entries.sweepSMU[i];
		}

		
		
		
		sweepP_.SR = entries.SR;
		//sweepP_.constV = entries.constV;
		sweepP_.startV = entries.startV;
		sweepP_.stopV = entries.stopV;
		sweepP_.lRange = entries.lRange;
		//sweepP_.range = entries.range;
		//sweepP_.comp = entries.comp;
		sweepP_.intTime = entries.intTime;

		fullCycle_ = entries.fullCycle;
		nCycles_ = entries.nCycles;
		
		int k = 0;
		for (int i = 0; i<4; i++){
			if ((entries.measMode[i] == 'I')|(entries.measMode[i] == 'V')){
				chMeas_[k] = i+1;
				k++;
			}
		}
		nMeasChannels_ = k;
		//Initialize the sweep object
		swp_ = new KT::ktSweep(sweepP_);

		//Find the size of array needed to store values
		swpSize_ = swp_->arraySizeNeeded();

		//Calculate runtime:
		runTime_ = (fabs(sweepP_.startV - sweepP_.stopV)/sweepP_.SR)
			*(int(fullCycle_)+1)*nCycles_;
	}

	int sweepProgram::arraySizeNeeded()
	{
		int sizeArrayNeeded_ = (swpSize_ - 1)*(int(fullCycle_)+1)*nCycles_
				+(1-int(fullCycle_))*nCycles_+1;
		return sizeArrayNeeded_;
	}
	
	int sweepProgram::setSR(double SR){
		swp_->setSR(SR);
		return 0;
	}

	int sweepProgram::runProgram(double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray)
	{
		if (Ibsta() & ERR) {
			std::cout << "Issue with device initialization - Quitting now" << std::endl;
			return 1;
		}
		int smuInd = 0;
		int k = 0;
		while (smuInd == 0){
			if (sweepSMU_[k]){
				smuInd = k+1;
			}
			k++;
		}
		
		
		time_t tstart;
		time_t tend;
		time(&tstart);
		tend = tstart+runTime_;
		std::cout<<"Program began at: " <<ctime(&tstart);
		std::cout<<"Program will finish at: " <<ctime(&tend);


		//Initialize channels, apply biases
		swp_-> initializeChannels();
		swp_->setForceParams(smuInd);
		//Set initial indice
		int iStart = 0;

		//Execute outer for loop for number of cycles requested
		//Execute inner loop only if full cycle 
		for (int j = 0; j<nCycles_; j++)
		{
			//Run the sweep
			swp_->runSweep(vFs, iMs, tMs, dMs, swpSize_, iStart);
			//Change the array indice
			iStart = iStart + swpSize_ - 1;
			if (fullCycle_)
			{
				//Reverse the direction of the sweep
				swp_->reverseV();
				swp_->runSweep(vFs, iMs, tMs, dMs, swpSize_, iStart);
				swp_->reverseV();
				iStart = iStart + swpSize_ - 1;
			}
			
		}
		return 0;
	}

	int sweepProgram::saveData(std::string fn, double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray)
	{
		//Data directory
		std::string direc = "C:/Users/Nanotech/Documents/KMData/Keithley/";
		//save as text file
		std::string frmt = ".csv";

		//Build file name for data and a second one to save test parameters
		std::string fp = direc + fn + frmt;
		std::string fpP = direc + fn + "_Param" + frmt;

		std::ofstream myfile;
		myfile.open(fp);
		myfile<<"vForce [V], ";
		for (int n = 0; n < nMeasChannels_; n++)
		{
			myfile<<"iMeas [A] - SMU " << chMeas_[n] << ", ";
		} 
		myfile << "time [ms], dTime [ms]\n";

		for (int i = 0; i<sizeArray; i++)
		{
			myfile<<vFs[i]<<',';
			for (int n = 0; n < nMeasChannels_; n++){
				myfile << iMs[i * nMeasChannels_ + n] << ',';
			}
			myfile<<tMs[i]<<','<<dMs[i]<<"\n";
		}

		myfile.close();

		//std::string fnP = "P_" + fn;
		std::ofstream myfile2;
		myfile2.open(fpP);
		myfile2<<"sweepProgram: Parameters\n";
		myfile2<<"startV [V], stopV [V], SR [V/s], constV [V/A], lRange, range, comp, intTime, sweepSMU, forceMode, measMode, nCycles, fullCycle\n";
		myfile2<<sweepP_.startV<<","<<sweepP_.stopV<<','<<sweepP_.SR<<", [";
		
		for (int smu=0; smu<4; smu++){
			myfile2<<sweepP_.appV[smu]<<',';
		}
		myfile2<<sweepP_.appV[3]<<"] ,"
				<<sweepP_.lRange<<",[";
					
		for (int smu = 0; smu<3; smu++){
				myfile2<<sweepP_.range[smu]<<',';
		}
		myfile2 << sweepP_.range[3] << "], [";
			
		for (int smu = 0; smu<3; smu++){
			myfile2<<sweepP_.comp[smu]<<',';
		}

		myfile2 << sweepP_.comp[3] << "], "
			<< sweepP_.intTime << ", [";
		
		for (int smu = 0; smu<3; smu++){
			myfile2<<sweepSMU_[smu]<<',';
		}		
		myfile2 << sweepSMU_[3] << "], [";
		
		
		for (int smu = 0; smu<3; smu++){
			myfile2<<sweepP_.forceMode[smu]<<',';
		}

		myfile2 << sweepP_.forceMode[3]<<"], [";

		for (int smu = 0; smu<3; smu++){
			myfile2<<sweepP_.measMode[smu]<<',';
		}

		myfile2 << sweepP_.measMode[3]<<"], ";
		
		myfile2<<nCycles_<<','<<fullCycle_<<"\n";
		myfile2.close();

		return 0;
	}

	sweepProgram::~sweepProgram(){
		delete swp_;
	}

	constProgram::constProgram(const constProgramParameters &entries){

		//Set all private member variables to inputs
		constP_.dt = entries.dt;
		//constP_.constSMU = entries.constSMU;
		//constP_.measSMU = entries.measSMU;
		constP_.measTime = entries.measTime;
		for (int i = 0; i<4; i++){
			constP_.appV[i] = entries.appV[i];
			constP_.forceMode[i] = entries.forceMode[i];
			constP_.measMode[i] = entries.measMode[i];
			constP_.range[i] = entries.range[i];
			constP_.comp[i] = entries.comp[i];
		}
		constP_.lRange = entries.lRange;
		//constP_.range = entries.range;
		//constP_.comp = entries.comp;
		constP_.intTime = entries.intTime;
		//constP_.measMode = 'I';
		//constP_.forceMode = 'V';
		//constP_.nChannels = entries.nChannels;
		//Initialize the const object
		cnst_ = new KT::ktConst(constP_);

		//Find the size of array needed to store values
		cnstSize_ = cnst_->arraySizeNeeded();
		runTime_ = constP_.measTime;
		//nChannels_ = constP_.nChannels;


		int k = 0;
		for (int i = 0; i<4; i++){
			if ((constP_.measMode[i] == 'I')|(constP_.measMode[i] == 'V')){
				chMeas_[k] = i+1;
				k++;
			}
		}

		nMeasChannels_ = k;
	}

	int constProgram::arraySizeNeeded()
	{
		return cnstSize_;
	}

	int constProgram::runProgram(double iMs[], double tMs[], int dMs[], int sizeArray)
	{
		cnst_->initializeChannels();
		time_t tstart;
		time_t tend;
		time(&tstart);
		tend = tstart+runTime_;
		std::cout<<"Program began at: " <<ctime(&tstart);
		std::cout<<"Program will finish at: " <<ctime(&tend);
		int iStart = 0;
		cnst_ -> runTest(iMs, tMs, dMs, cnstSize_, iStart);
		return 0;
	}

	int constProgram::saveData(std::string fn, double iMs[], double tMs[], int dMs[], int sizeArray)
	{
		
		//Data directory
		std::string direc = "C:/Users/Nanotech/Documents/KMData/Keithley/";
		//save as text file
		std::string frmt = ".csv";

		//Build file name for data and a second one to save test parameters
		std::string fp = direc + fn + frmt;
		std::string fpP = direc + fn + "_Param" + frmt;

		std::ofstream myfile;
		myfile.open(fp);

		for (int n = 0; n < nMeasChannels_; n++)
		{
			myfile<<"iMeas [A] - SMU " << chMeas_[n] << ", ";
		} 
		myfile << "time [ms], dTime [ms]\n";
		for (int i = 0; i<sizeArray; i++)
		{
			for (int n = 0; n < nMeasChannels_; n++){
				myfile << iMs[i * nMeasChannels_ + n] << ',';
			}	
			myfile<<tMs[i]<<','<<dMs[i]<<"\n";
		}
		myfile.close();

		//std::string fnP = "P_" + fn;
		std::ofstream myfile2;
		myfile2.open(fpP);
		myfile2<<"constProgram: Parameters\n";
		myfile2<<"dt [ms], measTime [s], constV [V/A], lRange, range, comp, intTime,"
				" forceMode, measMode\n";
		myfile2<<constP_.dt<<","<<constP_.measTime<<", [";
		for (int smu = 0; smu<3; smu++){
			myfile2<<constP_.appV[smu]<<',';
		}
		myfile2<<constP_.appV[3]<< "],"<<constP_.lRange<<", [";
		
		for (int smu = 0; smu<3; smu++){
			myfile2<<constP_.range[smu]<<',';
		}
		myfile2 << constP_.range[3] << "], [";
		
		for (int smu = 0; smu<3; smu++){
			myfile2<<constP_.comp[smu]<<',';
		}

		myfile2 << constP_.comp[3] << "], "
			<< constP_.intTime << ", [";

		for (int smu = 0; smu<3; smu++){
			myfile2<<constP_.forceMode[smu]<<',';
		}		
		myfile2 << constP_.forceMode[3] << "], [";
		
		for (int smu = 0; smu<3; smu++){
			myfile2<<constP_.measMode[smu]<<',';
		}

		myfile2 << constP_.measMode[3]<<"]\n";
		myfile2.close();

		return 0;
	}

	constProgram::~constProgram(){
		delete cnst_;
	}

	stepProgram::stepProgram(const stepProgramParameters &entries){

		//Set all private member variables to inputs
		
		stepP_.dt = entries.dt;
		for (int smu=0; smu<4; smu++){
			stepP_.appV[smu] = entries.appV[smu];
			stepP_.range[smu] = entries.range[smu];
			stepP_.comp[smu] = entries.comp[smu];
			stepP_.measMode[smu] = entries.measMode[smu];
			stepP_.forceMode[smu] = entries.forceMode[smu];
			stepSMU_[smu] = entries.stepSMU[smu];
		}
		//stepP_.appV[entries.measSMU-1] = entries.startV;
		//stepP_.constSMU[i] = entries.stepSMU;
		//stepP_.measSMU = entries.measSMU;
		stepP_.measTime = entries.stepTime;
		//stepP_.nChannels = entries.nChannels;
		stepP_.lRange = entries.lRange;
		//stepP_.range = entries.range;
		//stepP_.comp = entries.comp;
		stepP_.intTime = entries.intTime;
		//stepP_.measMode = 'I';
		//stepP_.forceMode = 'V';

		startV_ = entries.startV;
		stopV_ = entries.stopV;
		stepV_ = entries.stepV;
		//stepSMU_ = entries.stepSMU;
		//constV_ = entries.constV;
		nCycles_ = entries.nCycles;
		fullCycle_ = entries.fullCycle;
		//nChannels_ = entries.nChannels;

		//constSMU_ = entries.constSMU;
		int k = 0;
		for (int i = 0; i<4; i++){
			if ((stepP_.measMode[i] == 'I')|(stepP_.measMode[i] == 'V')){
				chMeas_[k] = i+1;
				k++;
			}
		}

		nMeasChannels_ = k;

		//Get ordered force channels
		step_ = new KT::ktConst(stepP_);

		stepSize_ = step_->arraySizeNeeded() - 1;
		std::cout<<"stepSize: "<<stepSize_<<std::endl;
		//Calculate number of steps for single climb and total number of steps
		std::cout<<"fC: "<<int(fullCycle_)<<std::endl;
		nSteps_ = (fabs(startV_ - stopV_)/(stepV_))+1.5;
		//std::cout<<(nSteps_ - 1)*((int(fullCycle_)+1)*nCycles_)<<std::endl;
		nStepsTot_ = (nSteps_ - 1)*((int(fullCycle_)+1)*nCycles_)
				+(1-int(fullCycle_))*nCycles_+ fullCycle_;
		runTime_ = nStepsTot_ * stepP_.measTime;
		if (startV_>stopV_) stepV_ = -fabs(stepV_);
		std::cout<<"nStep: "<<nSteps_<<std::endl;
		std::cout<<"nSteptot: "<<nStepsTot_<<std::endl;
		sizeArrayNeeded_ = nStepsTot_*(stepSize_) + 1;
		
	}

	int stepProgram::arraySizeNeeded(){
		return sizeArrayNeeded_;
	}

	int stepProgram::runProgram(double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray)
	{
		if (sizeArray != sizeArrayNeeded_){
			std::cout<<"Incorrect Array size"<<std::endl;
			return 1;
		}
		//Isolate first Step SMU index to set force param(range, comp, etc) correctly
		int smuInd = 0;
		int k = 0;
		while (smuInd == 0){
			if (stepSMU_[k]){
				smuInd = k+1;
			}
			k++;
		}


		time_t tstart;
		time_t tend;
		time(&tstart);
		tend = tstart+runTime_;
		std::cout<<"Program began at: " <<ctime(&tstart);
		std::cout<<"Program will finish at: " <<ctime(&tend);
		//Set initial indice
		step_ -> initializeChannels();
		step_-> setForceParams(smuInd);
		int iStart = 0;
		int nS = nSteps_;
		double iV = startV_;
		//step_->forceConstV(constSMU_, constV_);
		
		//std::cout<<"Forcing "<< constV_ << "on SMU" << constSMU_<<std::endl;
		//Execute outer for loop for number of cycles requested
		//Execute inner loop only if full cycle 
		for (int j = 0; j<nCycles_; j++)
		{
			//Run the sweep
			iStart = runFlight(iMs, tMs, dMs, iStart, nS, iV);
			nS = nSteps_ - fullCycle_;
			//Change the array indice
			std::cout<<iStart<<std::endl;
			if (iStart > sizeArrayNeeded_){
				std::cout<<"gone wild"<<std::endl;
				return 5;
			}
			//iStart = iStart + (stepSize_ - 1)*(nSteps_-int(fullCycle_)) - 1;
			if (fullCycle_)
			{
				//Reverse the direction of the sweep
				reverseV();
				iV = startV_ + stepV_;
				iStart = runFlight(iMs, tMs, dMs, iStart, nS, iV);
				reverseV();
				//iStart = iStart + (stepSize_ - 1) * (nSteps_ -1) - 1;
			}
			iV = startV_ + stepV_*(int(fullCycle_));
			
		}

		//Fill in vFs[], since ktConst does not do this
		std::cout << "nS" << nS << std::endl;
		iStart = 0;
		iV = startV_;
		int jEnd = nS + 1;
		for (int i = 0; i<nCycles_; i++){ 
			for (int j = 0; j<jEnd; j++){
				std::cout << 'j' << j << std::endl;
				for (int k=iStart; k<(iStart+stepSize_+1); k++){
					vFs[k] = iV;
				}
				iV = iV + stepV_;
				iStart = iStart + stepSize_ ;
			}
			if (fullCycle_) {
				reverseV();
				iV = startV_ + stepV_;
				for (int j = 0; j<nS; j++){
					for (int k=iStart; k<(iStart+stepSize_+1); k++){
						vFs[k] = iV;
					}
				iV = iV + stepV_;
				iStart = iStart + stepSize_ ;
				}
				reverseV();
			}
			iV = startV_ + stepV_*(int(fullCycle_));
			jEnd = nS;
		}
		return 0;
	}

	int stepProgram::runFlight(double iMs[], double tMs[], int dMs[], int iStart, int nS, double v)
	{
		step_->setIV(stepSMU_, v);
		for (int i=0; i<nS; i++){
			std::cout<<iStart<<std::endl;
			step_->runTest(iMs, tMs, dMs, (stepSize_+1), iStart);
			v = v + stepV_;
			if (i != (nS - 1)) {
				step_->setIV(stepSMU_, v);
			}
			iStart = iStart + stepSize_ ;
		}
		return iStart;
	}

	void stepProgram::reverseV()
	{
		double temp = startV_;
		startV_ = stopV_;
		stopV_ = temp;
		stepV_ = -1*stepV_;
	}

	int stepProgram::saveData(std::string fn, double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray)
	{
		
		//Data directory
		std::string direc = "C:/Users/Nanotech/Documents/KMData/Keithley/";
		//save as text file
		std::string frmt = ".csv";

		//Build file name for data and a second one to save test parameters
		std::string fp = direc + fn + frmt;
		std::string fpP = direc + fn + "_Param" + frmt;

		std::ofstream myfile;
		myfile.open(fp);
		myfile<<"vForce [V], ";
		for (int n = 0; n < nMeasChannels_; n++)
		{
			myfile<<"iMeas [A] - SMU " << chMeas_[n] << ", ";
		} 
		myfile << "time [ms], dTime [ms]\n";

		for (int i = 0; i<sizeArray; i++)
		{
			myfile<<vFs[i]<<',';
			for (int n = 0; n < nMeasChannels_; n++){
				myfile << iMs[i * nMeasChannels_ + n] << ',';
			}
			myfile<<tMs[i]<<','<<dMs[i]<<"\n";
		}
		myfile.close();

		//std::string fnP = "P_" + fn;
		std::ofstream myfile2;
		myfile2.open(fpP);
		myfile2<<"stepProgram: Parameters\n";
		myfile2<<"dt [ms], stepTime [s], startV [V], stopV [V], stepV [V], "
			"constV [V], nCycles, fullCycle, lRange, range, comp, "
			"intTime, stepSMU, forceMode, measMode\n";
		myfile2<<stepP_.dt<<","<<stepP_.measTime<<','<<startV_<<','<<stopV_<<','<<stepV_<<", [";
		
		for (int smu = 0; smu<3; smu++){
			myfile2<<stepP_.appV[smu]<<',';
		}
		myfile2 <<stepP_.appV[3]<<"],"<< nCycles_<<','<<fullCycle_<<','
				<<stepP_.lRange<<", [";
					
		for (int smu = 0; smu<3; smu++){
			myfile2<<stepP_.range[smu]<<',';
		}
		myfile2 << stepP_.range[3] << "], [";
		
		for (int smu = 0; smu<3; smu++){
			myfile2<<stepP_.comp[smu]<<',';
		}

		myfile2<<stepP_.comp[3]<<"], "
				<<stepP_.intTime<<", [";

		for (int smu = 0; smu<3; smu++){
			myfile2<<stepSMU_[smu]<<',';
		}		
		myfile2 << stepSMU_[3] << "], [";
	
		for (int smu = 0; smu<3; smu++){
			myfile2<<stepP_.forceMode[smu]<<',';
		}

		myfile2 << stepP_.forceMode[3]<<"], [";
		
		for (int smu = 0; smu<3; smu++){
			myfile2<<stepP_.measMode[smu]<<',';
		}

		myfile2 << stepP_.measMode[3]<<"]\n";
		myfile2.close();
		
		return 0;
	}
	stepProgram::~stepProgram(){
		delete step_;
	}


	pulseProgram::pulseProgram(const pulseProgramParameters &entries){

		//Set all private memeber variables to inputs

		pulseP_.dt  = entries.dt;
		for (int smu=0; smu<4; smu++){
			pulseP_.appV[smu] = entries.appV[smu];
			pulseP_.range[smu] = entries.range[smu];
			pulseP_.comp[smu] = entries.comp[smu];
			pulseP_.measMode[smu] = entries.measMode[smu];
			pulseP_.forceMode[smu] = entries.forceMode[smu];
			pulseSMU_[smu] = entries.pulseSMU[smu];
		}
		//pulseP_.measSMU = entries.measSMU;

		pulseP_.lRange = entries.lRange;
		//pulseP_.range = entries.range;
		//pulseP_.comp = entries.comp;
		pulseP_.intTime = entries.intTime;
		// pulseP_.measMode = 'I';
		// pulseP_.forceMode = 'V';

		pulseP_.measTime = entries.initTime;

		initTime_ = entries.initTime;
		pulseTime_ = entries.pulseTime;
		stepTime_ = entries.stepTime;

		pulseOffV_ = entries.pulseOffV;
		pulseV_ = entries.pulseV;
		nPulses_ = entries.nPulses;
		// pulseSMU_ = entries.pulseSMU;
		//nChannels_ = entries.nChannels;

		int k = 0;
		for (int i = 0; i<4; i++){
			if ((entries.measMode[i] == 'I')|(entries.measMode[i]== 'V')){
				chMeas_[k] = i+1;
				k++;
			}
		}
		nMeasChannels_ = k;
		//Calculate size needed for each step

		pulse_ = new KT::ktConst(pulseP_);

		initSize_ = pulse_->arraySizeNeeded() - 1;
		std::cout<<"init size: " << initSize_<<std::endl;
		pulse_->setMeasTime(stepTime_);
		stepSize_ = pulse_->arraySizeNeeded() - 1;
		std::cout<<"step size: " << stepSize_<<std::endl;
		pulse_->setMeasTime(pulseTime_);
		pulseSize_ = pulse_->arraySizeNeeded() - 1;
		std::cout<<"pulse size: " << pulseSize_<<std::endl;
		if (pulseSize_ < 2){ 
			std::cout<<"Need longer pulse"<<std::endl;
		}
		sizeArrayNeeded_ = initSize_  + (stepSize_ + pulseSize_ )*nPulses_ ;
		runTime_ = sizeArrayNeeded_ * pulseP_.dt*1e-3;
	}

	int pulseProgram::arraySizeNeeded(){
		return sizeArrayNeeded_;
	}

	int pulseProgram::runProgram(double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray)
	{
		if (sizeArray != sizeArrayNeeded_){
			std::cout<<"Incorrect Array size"<<std::endl;
			return 1;
		}
		int smuInd = 0;
		int k = 0;
		while (smuInd == 0){
			if (pulseSMU_[k]){
				smuInd = k+1;
			}
			k++;
		}
		time_t tstart;
		time_t tend;
		time(&tstart);
		tend = tstart+runTime_;
		std::cout<<"Program began at: " <<ctime(&tstart);
		std::cout<<"Program will finish at: "<<ctime(&tend);


		//Set initial measurement time
		pulse_->setMeasTime(initTime_);
		pulse_->initializeChannels();
		pulse_->setForceParams(smuInd);
		//Set initial indice
		int iStart = 0;

		pulse_->runTest(iMs, tMs, dMs, (initSize_+1), iStart);
		iStart = iStart + initSize_ - 1;

		for (int i = 0; i<nPulses_; i++){

			pulse_->setMeasTime(pulseTime_);
			pulse_->setIV(pulseSMU_, pulseV_);
			pulse_->runTest(iMs, tMs, dMs, (pulseSize_+1), iStart);
			pulse_->setIV(pulseSMU_, pulseOffV_);
			iStart = iStart + pulseSize_ ;
			pulse_->setMeasTime(stepTime_);
			pulse_->runTest(iMs, tMs, dMs, (stepSize_+1), iStart);
			//std::cout<<"pre: "<<iStart<<std::endl;
			iStart = iStart + stepSize_ ;
			//std::cout<<iStart<<std::endl;
		}
		//Fill in vFs

		iStart = 0;
		int iV = pulseOffV_;

		for (int k = 0; k<initSize_; k++){
			vFs[k] = pulseOffV_;
		}
		iStart = iStart + initSize_ ;

		for (int i = 0; i<nPulses_; i++){
			for (int k = iStart; k<(iStart+pulseSize_); k++)
			{
				vFs[k] = pulseV_;
			}
			iStart = iStart + pulseSize_ ;
			for (int k = iStart; k<(iStart+stepSize_); k++)
			{
				vFs[k] = pulseOffV_;
				//std::cout<<"k: "<<k<<std::endl;
			}
			iStart = iStart + stepSize_ ;
		}

		return 0;
	}


	int pulseProgram::saveData(std::string fn, double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray)
	{
		//Data Directory
		std::string direc = "C:/Users/Nanotech/Documents/KMData/Keithley/";
		//save as text file
		std::string frmt = ".csv";
		
		std::string fp = direc + fn + frmt;
		std::string fpP = direc + fn + "_Param" + frmt;

		std::ofstream myfile;
		myfile.open(fp);
		myfile<<"vForce [V], ";
		for (int n = 0; n < nMeasChannels_; n++)
		{
			myfile<<"iMeas [A] - SMU " << chMeas_[n] << ", ";
		} 
		myfile << "time [ms], dTime [ms]\n";

		for (int i = 0; i<sizeArray; i++)
		{
			myfile<<vFs[i]<<',';
			for (int n = 0; n < nMeasChannels_; n++){
				myfile << iMs[i * nMeasChannels_ + n] << ',';
			}
			myfile<<tMs[i]<<','<<dMs[i]<<"\n";
		}
		myfile.close();

		std::ofstream myfile2;
		myfile2.open(fpP);
		myfile2<<"pulseProgram: Parameters\n";
		myfile2<<"dt [ms], initTime [s], pulseTime [s], stepTime [s],"
			" pulseV [V], pulseOffV [V], constV [V/A], nPulses, lRange, range,"
			"comp, intTime, pulseSMU, forceMode, measMode\n";
		myfile2<<pulseP_.dt <<','<< initTime_<<','<< pulseTime_ <<','<<stepTime_<<','
			<<pulseV_<<','<<pulseOffV_<<", [";
		for (int smu=0; smu<3; smu++){
			myfile2<<pulseP_.appV[smu]<<',';
			
		}

		myfile2<<pulseP_.appV[3]<< "],"<<nPulses_<<','<< pulseP_.lRange<<", [";
		for (int smu = 0; smu<3; smu++){
				myfile2<<pulseP_.range[smu]<<',';
		}
		myfile2 << pulseP_.range[3] << "], [";
			
		for (int smu = 0; smu<3; smu++){
			myfile2<<pulseP_.comp[smu]<<',';
		}

		myfile2 << pulseP_.comp[3] << "], "
			<< pulseP_.intTime << ", [";

		for (int smu = 0; smu<3; smu++){
			myfile2<<pulseSMU_[smu]<<',';
		}		
		myfile2 << pulseSMU_[3] << "], [";
		
		for (int smu = 0; smu<3; smu++){
			myfile2<<pulseP_.forceMode[smu]<<',';
		}

		myfile2 << pulseP_.forceMode[3]<<"], [";
			
		for (int smu = 0; smu<3; smu++){
			myfile2<<pulseP_.measMode[smu]<<',';
		}

		myfile2 << pulseP_.measMode[3]<<"]\n";
		myfile2.close();

		return 0;
	}

	pulseProgram::~pulseProgram(){
		delete pulse_;
	}



}