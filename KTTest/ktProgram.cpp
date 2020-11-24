#include "ktCmd.h"
#include "ktSweep.h"
#include "ktProgram.h"
#include <string.h>
#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>
#include<time.h>

namespace KT
{
	sweepVDS_IDS::sweepVDS_IDS(const sweepVDS_IDSParameters &entries)
	{
		sweepP_.SMU = entries.sweepSMU;
		sweepP_.SR = entries.SR;
		sweepP_.startV = entries.startV;
		sweepP_.stopV = entries.stopV;
		sweepP_.lRange = entries.lRange;
		sweepP_.range = entries.range;
		sweepP_.comp = entries.comp;
		sweepP_.intTime = entries.intTime;

		fullCycle_ = entries.fullCycle;
		nCycles_ = entries.nCycles;
		
		//constSMU_ = entries.constSMU;

		swp_ = new KT::ktSweep(sweepP_);
		swpSize_ = swp_->arraySizeNeeded();
		std::cout<<"Sweep size is: "<<swpSize_<<std::endl;
	}

	int sweepVDS_IDS::arraySizeNeeded()
	{
		int sizeArrayNeeded_ = (swpSize_ - 1)*(int(fullCycle_)+1)*nCycles_
				+(1-int(fullCycle_))*nCycles_+1;
		return sizeArrayNeeded_;
	}

	int sweepVDS_IDS::runProgram(double vFs[], int sizeArray, double iMs[], double tMs[], int dMs[])
	{
		int iStart = 0;
		for (int j = 0; j<nCycles_; j++)
		{
			swp_->runSweep(vFs, swpSize_, iMs, tMs, dMs, iStart);
			iStart = iStart + swpSize_ - 1;
			if (fullCycle_)
			{
				swp_->reverseV();
				swp_->runSweep(vFs, swpSize_, iMs, tMs, dMs, iStart);
				//return 0;
				swp_->reverseV();
				iStart = iStart + swpSize_ - 1;
			}
			//std::cout<<"iStart: "<<iStart<<std::endl;
		}
		return 0;
	}
}