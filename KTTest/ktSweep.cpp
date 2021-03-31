//Source file for Keithley base sweep class

#include "ktCmd.h"
#include "ktSweep.h"
#include <string.h>
#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>
#include<chrono>
#include<thread>



namespace KT
{
	ktSweep::ktSweep(const sweepParameters &entries){
		//Set all private member variables from the inputted parameters
		for (int i = 0; i<4; i++){
			appV_[i] = entries.appV[i];
			forceMode_[i] = entries.forceMode[i];
			measMode_[i] = entries.measMode[i];
			range_[i] = entries.range[i];
			comp_[i] = entries.comp[i];
			sweepSMU_[i] = entries.sweepSMU[i];
		}

		SR_ = entries.SR;
		startV_ = entries.startV;
		stopV_ = entries.stopV;

		lRange_ = entries.lRange;

		intTime_ = entries.intTime;
		//nChannels_ = entries.nChannels;
		//nSwChannels_ = entries.nSwChannels;
		//Open handle to Keithley
		keith_ = new KT::ktCmd();

		//Set the machine parameters: Compliance, range, integration time 
		//keith_->setComp(measSMU_,comp_);
		
		//keith_.setRange(measSMU_, range_); Command not recognized?????
		keith_->setIntTime(intTime_);
		int k = 0;
		for (int i = 0; i < 4; i++) {
			if ((measMode_[i] == 'I') | (measMode_[i] == 'V')) {
				chMeas_[k] = i + 1;
				keith_->setLRange(chMeas_[k], lRange_);
				k++;
			}
		}
		nMeasChannels_ = k;
		//Set the time of each measurement based on the
		//desired integration time
		if (intTime_ == 1){
			dtMeas_ = 50 * nMeasChannels_; //ms
		}
		else if (intTime_ == 2) {
			dtMeas_ = 100 * nMeasChannels_; //ms
		}
		else if (intTime_ == 3) {
			dtMeas_ = 700 * nMeasChannels_; //ms
		}
		else 
		{
			std::cout<<"Invalid time integration mode"<<std::endl;

			return;
		}
		
		//Set the voltage step based on measurement time and SR
		stepV_ = SR_*dtMeas_*1e-3;

		//Calculate the size of array needed to record measurements
		sizeArrayNeeded_ = (fabs(startV_ - stopV_)/(stepV_))+1.5;

		//Ensure stepV has the correct sign
		if (startV_ > stopV_) stepV_ = -1*stepV_;
		


		k = 0;
		for (int i = 0; i<4; i++){
			if (sweepSMU_[i]){
				chSweep_[k] = i+1;
				k++;
			}
		}
		nSwChannels_ = k;

		//Force all SMUs to 0V
		keith_->srcZeroAll();
	}
	int ktSweep::setSR(double SR){
		SR_ = SR;
		return 0;
	}

	int ktSweep::initializeChannels()
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
		return 0;
	}

	
	int ktSweep::setForceParams(int SMU)
	{
		keith_->setForceMode(forceMode_[SMU-1]);
		keith_->setRange(range_[SMU-1]);
		keith_->setComp(comp_[SMU - 1]);
		return 0;
	}



	int ktSweep::runSweep(double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray, int iStart){
		//First ensure that the correct array size has been passed as argument
		if (sizeArrayNeeded_ != sizeArray){
			std::cout<<"Wrong array size"<<std::endl;
			return 1;
		}
		
		//Run the sweep. 

		//Force V, then measure I and time
		//Delay for the remainder of the measurment time
		//increment the voltage and repeat
		
		long delayT;

		//initalize voltage to start of sweep
		double v = startV_;
		
		//Initiate the timer
		auto clk = std::chrono::high_resolution_clock::now();

		
		
		//Force voltage
		for (int n=0; n<nSwChannels_; n++){
			keith_->ivForce(chSweep_[n],v);
		}

		//Record voltage
		vFs[iStart] = v;

		//Increment voltage
		v = v + stepV_;		
		
		//Measure and store current
		for (int n = 0; n < nMeasChannels_; n++) {
			keith_->ivMeas(chMeas_[n], measMode_[chMeas_[n] - 1], iMs[n]);
		}
		//Record the time
		
		auto clk2 = std::chrono::high_resolution_clock::now();
		tMs[iStart] = std::chrono::duration_cast<std::chrono::milliseconds> (clk2-clk).count();

		//Calculate amount of time to delay
		delayT = dtMeas_ - tMs[iStart];
		
		//Set delay to zero if negative
		if (delayT < 0) {delayT = 0;}

		//Record delay
		dMs[iStart] = delayT;

		//Sleep for remainder of measurement period

		std::this_thread::sleep_for(std::chrono::milliseconds(delayT));

		
		//Repeat for length of array, since multiple sweeps may occur,
		//the indices of where to store data in array will differe by iStart
		for (int i=(iStart+1); i<(iStart+sizeArray); i++){
			for (int n=0; n<nSwChannels_; n++){
				keith_->ivForce(chSweep_[n],v);
			}

			vFs[i] = v;
			v = v + stepV_;
			for (int n=0; n<nMeasChannels_; n++){		
				keith_->ivMeas(chMeas_[n], measMode_[chMeas_[n]-1], iMs[i*nMeasChannels_ + n]);
			}
			clk2 = std::chrono::high_resolution_clock::now();
			tMs[i] = std::chrono::duration_cast<std::chrono::milliseconds> (clk2 - clk).count();
			delayT = dtMeas_*(i+1) - tMs[i];
			if (delayT < 0) {delayT = 0;}
			dMs[i] = delayT;
			std::this_thread::sleep_for(std::chrono::milliseconds(delayT));
		}
		
		return 0;
	}

	int ktSweep::arraySizeNeeded(){
		return sizeArrayNeeded_;
	}

	int ktSweep::reverseV(){
		double temp = startV_;
		startV_ = stopV_;
		stopV_ = temp;
		stepV_ = stepV_*-1;
		return 0;
	}
	/*
	int ktSweep::constBias(double constV)
	{
		if (constSMU_ ==0) return 1;
		keith_->vForce(constSMU_,v);
		return 0;
	}*/

	ktSweep::~ktSweep(){
		keith_->srcZeroAll();
		delete keith_;
	}

}
