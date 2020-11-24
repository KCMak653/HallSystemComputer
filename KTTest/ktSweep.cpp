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
		SMU_ = entries.SMU;
		SR_ = entries.SR;
		startV_ = entries.startV;
		stopV_ = entries.stopV;
		lRange_ = entries.lRange;
		range_ = entries.range;
		comp_ = entries.comp;
		intTime_ = entries.intTime;

		keith_.setComp(SMU_,comp_);
		keith_.setLRange(SMU_, lRange_);
		//keith_.setRange(SMU_, range_); Command not recognized?????
		keith_.setIntTime(intTime_);
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
		//dtMeas_ =0.5;
		stepV_ = SR_*dtMeas_*1e-3;
		sizeArrayNeeded_ = (fabs(startV_ - stopV_)/(stepV_))+1.5;
		if (startV_ > stopV_) stepV_ = -1*stepV_;
		std::cout<<"stepV is: "<<stepV_<<std::endl;
		keith_.srcZeroAll();
	}

	int ktSweep::runSweep(double vFs[], int sizeArray, double iMs[], double tMs[], int dMs[], int iStart){
		if (sizeArrayNeeded_ != sizeArray){
			std::cout<<"Wrong array size"<<std::endl;
			return 1;
		}
		int delayT;
		double v = startV_;
		//keith_.srcZeroAll();
		clock_t clk = clock();
		clock_t clk2 = clk;
		
		keith_.vForce(SMU_,v);
		vFs[iStart] = v;
		v = v + stepV_;		
		keith_.iMeas(SMU_, iMs[iStart]);
		//clk2 = clock();
		tMs[iStart] = (double)(clock());
		delayT = dtMeas_ - tMs[iStart];
		//std::cout<<delayT;
		if (delayT < 0) {delayT = 0;}
		dMs[iStart] = delayT;
		Sleep(delayT);
		
		for (int i=(iStart+1); i<(iStart+sizeArray); i++){
			//clk = clock();
			keith_.vForce(SMU_,v);
			vFs[i] = v;
			v = v + stepV_;		
			keith_.iMeas(SMU_, iMs[i]);
			tMs[i] = (double)(clock());
			delayT = dtMeas_*(i+1) - tMs[i];
			//std::cout<<delayT<<std::endl;
			if (delayT < 0) {delayT = 0;}
			dMs[i] = delayT;
			Sleep(delayT);
		
			
			//std::cout<<v<<std::endl;
		}
		//keith_.srcZeroAll();
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

}
