//Basic Sweep Class

#pragma once
#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>
#include<string>

#include "C:\Program Files\National Instruments\Shared\ExternalCompilerSupport\C\include\ni4882.h"

/*
int ARRAYSIZE_RD 1024;//Size of read buffer
double stepTime 1e-3;//Size of step, 1ms 
*/
struct sweepParameters
{
	double startV, stopV, SR;
};

struct srcCmd
{
	std::string srcCh;
	int srcMode;
	double srcVal;
	double comp;
};

namespace Keithley
{
	class Sweep
	{/*
	public:
		int setParameters(struct sweepParameters &entries);
		int runSweep(int Dev, int nPts);
	private:
		double startV_;
		double stopV_;
		double SR_;
		char vForceCMD_[] = "DV3, 1,+0.10000e+001, 1.0E-3";*/
	};
}

