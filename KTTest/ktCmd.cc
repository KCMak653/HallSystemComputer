#include "ktCmd.h"
#include <string>
#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>

#include "ni4882.h"
//#include "C:\Program Files\National Instruments\Shared\ExternalCompilerSupport\C\include\ni4882.h"


#define ARRAYSIZE          1024     // Size of read buffer

//GPIB communication parameters:
#define BDINDEX               0     // Board Index
#define PRIMARY_ADDR_OF_DMM   17     // Primary address of device
#define NO_SECONDARY_ADDR     0     // Secondary address of device
#define TIMEOUT               T10s  // Timeout value = 10 seconds
#define EOTMODE               1     // Enable the END message
#define EOSMODE               0     // Disable the EOS mode


char ValueStr[ARRAYSIZE + 1];
char ErrorMnemonic[29][5] = { "EDVR", "ECIC", "ENOL", "EADR", "EARG",
                              "ESAC", "EABO", "ENEB", "EDMA", "",
                              "EOIP", "ECAP", "EFSO", "",     "EBUS",
                              "ESTB", "ESRQ", "",     "",      "",
                              "ETAB", "ELCK", "EARM", "EHDL",  "",
                              "",     "EWIP", "ERST", "EPWR" };
const int ivForceVInd = 19;
const int ivForceSMUInd = 2;
const int ivForceVSize = 13;
const int ivForceArrSize = 28;
const int ivForceCompInd = 27;
const int ivForceModeInd = 1;

const int ivTrigArrSize = 3;
const int ivTrigModeInd = 1;

const int setLRangeArrSize = 10;
const int setLRangeSMUInd = 3;
const int setLRangeRangeInd = 9;

const int setRangeArrSize = 16;
const int setRangeSMUInd = 3;
const int setRangeRangeInd = 9;
const int setRangeCompInd = 15;
const int setRangeModeInd = 1;

const int setIntTimeArrSize = 3;
const int setIntTimeTimeInd = 2;


namespace KT  
{	//Constructor: Define initial command character arrays
	ktCmd::ktCmd(){
		static char const CMD[] = "DV3, 1,+0.00000e+000, 1.0E-3\0";
		strcpy(ivForceCMD_, CMD);
		static char const trigCMD[] = "TI1\0";
		strcpy(ivTrigCMD_, trigCMD); 
		static char lRangeCMD[] = "RG 1, 1E-3\0";
		strcpy(setLRangeCMD_, lRangeCMD);
		static char rangeCMD[] = "RI 1, 1E-3, 1E-3\0";
		strcpy(setRangeCMD_, rangeCMD);
		static char intTimeCMD[] ="IT1\0";
		strcpy(setIntTimeCMD_, intTimeCMD);



		
		//Open handle to Keithley
		Dev_ = ibdev(BDINDEX, PRIMARY_ADDR_OF_DMM, NO_SECONDARY_ADDR,
               TIMEOUT, EOTMODE, EOSMODE);
		if (Ibsta() & ERR)
		{
			printf("Unable to open device\nibsta = 0x%x iberr = %d\n",
				Ibsta(), Iberr());
			ktStatus = 1;
			return;
		}
		else
		std::cout<<"Keithley properly initialized"<<std::endl;

		//Clear any commands
		ibclr(Dev_);
		if (Ibsta() & ERR)
		{
			GPIBCleanup("Unable to clear device");
			ktStatus = 1;
			return;
		}
		ibwrt(Dev_, "US", 2);
		//ibwrt(Dev_, "DE CH2", 6);
		//ibwrt(Dev_, "IT1", 3);
		//ibwrt(Dev_, "RI 2, 1E-3, 1E-3", 16);
		//ibwrt(Dev_, "RG 2, 1E-3", 10);
		

		//ibwrt(Dev_, "UL", 2);
		//char cmd4[] = "RI 2, 1E-5, 1E-5";
		//ibwrt(Dev_, cmd4, strlen(cmd4));
		ktStatus = 0;
	}
	
	void ktCmd::GPIBCleanup(const char * ErrorMsg)
	{
		std::cout<<"Error : "<<ErrorMsg<<std::endl;
		std::cout<<"ibsta = 0x"<< Ibsta()<< " iberr = "<<Iberr()<<
			"("<<ErrorMnemonic[Iberr()]<<")"<<std::endl;
		if (ktStatus)
		{
			std::cout<<"Cleanup: Taking device off-line"<<std::endl;
			ibonl(Dev_, 0);

		}
	}

	int ktCmd::setComp(const int SMU, const int comp)
	{
		char j = '0' + comp;
		ivForceCMD_[ivForceCompInd] = j;
		setRangeCMD_[setRangeCompInd] = j;
		return 0;
	}

	int ktCmd::setLRange(const int SMU, const int lRange)
	{
		char j = '0' + SMU;
		setLRangeCMD_[setLRangeSMUInd] = j;
		char k = '0' + lRange;
		setLRangeCMD_[setLRangeRangeInd] = k;
		ibwrt(Dev_, setLRangeCMD_, setLRangeArrSize);
		if (Ibsta() & ERR)
		{
			GPIBCleanup("Unable to set LRange device");
			ktStatus = 1;
			return ktStatus;
		}
		return 0;
	}

	int ktCmd::setRange(const int SMU, const int range)
	{
		char j = '0' + SMU;
		setRangeCMD_[setRangeSMUInd] = j;
		char k = '0' + range;
		setRangeCMD_[setRangeRangeInd] = k;
		ibwrt(Dev_, setRangeCMD_, setRangeArrSize);
		if (Ibsta() & ERR)
		{
			GPIBCleanup("Unable to set Range");
			ktStatus = 1;
			return ktStatus;
		}
		return 0;
	}

	int ktCmd::setForceMode(const char mode)
	{
		ivForceCMD_[ivForceModeInd] = mode;
		return 0;
	}


	int ktCmd::setMeasMode(const char mode)
	{
		ivTrigCMD_[1] = mode;
		setRangeCMD_[setRangeModeInd] = mode;
		return 0;
	}

	int ktCmd::setIntTime(const int mode)
	{
		char j = '0' + mode;
		setIntTimeCMD_[setIntTimeTimeInd] = j;
		ibwrt(Dev_, setIntTimeCMD_, setIntTimeArrSize);
		return 0;
	}

	//set all SMU to force 0V
	int ktCmd::srcZeroAll(){
		updateCMD(0.0);
		for (int i=1; i<5; i++){
			char j = '0'+ i;
			ivForceCMD_[2] = j;
			ibwrt(Dev_, ivForceCMD_, ivForceArrSize);
		}
		std::cout<<"Keithley sourced to 0V"<<std::endl;
		return 0;
	}

	int ktCmd::updateCMD(double val){
		
		//Insert number into cmd array
		std::string asStr = std::to_string(static_cast<long double>(val));
		int numLen = asStr.length();
		//std::cout<<numLen<<std::endl;
		for (int i=0; i<ivForceVSize; i++){
			if (i > (numLen-1))
			{
				ivForceCMD_[ivForceVInd-i] =' ';
			} else {
				ivForceCMD_[ivForceVInd-i] = asStr[(numLen-1)-i];
			}
		}
		//std::cout<<&vForceCMD_<<std::endl;
		return 0;
	}

	int ktCmd::ivForce(int SMU, double vF){
		char j = '0' + SMU;
		ivForceCMD_[ivForceSMUInd] = j;
		updateCMD(vF);
		ibwrt(Dev_, ivForceCMD_, ivForceArrSize);
		//std::cout<<vForceCMD_<<std::endl;
		//GPIBCleanup(Dev, "Unable to write to multimeter");
		if (Ibsta() & ERR)
		{
			GPIBCleanup("Unable to write to multimeter");
			return 1;
		}
		return 0;
	}
	
	int ktCmd::ivMeas(int SMU, double & measVal){
		char j = '0' + SMU;
		ivTrigCMD_[2] = j;
		
		ibwrt(Dev_, ivTrigCMD_, ivTrigArrSize);
		ibrd(Dev_, ValueStr, ARRAYSIZE);
		if (Ibsta() & ERR)
		{
			GPIBCleanup("Unable to read data from multimeter");
			return 1;
		}
		sscanf(ValueStr, "%*[^-0123456789]%lf", &measVal);
		//sscanf(ValueStr, "%*s%f", &measVal);
		return 0;
	}


	int ktCmd::sendPersCmd(char cmd[], int len){
		
		ibwrt(Dev_, cmd, len);
		if (Ibsta() & ERR)
		{
			GPIBCleanup("Command issue");
			ktStatus = 1;
			return 1;
		}
		return 0;
	}
	ktCmd::~ktCmd() {
		   
    // The device is taken offline.
    
		ibonl(Dev_, 0);
		std::cout<<"Keithley has been shut down"<<std::endl;
	}
}
