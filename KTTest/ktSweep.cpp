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
			dtMeas_ = 600; //ms
		}
		else 
		{
			std::cout<<"Invalid time integration mode"<<std::endl;

			return;
		}
		//dtMeas_ =0.5;
		stepV_ = SR_*dtMeas_*1e-3;
		sizeArrayNeeded_ = (fabs(startV_ - stopV_)/(stepV_))+1.5;
		keith_.srcZeroAll();
	}

	int ktSweep::runSweep(double vFs[], int sizeArray, double iMs[], double tMs[]){
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
		vFs[0] = v;
		v = v + stepV_;		
		keith_.iMeas(SMU_, iMs[0]);
		tMs[0] = (double)(clock());
		delayT = dtMeas_ - tMs[0];
		std::cout<<delayT;
		if (delayT < 0) {delayT = 0;}
		Sleep(delayT);
		
		for (int i=1; i<sizeArray; i++){
			//clk = clock();
			keith_.vForce(SMU_,v);
			vFs[i] = v;
			v = v + stepV_;		
			keith_.iMeas(SMU_, iMs[i]);
			tMs[i] = (double)(clock());
			delayT = dtMeas_ - (tMs[i]-tMs[i-1]);
			std::cout<<delayT<<std::endl;
			if (delayT < 0) {delayT = 0;}
			Sleep(delayT);
		
			
			//std::cout<<v<<std::endl;
		}
		keith_.srcZeroAll();
		return 0;
	}

	int ktSweep::arraySizeNeeded(){
		return sizeArrayNeeded_;
	}
}
