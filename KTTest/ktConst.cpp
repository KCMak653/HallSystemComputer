#include "ktCmd.h"
#include "ktConst.h"
#include <string.h>
#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>
#include<time.h>

namespace KT
{
	ktConst::ktConst(const constParameters &entries)
	{
		dt_ = entries.dt;
		measTime_ = entries.measTime;
		for (int i = 0; i <4; i++){
			appV_[i] = entries.appV[i];
			forceMode_[i] = entries.forceMode[i];
			measMode_[i] = entries.measMode[i];
			range_[i] = entries.range[i];
			comp_[i] = entries.comp[i];
		}
		//constSMU_ = entries.constSMU;
		//measSMU_ = entries.measSMU;
		lRange_ = entries.lRange;
		//range_ = entries.range;
		//comp_ = entries.comp;
		intTime_ = entries.intTime;
		//nChannels_ = entries.nChannels;

		//Open handle to Keithley
		keith_ = new KT::ktCmd();

		//Set the machine parameters: Compliance, range, integration time 
		
		//keith_->setForceMode(forceMode_);
		//keith_->setMeasMode(measMode_);
		//keith_->setComp(measSMU_,comp_);
		keith_->setLRange(measSMU_, lRange_);
		//keith_.setRange(measSMU_, range_); Command not recognized?????
		keith_->setIntTime(intTime_);


		//Set the minimum time of each measurement based on the
		//desired integration time
		if (intTime_ == 1){
			dtMin_ = 50 * nChannels_; //ms
		}
		else if (intTime_ == 2) {
			dtMin_ = 100 * nChannels_; //ms
		}
		else if (intTime_ == 3) {
			dtMin_ = 700 * nChannels_; //ms
		}
		else 
		{
			std::cout<<"Invalid time integration mode"<<std::endl;

			return;
		}

		if (dtMin_ > dt_){
			std::cout<<"Measurement frequency too high for integration time"<<std::endl;
			return;
		}

		//Calculate the size of array needed to record measurements
		sizeArrayNeeded_ = measTime_*1e3/dt_+1.5;
	
		//Initialize chMeas array which keeps channels to measure in order - don't have to keep checking all of measMode
		int k = 0;
		for (int i = 0; i<4; i++){
			if ((measMode_[i] == 'I') | (measMode_[i] == 'V')){
				chMeas_[k] = i+1;
				k++;
			}
		}
		nMeasChannels_=k;

		//Force all SMUs to 0V
		keith_->srcZeroAll();
	}
	/*
	int ktConst::forceConstV(int constSMU2, double vConst)
	{
		keith_->vForce(constSMU2, vConst);
		return 0;
	}
	*/
	//Sets range, comp, mode and applies initial bias for all channels
	int ktConst::initializeChannels()
	{
		for (int i = 0; i<4; i++)
		{
			if ((forceMode_[i] == 'I') | (forceMode_[i] == 'V'))
			{
				keith_->setForceMode(forceMode_[i]);
				keith_->setRange(range_[i]);
				keith_->setComp(comp_[i]);
				keith_->ivForce((i+1), appV_[i]);
			}
		}
	}


	int ktConst::setForceParams(int SMU)
	{
		keith_->setForceMode(forceMode_[SMU-1]);
		keith_->setRange(range_[SMU-1]);
		keith_->setComp(comp_[SMU - 1]);
	}


	int ktConst::runTest(double iMs[], double tMs[], int dMs[], int sizeArray, int iStart)
	{
		//First ensure that the correct array size has been passed as argument
		if (sizeArrayNeeded_ != sizeArray){
			std::cout<<"Wrong array size"<<std::endl;
			return 1;
		}		

		int delayT;
		
		//double v = constV_;
		//Initiate the timer
		clock_t clk = clock();
		clock_t clk2 = clk;
		
		//Apply constant bias
		//std::cout<<"V: "<<constV_<<std::endl;
		// for (int smu = 0; smu<4; smu++){
		// 	keith_->ivForce(smu+1,appV_[smu]);
		// }
		
		
		//Run the sweep. 

		//Measure I and time
		//Delay for the remainder of the measurment time
		
		//vFs[iStart] = v;

		//Measure and store current
		for (int n=0; n<nMeasChannels_; n++){
			keith_->ivMeas(chMeas_[n], measMode[chMeas[n]-1], iMs[n]);
		}
		
		//Record the time
		tMs[iStart] = (double)(clock());

		//Calculate amount of time to delay
		delayT = dt_ - tMs[iStart];
		
		//Set delay to zero if negative
		if (delayT < 0) {delayT = 0;}

		//Record delay
		dMs[iStart] = delayT;

		//Sleep for remainder of measurement period
		Sleep(delayT);
		
		//Repeat for length of array, since multiple sweeps may occur,
		//the indices of where to store data in array will differe by iStart
		for (int i=(iStart+1); i<(iStart+sizeArray); i++){
			//vFs[i] = v;
			for (int n = 0; n<nChannels; n++){
				keith_->ivMeas(chMeas_[n], measMode[chMeas[n]-1], iMs[i*nMeasChannels_ + n]);
			}
			tMs[i] = (double)(clock());
			delayT = dt_*(i+1) - tMs[i];
			if (delayT < 0) {delayT = 0;}
			dMs[i] = delayT;
			Sleep(delayT);
		}
		
		return 0;
	}

	int ktConst::arraySizeNeeded(){

		return sizeArrayNeeded_;
	}

	int ktConst::setIV(bool SMU[], double v)
	{
		for (int i =0; i<4; i++){
			if (SMU[i])){
				keith_-> iVForce((i+1), v);
			}
		}
		return 0;
	}

	int ktConst::setMeasTime(double t)
	{
		measTime_ = t;
		sizeArrayNeeded_ = measTime_*1e3/dt_+1.5;
		return 0;
	}
	
	ktConst::~ktConst(){
		keith_->srcZeroAll();
		delete keith_;
	}
}

