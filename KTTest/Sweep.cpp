#include "Sweep.h"
#include<string.h>

namespace Keithley
{/*
	int Sweep::setParameters(struct sweepParameters &entries)
	{
		startV_ = entries.startV;
		stopV_ = entries.stopV;
		SR_ = entries.SR;
		return 0;
	}

	int Sweep::runSweep(int Dev, int nPts)
	{
		srcCmd cmd2;
		cmd2.srcCh="DV3";
		cmd2.srcMode = 1;
		cmd2.srcVal = 0.1;
		cmd2.comp =1e-3;
		std::string srcMode_str = std::to_string(static_cast<long long>(cmd2.srcMode));
		std::string srcVal_str = std::to_string(static_cast<long double>(cmd2.srcVal));
		std::string comp_str = std::to_string(static_cast<long double>(cmd2.comp));
		
		std::string cmdStr = cmd2.srcCh + ", " + srcMode_str+","+srcVal_str + "," +comp_str;
		std::cout<<cmdStr<<std::endl;
		std::cout<<cmd2.srcVal<<std::endl;
		//double vForce[nPts];
		//double iMeas[nPts];
		//double vF = startV;
		//string vF_str = std::to_string(vF);
		const int cmdLength = cmdStr.length();
		std::cout<<cmdLength<<std::endl;
		//char cmd[cmdLength+1];
		//strcpy(cmd, cmdStr.c_str());
		//ibwrt(Dev, "US", 2);
		//ibwrt(Dev, cmd, cmdLength);
		/*for (int i =0; i<nPts; i++)
		{
			char cmd[] = "DV3, 1, 0.1, 1.0E-3";
			length = strlen(cmd);
			ibwrt(Dev, cmd, length);
			ibwrt(Dev, "TI3", 3);

		std::cout<<"V: "<<startV_ <<std::endl;
		std::cout<<"stopV: "<<stopV_ <<std::endl;
		std::cout<<"SR: "<<SR_<<std::endl;*/
		//return 0;
	}

}