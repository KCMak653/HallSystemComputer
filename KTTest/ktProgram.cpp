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
		//Set all private member variables to inputs
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
		
		
		//Initialize the sweep object
		swp_ = new KT::ktSweep(sweepP_);

		//Find the size of array needed to store values
		swpSize_ = swp_->arraySizeNeeded();
	}

	int sweepVDS_IDS::arraySizeNeeded()
	{
		int sizeArrayNeeded_ = (swpSize_ - 1)*(int(fullCycle_)+1)*nCycles_
				+(1-int(fullCycle_))*nCycles_+1;
		return sizeArrayNeeded_;
	}

	int sweepVDS_IDS::runProgram(double vFs[], int sizeArray, double iMs[], double tMs[], int dMs[])
	{
		//Set initial indice
		int iStart = 0;

		//Execute outer for loop for number of cycles requested
		//Execute inner loop only if full cycle 
		for (int j = 0; j<nCycles_; j++)
		{
			//Run the sweep
			swp_->runSweep(vFs, swpSize_, iMs, tMs, dMs, iStart);
			//Change the array indice
			iStart = iStart + swpSize_ - 1;
			if (fullCycle_)
			{
				//Reverse the direction of the sweep
				swp_->reverseV();
				swp_->runSweep(vFs, swpSize_, iMs, tMs, dMs, iStart);
				swp_->reverseV();
				iStart = iStart + swpSize_ - 1;
			}
			
		}
		return 0;
	}

	sweepVDS_IDS::~sweepVDS_IDS(){
		delete swp_;
	}
}