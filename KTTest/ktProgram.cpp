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

namespace KT
{
	sweepVDS_IDS::sweepVDS_IDS(const sweepVDS_IDSParameters &entries)
	{
		//Set all private member variables to inputs
		sweepP_.sweepSMU = entries.sweepSMU;
		sweepP_.constSMU = entries.constSMU;
		sweepP_.measSMU = entries.measSMU;
		sweepP_.SR = entries.SR;
		sweepP_.constV = entries.constV;
		sweepP_.startV = entries.startV;
		sweepP_.stopV = entries.stopV;
		sweepP_.lRange = entries.lRange;
		sweepP_.range = entries.range;
		sweepP_.comp = entries.comp;
		sweepP_.intTime = entries.intTime;

		fullCycle_ = entries.fullCycle;
		nCycles_ = entries.nCycles;
		
		
		//Initialize the sweep object
		swp_ = new KT::ktSweep(sweepP_);

		//Find the size of array needed to store values
		swpSize_ = swp_->arraySizeNeeded();

		//Calculate runtime:
		runTime_ = (fabs(sweepP_.startV - sweepP_.stopV)/sweepP_.SR)
			*(int(fullCycle_)+1)*nCycles_;
	}

	int sweepVDS_IDS::arraySizeNeeded()
	{
		int sizeArrayNeeded_ = (swpSize_ - 1)*(int(fullCycle_)+1)*nCycles_
				+(1-int(fullCycle_))*nCycles_+1;
		return sizeArrayNeeded_;
	}
	
	int sweepVDS_IDS::setSR(double SR){
		swp_->setSR(SR);
		return 0;
	}

	int sweepVDS_IDS::runProgram(double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray)
	{
		time_t tstart;
		time_t tend;
		time(&tstart);
		tend = tstart+runTime_;
		std::cout<<"Program began at: " <<ctime(&tstart);
		std::cout<<"Program will finish at: " <<ctime(&tend);
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

	int sweepVDS_IDS::saveData(std::string fn, double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray)
	{
		//Data directory
		std::string direc = "C:/Documents and Settings/NanoHall_User/My Documents/Data-Kristin/Keithley/";
		//save as text file
		std::string frmt = ".csv";

		//Build file name for data and a second one to save test parameters
		std::string fp = direc + fn + frmt;
		std::string fpP = direc + fn + "_Param" + frmt;

		std::ofstream myfile;
		myfile.open(fp);
		myfile<<"vForce [V], iMeas [A], time [ms], dTime [ms]\n";
		for (int i = 0; i<sizeArray; i++)
		{
			//std::cout<<"Saving line: "<< i << " of " << sizeArray <<std::endl;
			myfile<<vFs[i]<<','<<iMs[i]<<','<<tMs[i]<<','<<dMs[i]<<"\n";
		}
		myfile.close();

		//std::string fnP = "P_" + fn;
		std::ofstream myfile2;
		myfile2.open(fpP);
		myfile2<<"startV [V], stopV [V], SR [V/s], constV [V], lRange, range, comp, intTime, sweepSMU, constSMU, measSMU, nCycles, fullCycle\n";
		myfile2<<sweepP_.startV<<","<<sweepP_.stopV<<','<<sweepP_.SR<<','<<sweepP_.constV<<','
				<<sweepP_.lRange<<','<<sweepP_.range<<','<<sweepP_.comp<<','
				<<sweepP_.intTime<<','<<sweepP_.sweepSMU<<','<<sweepP_.constSMU<<','<<sweepP_.measSMU<<','
				<<nCycles_<<','<<fullCycle_<<"\n";
		myfile2.close();

		return 0;
	}

	sweepVDS_IDS::~sweepVDS_IDS(){
		delete swp_;
	}

	constVDS_IDS::constVDS_IDS(const constVDS_IDSParameters &entries){

		//Set all private member variables to inputs
		constP_.dt = entries.dt;
		//constP_.constSMU = entries.constSMU;
		constP_.measSMU = entries.measSMU;
		constP_.measTime = entries.measTime;
		for (int i = 0; i<4; i++){
			constP_.appV[i] = entries.appV[i];
		}
		constP_.lRange = entries.lRange;
		constP_.range = entries.range;
		constP_.comp = entries.comp;
		constP_.intTime = entries.intTime;

		//Initialize the const object
		cnst_ = new KT::ktConst(constP_);

		//Find the size of array needed to store values
		cnstSize_ = cnst_->arraySizeNeeded();
		runTime_ = constP_.measTime;
	}

	int constVDS_IDS::arraySizeNeeded()
	{
		return cnstSize_;
	}

	int constVDS_IDS::runProgram(double iMs[], double tMs[], int dMs[], int sizeArray)
	{
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

	int constVDS_IDS::saveData(std::string fn, double iMs[], double tMs[], int dMs[], int sizeArray)
	{
		
		//Data directory
		std::string direc = "C:/Documents and Settings/NanoHall_User/My Documents/Data-Kristin/Keithley/";
		//save as text file
		std::string frmt = ".csv";

		//Build file name for data and a second one to save test parameters
		std::string fp = direc + fn + frmt;
		std::string fpP = direc + fn + "_Param" + frmt;

		std::ofstream myfile;
		myfile.open(fp);
		myfile<<"iMeas [A], time [ms], dTime [ms]\n";
		for (int i = 0; i<sizeArray; i++)
		{
			myfile<<iMs[i]<<','<<tMs[i]<<','<<dMs[i]<<"\n";
		}
		myfile.close();

		//std::string fnP = "P_" + fn;
		std::ofstream myfile2;
		myfile2.open(fpP);
		myfile2<<"dt [ms], measTime [s], constV [V], lRange, range, comp, intTime, measSMU\n";
		myfile2<<constP_.dt<<","<<constP_.measTime<<', [';
		for (int smu = 0; smu<3; smu++){
			myfile2<<constP_.appV[smu]<<',';
		}
		myfile2<<constP_.appV[3]<< '],'<<constP_.lRange<<','<<constP_.range<<','<<constP_.comp<<','
				<<constP_.intTime<<','<<','<<constP_.measSMU<<"\n";
		myfile2.close();

		return 0;
	}

	constVDS_IDS::~constVDS_IDS(){
		delete cnst_;
	}

	stepVDS_IDS::stepVDS_IDS(const stepVDS_IDSParameters &entries){

		//Set all private member variables to inputs
		
		stepP_.dt = entries.dt;
		for (int smu=0; smu<4; smu++){
			stepP_.appV[smu] = entries.appV[smu];
		}
		stepP_.appV[entries.measSMU-1] = entries.startV;
		//stepP_.constSMU[i] = entries.stepSMU;
		stepP_.measSMU = entries.measSMU;
		stepP_.measTime = entries.stepTime;
		
		stepP_.lRange = entries.lRange;
		stepP_.range = entries.range;
		stepP_.comp = entries.comp;
		stepP_.intTime = entries.intTime;

		startV_ = entries.startV;
		stopV_ = entries.stopV;
		stepV_ = entries.stepV;
		stepSMU_ = entries.stepSMU;
		//constV_ = entries.constV;
		nCycles_ = entries.nCycles;
		fullCycle_ = entries.fullCycle;

		//constSMU_ = entries.constSMU;

		step_ = new KT::ktConst(stepP_);

		stepSize_ = step_->arraySizeNeeded();
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
		sizeArrayNeeded_ = nStepsTot_*(stepSize_-1) + 1;
		
	}

	int stepVDS_IDS::arraySizeNeeded(){
		return sizeArrayNeeded_;
	}

	int stepVDS_IDS::runProgram(double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray)
	{
		if (sizeArray != sizeArrayNeeded_){
			std::cout<<"Incorrect Array size"<<std::endl;
			return 1;
		}
		time_t tstart;
		time_t tend;
		time(&tstart);
		tend = tstart+runTime_;
		std::cout<<"Program began at: " <<ctime(&tstart);
		std::cout<<"Program will finish at: " <<ctime(&tend);
		//Set initial indice
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
		iStart = 0;
		iV = startV_;
		for (int i = 0; i<nCycles_; i++){ 
			for (int j = 0; j<nS; j++){
				for (int k=iStart; k<(iStart+stepSize_); k++){
					vFs[k] = iV;
				}
				iV = iV + stepV_;
				iStart = iStart + stepSize_ - 1;
			}
			if (fullCycle_) {
				reverseV();
				iV = startV_ + stepV_;
				for (int j = 0; j<nS; j++){
					for (int k=iStart; k<(iStart+stepSize_); k++){
						vFs[k] = iV;
					}
				iV = iV + stepV_;
				iStart = iStart + stepSize_ - 1;
				}
				reverseV();
			}
			iV = startV_ + stepV_*(int(fullCycle_));
		}
		return 0;
	}

	int stepVDS_IDS::runFlight(double iMs[], double tMs[], int dMs[], int iStart, int nS, double v)
	{
		step_->setV(stepSMU_, v);
		for (int i=0; i<nS; i++){
			std::cout<<iStart<<std::endl;
			step_->runTest(iMs, tMs, dMs, stepSize_, iStart);
			v = v + stepV_;
			step_->setV(stepSMU_, v);
			iStart = iStart + stepSize_- 1;
		}
		return iStart;
	}

	void stepVDS_IDS::reverseV()
	{
		double temp = startV_;
		startV_ = stopV_;
		stopV_ = temp;
		stepV_ = -1*stepV_;
	}

	int stepVDS_IDS::saveData(std::string fn, double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray)
	{
		
		//Data directory
		std::string direc = "C:/Documents and Settings/NanoHall_User/My Documents/Data-Kristin/Keithley/";
		//save as text file
		std::string frmt = ".csv";

		//Build file name for data and a second one to save test parameters
		std::string fp = direc + fn + frmt;
		std::string fpP = direc + fn + "_Param" + frmt;

		std::ofstream myfile;
		myfile.open(fp);
		myfile<<"vForce [V], iMeas [A], time [ms], dTime [ms]\n";
		for (int i = 0; i<sizeArray; i++)
		{
			myfile<<vFs[i]<<','<<iMs[i]<<','<<tMs[i]<<','<<dMs[i]<<"\n";
		}
		myfile.close();

		//std::string fnP = "P_" + fn;
		std::ofstream myfile2;
		myfile2.open(fpP);
		myfile2<<"stepVDS_IDS: Parameters\n";
		myfile2<<"dt [ms], stepTime [s], startV [V], stopV [V], stepV [V], "
			"constV [V], nCycles, fullCycle, lRange, range, comp, "
			"intTime, stepSMU, measSMU\n";
		myfile2<<stepP_.dt<<","<<stepP_.measTime<<','<<startV_<<','<<stopV_<<','<<stepV_<<', [';
		for (int smu = 0; smu<4; smu++){
			if (smu != stepSMU_){
				myfile2<<stepP_.appV[smu]<<',';
			}
		}
		myfile2 <<"],"<< nCycles_<<','<<fullCycle_<<','
				<<stepP_.lRange<<','<<stepP_.range<<','<<stepP_.comp<<','
				<<stepP_.intTime<<',' << stepSMU_<<','
				<<stepP_.measSMU<<"\n";
		myfile2.close();
		
		return 0;
	}
	stepVDS_IDS::~stepVDS_IDS(){
		delete step_;
	}


	pulseVGS_IDS::pulseVGS_IDS(const pulseVGS_IDSParameters &entries){

		//Set all private memeber variables to inputs

		pulseP_.dt  = entries.dt;
		for (int smu=0; smu<4; smu++){
			pulseP_.appV[smu] = entries.appV[smu];
		}
		pulseP_.measSMU = entries.measSMU;

		pulseP_.lRange = entries.lRange;
		pulseP_.range = entries.range;
		pulseP_.comp = entries.comp;
		pulseP_.intTime = entries.intTime;


		pulseP_.measTime = entries.initTime;

		initTime_ = entries.initTime;
		pulseTime_ = entries.pulseTime;
		stepTime_ = entries.stepTime;

		pulseOffV_ = entries.pulseOffV;
		pulseV_ = entries.pulseV;
		nPulses_ = entries.nPulses;
		pulseSMU_ = entries.pulseSMU;

		//Calculate size needed for each step

		pulse_ = new KT::ktConst(pulseP_);

		initSize_ = pulse_->arraySizeNeeded();
		std::cout<<"init size: " << initSize_<<std::endl;
		pulse_->setMeasTime(stepTime_);
		stepSize_ = pulse_->arraySizeNeeded();
		std::cout<<"step size: " << stepSize_<<std::endl;
		pulse_->setMeasTime(pulseTime_);
		pulseSize_ = pulse_->arraySizeNeeded();
		std::cout<<"pulse size: " << pulseSize_<<std::endl;
		if (pulseSize_ < 2){ 
			std::cout<<"Need longer pulse"<<std::endl;
		}
		sizeArrayNeeded_ = initSize_  + (stepSize_ + pulseSize_ - 2)*nPulses_ + nPulses_  -1;
		runTime_ = sizeArrayNeeded_ * pulseP_.dt*1e-3;
	}

	int pulseVGS_IDS::arraySizeNeeded(){
		return sizeArrayNeeded_;
	}

	int pulseVGS_IDS::runProgram(double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray)
	{
		if (sizeArray != sizeArrayNeeded_){
			std::cout<<"Incorrect Array size"<<std::endl;
			return 1;
		}

		time_t tstart;
		time_t tend;
		time(&tstart);
		tend = tstart+runTime_;
		std::cout<<"Program began at: " <<ctime(&tstart);
		std::cout<<"Program will finish at: "<<ctime(&tend);

		//Set initial measurement time
		pulse_->setMeasTime(initTime_);

		//Set initial indice
		int iStart = 0;

		pulse_->runTest(iMs, tMs, dMs, initSize_, iStart);
		iStart = iStart + initSize_ -1;

		for (int i = 0; i<nPulses_; i++){
			pulse_->setMeasTime(pulseTime_);
			pulse_->setV(pulseSMU_, pulseV_);
			pulse_->runTest(iMs, tMs, dMs, pulseSize_, iStart);
			pulse_->setV(pulseSMU_, pulseOffV_);
			iStart = iStart + pulseSize_-1;
			pulse_->setMeasTime(stepTime_);
			pulse_->runTest(iMs, tMs, dMs, stepSize_, iStart);
			//std::cout<<"pre: "<<iStart<<std::endl;
			iStart = iStart + stepSize_;
			//std::cout<<iStart<<std::endl;
		}
		//Fill in vFs

		iStart = 0;
		int iV = pulseOffV_;

		for (int k = 0; k<initSize_; k++){
			vFs[k] = pulseOffV_;
		}
		iStart = iStart + initSize_ - 1;

		for (int i = 0; i<nPulses_; i++){
			for (int k = iStart; k<(iStart+pulseSize_); k++)
			{
				vFs[k] = pulseV_;
			}
			iStart = iStart + pulseSize_ - 1;
			for (int k = iStart; k<(iStart+stepSize_); k++)
			{
				vFs[k] = pulseOffV_;
				//std::cout<<"k: "<<k<<std::endl;
			}
			iStart = iStart + stepSize_ ;
		}

		return 0;
	}


	int pulseVGS_IDS::saveData(std::string fn, double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray)
	{
		//Data Directory
		std::string direc = "C:/Documents and Settings/NanoHall_User/My Documents/Data-Kristin/Keithley/";
		//save as text file
		std::string frmt = ".csv";
		
		std::string fp = direc + fn + frmt;
		std::string fpP = direc + fn + "_Param" + frmt;

		std::ofstream myfile;
		myfile.open(fp);
		myfile<<"vForce [V], iMeas [A], time [ms], dTime [ms]\n";
		for (int i = 0; i<sizeArray; i++)
		{
			myfile<<vFs[i]<<','<<iMs[i]<<','<<tMs[i]<<','<<dMs[i]<<"\n";
		}
		myfile.close();

		std::ofstream myfile2;
		myfile2.open(fpP);
		myfile2<<"pulseVGS_IDS: Parameters\n";
		myfile2<<"dt [ms], initTime [s], pulseTime [s], stepTime [s],"
			" pulseV [V], pulseOffV [V], constV [V], nPulses, lRange, range,"
			"comp, intTime, pulseSMU, measSMU\n";
		myfile2<<pulseP_.dt <<','<< initTime_<<','<< pulseTime_ <<','<<stepTime_<<','
			<<pulseV_<<','<<pulseOffV_<<', [';
		for (int smu=0; smu<4; smu++){
			if (smu !=pulseSMU_){
				myfile2<<pulseP_.appV[smu]<<',';
			}
		}

		myfile2<< "],"<<nPulses_<<','<< pulseP_.lRange<<','
				<<pulseP_.range<<','<<pulseP_.comp<<','<<pulseP_.intTime<<','
				<<pulseSMU_<<','<<pulseP_.measSMU<<"\n";
		myfile2.close();

		return 0;
	}

	pulseVGS_IDS::~pulseVGS_IDS(){
		delete pulse_;
	}



}