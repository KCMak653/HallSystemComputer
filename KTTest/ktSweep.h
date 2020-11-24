//Keithley base sweep class

#pragma once
//#include "ktCmd.h"

struct sweepParameters
{
	double startV, stopV, SR;
	int lRange, range, comp, intTime;
	int SMU;
};

namespace KT
{
	class ktSweep
	{
	public:
		ktSweep(const sweepParameters &entries);
		//ktSweep();
		int runSweep(double vFs[], int sizeArray, double iMs[], double tMs[], int dMs[], int iStart);
		int arraySizeNeeded();
		int reverseV();

	private:
		double startV_;
		double stopV_;
		double SR_;
		int lRange_;
		int range_;
		int comp_;
		int intTime_;
		KT::ktCmd keith_;
		int dtMeas_;
		double stepV_;
		int sizeArrayNeeded_;
		int SMU_;

	};
}