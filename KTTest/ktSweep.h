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
		int runSweep(double vFs[], int sizeArray, double iMs[], double tMs[]);
		int arraySizeNeeded();

	private:
		double startV_;
		double stopV_;
		double SR_;
		unsigned int lRange_;
		unsigned int range_;
		unsigned int comp_;
		unsigned int intTime_;
		KT::ktCmd keith_;
		double dtMeas_;
		double stepV_;
		int sizeArrayNeeded_;
		int SMU_;

	};
}