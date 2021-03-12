#include "mcCmd.h"
#include <string>
#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>


#include "C:\Program Files\National Instruments\Shared\ExternalCompilerSupport\C\include\ni4882.h"


#define ARRAYSIZE          1024     // Size of read buffer

//GPIB communication parameters:
#define BDINDEX               0     // Board Index
#define PRIMARY_ADDR_OF_DMM   6     // Primary address of device
#define NO_SECONDARY_ADDR     0     // Secondary address of device
#define TIMEOUT               T10s  // Timeout value = 10 seconds
#define EOTMODE               1     // Enable the END message
#define EOSMODE               1     // Disable the EOS mode


char ValueStrMC[ARRAYSIZE + 1];
char ErrorMnemonicMC[29][5] = { "EDVR", "ECIC", "ENOL", "EADR", "EARG",
                              "ESAC", "EABO", "ENEB", "EDMA", "",
                              "EOIP", "ECAP", "EFSO", "",     "EBUS",
                              "ESTB", "ESRQ", "",     "",      "",
                              "ETAB", "ELCK", "EARM", "EHDL",  "",
                              "",     "EWIP", "ERST", "EPWR" };

const int moveArrSize = 3;
const int setRefPtArrSize = 3;

const int moveOriginArrSize = 3;

const int setSpeedInd = 2;
const int setSpeedArrSize = 7;

const int setNStepsInd = 2;
const int setNStepsArrSize = 9;

const int moveRefPtInd = 2;
const int moveRefPtArrSize = 9;

const int setDirectionInd = 0;
const int setDirectionSizeArr = 3;


namespace MC
{	//Constructor: Define initial command character arrays
	mcCmd::mcCmd(){
		
		static char const moveCMD[] = "MW\r";
		strcpy(moveCMD_, moveCMD);
		static char const setSpeedCMD[] = "RW0050\r";
		strcpy(setSpeedCMD_, setSpeedCMD); 
		static char setNStepsCMD[] = "NW000100\r";
		strcpy(setNStepsCMD_, setNStepsCMD);
		static char moveRefPtCMD[] = "PW000100\r";
		strcpy(moveRefPtCMD_, moveRefPtCMD);
		static char setDirectionCMD[] = "-W\r";
		strcpy(setDirectionCMD_, setDirectionCMD);
		static char setRefPtCMD[] ="AW\r";
		strcpy(setRefPtCMD_, setRefPtCMD);
		static char moveOriginCMD[]= "OW\r";
		strcpy(moveOriginCMD_, moveOriginCMD);

		//std::cout<<"cmd: "<<moveRefPtCMD_<<std::endl;
		

		//Open handle to Keithley
		Dev_ = ibdev(BDINDEX, PRIMARY_ADDR_OF_DMM, NO_SECONDARY_ADDR,
               TIMEOUT, EOTMODE, EOSMODE);
		ibconfig(Dev_, IbcEOSchar, 13);
		ibconfig(Dev_, IbcEOSrd, 1);
				if (Ibsta() & ERR)
		{
			printf("Unable to open device\nibsta = 0x%x iberr = %d\n",
				Ibsta(), Iberr());
			mcStatus = 1;
			return;
		}
		else
		std::cout<<"Micro-controller properly initialized"<<std::endl;

		ibclr(Dev_);
		if (Ibsta() & ERR)
			{
				GPIBCleanup("Unable to clear device");
				mcStatus = 1;
			}
		
		
		//ibwrt(Dev_, "NW0200\r", 7);
		//ibwrt(Dev_, "+W\r", 3);
		//ibwrt(Dev_, "RW500\r",6);
		ibwrt(Dev_, "-W\r", 3);
		//std::cout<<"Writing command"<<std::endl;
		ibwrt(Dev_, setRefPtCMD_, 3);
		ibwrt(Dev_, "-W\r", 3);
		//ibwrt(Dev_, "AW\r", 3);
		position_ = 0;
		currDirec_ = -1;
		//ibwrt(Dev_, setNStepsCMD_, setNStepsArrSize);
		//ibwrt(Dev_, "MW\r", 3);
		//ibwrt(Dev_, "PW-370\r", 7);
		if (Ibsta() & ERR) {
		  GPIBCleanup("ibwrt Error");
		}
		mcStatus = 0;
	}

	void mcCmd::GPIBCleanup(const char * ErrorMsg)
	{
		std::cout<<"Error : "<<ErrorMsg<<std::endl;
		std::cout<<"ibsta = 0x"<< Ibsta()<< " iberr = "<<Iberr()<<
			"("<<ErrorMnemonicMC[Iberr()]<<")"<<std::endl;
		if (mcStatus)
		{
			std::cout<<"Cleanup: Taking device off-line"<<std::endl;
			ibonl(Dev_, 0);

		}
	}

	int mcCmd::getPosition()
	{
		return position_;
	}
	
	int mcCmd::setSpeed(int speed)
	{
		std::string strVal = std::to_string((long long) speed);
		int numLen = strVal.length();

		for (int i=setSpeedInd; i<(setSpeedArrSize-1); i++){
			if (i < (setSpeedArrSize -numLen-1))
			{
				setSpeedCMD_[i] ='0';
			} else {
				setSpeedCMD_[i] = strVal[i-(setSpeedArrSize - numLen - 1)];
			}
		}

		ibwrt(Dev_, setSpeedCMD_, setSpeedArrSize);
		if (Ibsta() & ERR)
		{
			GPIBCleanup("Unable to set speed");
			mcStatus = 1;
			return mcStatus;
		}
		std::cout<<"Spped CMD: "<<setSpeedCMD_<<std::endl;
		return 0;
	}


	int mcCmd::setNSteps(int steps)
	{
		steps_ = steps;
		std::string strVal = std::to_string((long long) steps);
		int numLen = strVal.length();


		for (int i=setNStepsInd; i<(setNStepsArrSize-1); i++){
			if (i < (setNStepsArrSize -numLen-1))
			{
				setNStepsCMD_[i] ='0';
			} else {
				setNStepsCMD_[i] = strVal[i-(setNStepsArrSize - numLen - 1)];
			}
		}

		ibwrt(Dev_, setNStepsCMD_, setNStepsArrSize);
		if (Ibsta() & ERR)
		{
			GPIBCleanup("Unable to set step number");
			mcStatus = 1;
			return mcStatus;
		}
		std::cout<<"NSteps CMD: "<<setNStepsCMD_<<std::endl;
		return 0;
	}
	
	int mcCmd::moveRefPt(int pos)
	{
		//ibwrt(Dev_, "AW\r", 3);
		std::string strVal = std::to_string((long long) pos);
		std::cout<<"StrVal: " <<strVal<<std::endl;
		int numLen = strVal.length();
		position_ = pos;
		for (int i=moveRefPtInd; i<(moveRefPtArrSize-1); i++){
			if (i < (moveRefPtArrSize -numLen-1))
			{
				moveRefPtCMD_[i] ='0';
			} else {
				moveRefPtCMD_[i] = strVal[i-(moveRefPtArrSize - numLen - 1)];
			}
		}

		ibwrt(Dev_, moveRefPtCMD_, moveRefPtArrSize);
		if (Ibsta() & ERR)
		{
			GPIBCleanup("Unable to set step number");
			mcStatus = 1;
			return mcStatus;
		}
		std::cout<<"MoveRefPt CMD: "<<moveRefPtCMD_<<std::endl;
		return 0;
	}



	int mcCmd::setDirection(int direction)
	{
		if (direction == -1){
			ibwrt(Dev_, "-W\r", 3);
			currDirec_ = -1;
			if (Ibsta() & ERR)
			{
				GPIBCleanup("Unable to change direction");
				mcStatus = 1;
				return mcStatus;
			}
			return 0;
		}
		else if (direction == 1){
			ibwrt(Dev_, "+W\r", 3);
			currDirec_ = 1;
			if (Ibsta() & ERR)
			{
				GPIBCleanup("Unable to change direction");
				mcStatus = 1;
				return mcStatus;
			}
			return 0;
		}
	}

	int mcCmd::reverseDirection(){
		if (currDirec_ == 1) setDirection(-1);
		else if (currDirec_ == -1) setDirection(1);
		return 0;
	}

	int mcCmd::move() {
		ibwrt(Dev_, moveCMD_, 3);
		position_ = position_ + currDirec_ * steps_;
		if (Ibsta() & ERR)
		{
			GPIBCleanup("Unable to move");
			mcStatus = 1;
			return mcStatus;
		}
		//std::cout<<"Spped CMD: "<<setSpeedCMD_<<std::endl;
		return 0;
	}

	int mcCmd::setRefPt(){
		ibwrt(Dev_, setRefPtCMD_, 3);
		position_=0;
		if (Ibsta() & ERR)
		{
			GPIBCleanup("Unable to set ref point");
			mcStatus = 1;
			return mcStatus;
		}
		//std::cout<<"Spped CMD: "<<setSpeedCMD_<<std::endl;
		return 0;
	}


	int mcCmd::moveOrigin() {
		ibwrt(Dev_, moveOriginCMD_, 3);
		if (Ibsta() & ERR)
		{
			GPIBCleanup("Unable to move");
			mcStatus = 1;
			return mcStatus;
		}
		//std::cout<<"Spped CMD: "<<setSpeedCMD_<<std::endl;
		return 0;
	}
	mcCmd::~mcCmd() {
		   
    // The device is taken offline.
		char c;
		ibonl(Dev_, 0);
		std::cout<<"Micro-controller has been shut down"<<std::endl;
		std::cin>>c;
	}
}