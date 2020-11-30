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
		constV_ = entries.constV;
		constSMU_ = entries.constSMU;
		measSMU_ = entries.measSMU;
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


		//Set the minimum time of each measurement based on the
		//desired integration time
		if (intTime_ == 1){
			dtMin_ = 50; //ms
		}
		else if (intTime_ == 2) {
			dtMin_ = 100; //ms
		}
		else if (intTime_ == 3) {
			dtMin_ = 700; //ms
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
		
		//Force all SMUs to 0V
		keith_->srcZeroAll();
	}

	int ktConst::runTest(double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray, int iStart)
	{
		//First ensure that the correct array size has been passed as argument
		if (sizeArrayNeeded_ != sizeArray){
			std::cout<<"Wrong array size"<<std::endl;
			return 1;
		}
		int delayT;
		
		double v = constV_;
		//Initiate the timer
		clock_t clk = clock();
		clock_t clk2 = clk;
		
		//Apply constant bias
		std::cout<<"V: "<<constV_<<std::endl;
		keith_->vForce(constSMU_,constV_);
		
		
		//Run the sweep. 

		//Measure I and time
		//Delay for the remainder of the measurment time
		
		vFs[iStart] = v;

		//Measure and store current
		keith_->iMeas(measSMU_, iMs[iStart]);
		
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
			vFs[i] = v;
			keith_->iMeas(measSMU_, iMs[i]);
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

	int ktConst::setV(double v)
	{
		constV_ = v;
		return 0;
	}

	ktConst::~ktConst(){
		keith_->srcZeroAll();
		delete keith_;
	}
}

