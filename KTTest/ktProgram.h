#pragma once

//Include for structure
//#include "ktSweep.h"

struct sweepVDS_IDSParameters
{
	double startV, stopV, SR;
	int lRange, range, comp, intTime;
	int sweepSMU, nCycles;
	bool fullCycle;
};


namespace KT 
{
	class sweepVDS_IDS
	{
	public:
		sweepVDS_IDS(const sweepVDS_IDSParameters &entries);
		int runProgram(double vFs[], int sizeArray, double iMs[], double tMs[], int dMs[]);
		int arraySizeNeeded();

	private:
		sweepParameters sweepP_;
		KT::ktSweep* swp_;
		int swpSize_;
		int nCycles_;
		bool fullCycle_;
		int sizeArrayNeeded;
	};
}