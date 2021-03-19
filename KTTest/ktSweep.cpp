//Source file for Keithley base sweep class

#include "ktCmd.h"
#include "ktSweep.h"
#include <string.h>
#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>
#include<time.h>



namespace KT
{
	ktSweep::ktSweep(const sweepParameters &entries){
		//Set all private member variables from the inputted parameters
		sweepSMU_ = entries.sweepSMU;
		constSMU_ = entries.constSMU;
		measSMU_ = entries.measSMU;
		SR_ = entries.SR;
		startV_ = entries.startV;
		stopV_ = entries.stopV;
		constV_ = entries.constV;
		lRange_ = entries.lRange;
		range_ = entries.range;
		comp_ = entries.comp;
		intTime_ = entries.intTime;

		//Open handle to Keithley
		keith_ = new KT::ktCmd();

		//Set the machine parameters: Compliance, range, integration time 
		keith_->setComp(measSMU_,comp_);
		keith_->setLRange(measSMU_, lRange_);
		//keith_.setRange(measSMU_, range_); Command not recognized?????
		keith_->setIntTime(intTime_);

		//Set the time of each measurement based on the
		//desired integration time
		if (intTime_ == 1){
			dtMeas_ = 50; //ms
		}
		else if (intTime_ == 2) {
			dtMeas_ = 100; //ms
		}
		else if (intTime_ == 3) {
			dtMeas_ = 700; //ms
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
		
		//Force all SMUs to 0V
		keith_->srcZeroAll();
	}
	int ktSweep::setSR(double SR){
		SR_ = SR;
		return 0;
	}

	int ktSweep::runSweep(double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray, int iStart){
		//First ensure that the correct array size has been passed as argument
		if (sizeArrayNeeded_ != sizeArray){
			std::cout<<"Wrong array size"<<std::endl;
			return 1;
		}
		
		//Apply constant bias if there is one:
		if ((constSMU_!=0) & (constSMU_ != sweepSMU_)){
			keith_->ivForce(constSMU_,constV_);
		}
		
		//Run the sweep. 

		//Force V, then measure I and time
		//Delay for the remainder of the measurment time
		//increment the voltage and repeat
		
		int delayT;

		//initalize voltage to start of sweep
		double v = startV_;
		
		//Initiate the timer
		clock_t clk = clock();
		clock_t clk2 = clk;
		
		//Force voltage
		keith_->ivForce(sweepSMU_,v);

		//Record voltage
		vFs[iStart] = v;

		//Increment voltage
		v = v + stepV_;		
		
		//Measure and store current
		keith_->ivMeas(measSMU_, iMs[iStart]);
		
		//Record the time
		tMs[iStart] = (double)(clock());

		//Calculate amount of time to delay
		delayT = dtMeas_ - tMs[iStart];
		
		//Set delay to zero if negative
		if (delayT < 0) {delayT = 0;}

		//Record delay
		dMs[iStart] = delayT;

		//Sleep for remainder of measurement period
		Sleep(delayT);
		
		//Repeat for length of array, since multiple sweeps may occur,
		//the indices of where to store data in array will differe by iStart
		for (int i=(iStart+1); i<(iStart+sizeArray); i++){
			
			keith_->ivForce(sweepSMU_,v);
			vFs[i] = v;
			v = v + stepV_;		
			keith_->ivMeas(measSMU_, iMs[i]);
			tMs[i] = (double)(clock());
			delayT = dtMeas_*(i+1) - tMs[i];
			if (delayT < 0) {delayT = 0;}
			dMs[i] = delayT;
			Sleep(delayT);
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
