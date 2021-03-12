#include "mcCmd.h"
#include "mcConst.h"
#include <string.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>
#include<time.h>

namespace MC
{
	mcConst::mcConst(double iPos)
	{
		iPos_ = iPos;
		cPos_ = iPos;
		mLSteps_ = 370;
		lLimN_ = -(iPos_ - 5)*mLSteps_;
		rLimN_ = (15 - iPos_)*mLSteps_;
		direc_ = -1;
		mc_ = new MC::mcCmd();

	}

	int mcConst::movePosition(double fPos)
	{
		if ((fPos > 15) | (fPos < 5))
		{
			std::cout<<"Out of range"<<std::endl;
			return 1;
		}
		int fPosN = (fPos -iPos_)*mLSteps_*direc_;
		std::cout<<"fPosN: " <<fPosN <<std::endl;
		std::cout<<"direc: " <<direc_ <<std::endl;
		
		if (fPosN < 0)
		{
			fPosN = fPosN*-1;
			changeDirection();
			direc_=direc_*-1;
			std::cout<<"direc: " <<direc_ <<std::endl;
		}
		
		
		mc_->setNSteps(fPosN);
		mc_->move();
		cPos_ = fPos;
		return 0;
	}

	int mcConst::move(double fPos, double time)
	{
		int fPosN = (fPos -cPos_)*mLSteps_*direc_;
		if (fPosN < 0)
		{
			fPosN = fPosN*-1;
			changeDirection();
		}
		//Need to determine minimum stepsize
		double rate = fPosN/time; //# steps per second
		int dt = 1; //Number of seconds between steps
		double timeTemp = time;
		while (rate < 1)
		{
			timeTemp = timeTemp / 2;
			dt = dt*2;
			rate = fPosN/timeTemp;
		}
		//std::cout<<"rate :"<<rate<<std::endl;
		dt = ceil(rate)/rate*dt;
		//std::cout<<"dt: "<<dt<<std::endl;
		int nSteps = ceil(rate);
		//std::cout<<"nSteps: "<<nSteps<<std::endl;
		mc_->setNSteps(nSteps);

		int nMvs = time/dt + 1;
		int delayT;

		clock_t clk = clock();
		clock_t clk2 = clk;

		for (int i=0; i<nMvs; i++)
		{
			delayT = dt*(i+1)*1e3 - (double)(clock());
			if (delayT < 0) {delayT = 0;}
			Sleep(delayT);
			std::cout<<"step"<<std::endl;
			mc_->move();
		}
		cPos_ = fPos;
		return 0;
	}



	int mcConst::changeDirection()
	{
		mc_->reverseDirection();
		direc_ = direc_ * -1;
		return 0;
	}

	mcConst::~mcConst(){
		delete mc_;
	}
}