/*
 * Filename - Simple.c
 *
 * This application demonstrates how to read from and write to the
 * Fluke 45 Dual Display Multimeter using GPIB.
 *
 * This sample application is comprised of three basic parts:
 *
 *  1. Initialization
 *  2. Main Body
 *  3. Cleanup
 *
 * The Initialization portion consists of getting a handle to a
 * device and then clearing the device.
 *
 * In the Main Body, this application queries a device for its
 * identification code by issuing the '*IDN?' command. Many
 * instruments respond to this command with an identification string.
 * Note, 488.2 compliant devices are required to respond to this
 * command.
 *
 * The last step, Cleanup, takes the device offline.
 */
#include <iostream>
#include "ktCmd.h"
#include "ktSweep.h"
#include "ktConst.h"
#include<time.h>
#include<Windows.h>
#include<time.h>
#include "ktProgram.h"
#include <fstream>
#include <string.h>
#include <string>
#include "rcCmd.h"
#include<conio.h>
#include<sstream>
//struct sweepParameters
//{
	//double startV, stopV, SR;
//};

int __cdecl main(void)
{
	
	//RC::rcCmd rc;
	std::ostringstream oss;
	std::string fname;
	char c;

	sweepVDS_IDSParameters sweepP;
	sweepP.sweepSMU = 2;
	sweepP.constSMU = 3;
	sweepP.measSMU = 3; //Change to 3
	sweepP.startV = 0;
	sweepP.stopV = -0.7;
	sweepP.SR = 0.08;
	sweepP.constV = .05;
	sweepP.lRange = 3;
	sweepP.range =3;
	sweepP.comp = 3;
	sweepP.intTime = 1;
	sweepP.nCycles = 6;
	sweepP.fullCycle = TRUE;

	KT::sweepVDS_IDS swp(sweepP);
	
	
	int arraySize = swp.arraySizeNeeded();
	std::cout<<"Array size is: "<<arraySize<<std::endl;
	//rc.chOn(1);
	//rc3.chAllOn();
	double * vFs = new double[arraySize];
	double * iMs = new double[arraySize];
	double * tMs = new double[arraySize];
	int * dMs = new int[arraySize];
	


	
	for (int pH = 1; pH<2; pH++) { //Change to number of actual
		for (int dev = 5; dev<8; dev++){

			std::cout<<"Press s to skip or g to begin test for: " <<std::endl;
			std::cout << "Device: " << dev << " and solution: " << pH << std::endl;
			std::cin >> c;

			if (c == 'g') {
				std::cout<< "Program is running"<<std::endl;
				swp.runProgram(vFs, iMs, tMs, dMs, arraySize);
				oss << "PBS_CV_G6_" << std::to_string((long long) dev)<<"_Day3test";
				fname = oss.str();
				std::cout<<"Filename: " <<fname << std::endl;

				//rc.chOff(1);
				//rc.chOn(2);
				for (int i = 0; i<arraySize; i++){
					std::cout<<"F: :"<<vFs[i]<<std::endl;
					std::cout<<"i: "<<iMs[i]<<std::endl;
					std::cout<<"t: "<< tMs[i]<<std::endl;
					std::cout<<"d: "<<dMs[i]<<std::endl;
				}
				swp.saveData(fname, vFs, iMs, tMs, dMs, arraySize);
				oss.str("");
				oss.clear();
			}
			else std::cout << "Skipped"  <<std::endl;
		}
	}

	//rc.chOff(2);

	
	delete vFs;
	delete iMs;
	delete tMs;
	delete dMs;
   /*
	//KT::ktCmd test1;
	//KT.initialize();
	//test1.srcZeroAll();
	//test1.vForce(2, .1);
	//double mV =0.98;
	//test1.iMeas(2, mV);
	//std::cout<<mV<<std::endl;
	//test1.srcZeroAll();
	//KT.GPIBCleanup(17, "Hello");
	time_t t;
	time_t endt;
	time(&t);
	endt = t +100;
	std::cout<<"Current time: " << ctime(&t) << std::endl;
	std::cout<<"End time: " << ctime(&endt) <<std::endl;
	*/
	//RC::rcCmd rc3;
	/*
	constVDS_IDSParameters constP;
	constP.constV = 0.02;
	constP.measTime = 28800;
	constP.dt = 1000;
	constP.lRange = 6;
	constP.range = 6;
	constP.comp = 4;
	constP.intTime = 1;
	constP.measSMU =2;
	constP.constSMU =2;

	KT::constVDS_IDS swp(constP);

	int arraySize = swp.arraySizeNeeded();
	
	std::cout<<"Array size is: "<<arraySize<<std::endl;
	
	//rc3.chOn(4);
	//rc3.chAllOn();
	double * vFs = new double[arraySize];
	double * iMs = new double[arraySize];
	double * tMs = new double[arraySize];
	int * dMs = new int[arraySize];
	swp.runProgram(vFs, iMs, tMs, dMs, arraySize);
	for (int i = 0; i<arraySize; i++){
		std::cout<<"F: :"<<vFs[i]<<std::endl;
		std::cout<<"i: "<<iMs[i]<<std::endl;
		std::cout<<"t: "<< tMs[i]<<std::endl;
		std::cout<<"d: "<<dMs[i]<<std::endl;
	}

	
	std::string fn = "vdsidsConst";
	//std::string fn2 = "Param_";
	swp.saveData(fn, vFs, iMs, tMs, dMs, arraySize);
	delete vFs;
	delete iMs;
	delete tMs;
	delete dMs;
	*/
	/*
	sweepVDS_IDSParameters sweepP;
	sweepP.sweepSMU = 3;
	sweepP.constSMU = 2;
	sweepP.measSMU = 2;
	sweepP.startV = -2;
	sweepP.stopV = 6;
	sweepP.SR = 0.5;
	sweepP.constV = .05;
	sweepP.lRange = 3;
	sweepP.range =3;
	sweepP.comp = 3;
	sweepP.intTime = 1;
	sweepP.nCycles = 4;
	sweepP.fullCycle = TRUE;

	KT::sweepVDS_IDS swp(sweepP);
	
	
	int arraySize = swp.arraySizeNeeded();
	std::cout<<"Array size is: "<<arraySize<<std::endl;
	//rc3.chOn(4);
	//rc3.chAllOn();
	double * vFs = new double[arraySize];
	double * iMs = new double[arraySize];
	double * tMs = new double[arraySize];
	int * dMs = new int[arraySize];
	swp.runProgram(vFs, iMs, tMs, dMs, arraySize);
	for (int i = 0; i<arraySize; i++){
		std::cout<<"F: :"<<vFs[i]<<std::endl;
		std::cout<<"i: "<<iMs[i]<<std::endl;
		std::cout<<"t: "<< tMs[i]<<std::endl;
		std::cout<<"d: "<<dMs[i]<<std::endl;
	}

	
	std::string fn = "vgsidstest";
	//std::string fn2 = "Param_";
	swp.saveData(fn, vFs, iMs, tMs, dMs, arraySize);
	delete vFs;
	delete iMs;
	delete tMs;
	delete dMs;
	//rc3.chAllOff();


	/*
	int slptime = 15;
	double slptime2 = 25;
	clock_t this_time = clock();
	clock_t last_time = clock();
	clock_t set_time = clock();
	
	Sleep(slptime);
	this_time=clock();
	
	Sleep(slptime2);
	last_time = clock();
	double time_count = (double)(last_time - this_time);
	std::cout<<time_count<<std::endl;
	std::cout<<last_time<<std::endl;
	std::cout<<this_time<<std::endl;
	std::cout<<set_time<<std::endl;
	*/
	/*
	stepVDS_IDSParameters sweepP;
	sweepP.startV = -0.6;
	sweepP.stopV = 0;
	sweepP.stepV = 0.2;
	sweepP.dt = 60;
	sweepP.stepTime = 3600;
	sweepP.stepSMU = 2;
	sweepP.constSMU = 3;
	sweepP.measSMU = 3;
	sweepP.constV = 0.05;
	sweepP.lRange = 3;
	sweepP.range =3;
	sweepP.comp = 3;
	sweepP.intTime = 1;
	sweepP.nCycles = 1;
	sweepP.fullCycle = 0;


	KT::stepVDS_IDS swp(sweepP);
	
	
	int arraySize = swp.arraySizeNeeded();
	std::cout<<"Array size is: "<<arraySize<<std::endl;
	
	double * vFs = new double[arraySize];
	double * iMs = new double[arraySize];
	double * tMs = new double[arraySize];
	int * dMs = new int[arraySize];
	//swp.runFlight(vFs, iMs, tMs, dMs, 0);
	swp.runProgram(vFs, iMs, tMs, dMs, arraySize);
	/*
	for (int i = 0; i<arraySize; i++){
		std::cout<<"F: :"<<vFs[i]<<std::endl;
		std::cout<<"i: "<<iMs[i]<<std::endl;
		std::cout<<"t: "<< tMs[i]<<std::endl;
		std::cout<<"d: "<<dMs[i]<<std::endl;
	}
	*/
	//std::cout<<arraySize<<std::endl;
	/*
	std::string fn = "LStV_G6_5_PBS_long_test";
	//std::string fn2 = "Param_";
	
	swp.saveData(fn, vFs, iMs, tMs, dMs, arraySize);
	
	delete vFs;
	delete iMs;
	delete tMs;
	delete dMs;
	
	
	//RC::rcCmd rc3;
	//rc3.chOn(3);
	//rc3.chOn(7);
	/*rc3.chOn(4);
	rc3.chOn(6);
	rc3.chOff(2);
	rc3.chAllOff();
	*/
	/*
	char sentence[] = "NBI-0.456E6";
	double val;
	//sscanf(sentence, "%*[^0-9]%d", &val);
	sscanf(sentence, "%*[^-0123456789]%lf", &val);
	std::cout<<val<<std::endl;
	*/
	/*
	sweepVDS_IDSParameters sweepP;
	sweepP.sweepSMU = 2;
	sweepP.constSMU = 1;
	sweepP.measSMU = 2;
	sweepP.startV = -.7;
	sweepP.stopV = .1;
	sweepP.SR = 0.1;
	sweepP.constV = 0.0;
	sweepP.lRange = 3;
	sweepP.range =3;
	sweepP.comp = 3;
	sweepP.intTime = 1;
	sweepP.nCycles = 10;
	sweepP.fullCycle = TRUE;

	KT::sweepVDS_IDS swp(sweepP);
	
	
	int arraySize = swp.arraySizeNeeded();
	std::cout<<"Array size is: "<<arraySize<<std::endl;
	rc3.chOn(5);
	//rc3.chAllOn();
	double * vFs = new double[arraySize];
	double * iMs = new double[arraySize];
	double * tMs = new double[arraySize];
	int * dMs = new int[arraySize];
	swp.runProgram(vFs, iMs, tMs, dMs, arraySize);
	for (int i = 0; i<arraySize; i++){
		std::cout<<"F: :"<<vFs[i]<<std::endl;
		std::cout<<"i: "<<iMs[i]<<std::endl;
		std::cout<<"t: "<< tMs[i]<<std::endl;
		std::cout<<"d: "<<dMs[i]<<std::endl;
	}

	
	std::string fn = "CVF5GndPos";
	//std::string fn2 = "Param_";
	swp.saveData(fn, vFs, iMs, tMs, dMs, arraySize);
	delete vFs;
	delete iMs;
	delete tMs;
	delete dMs;
	rc3.chAllOff();
	//rc3.chOff(3);
	//std::cout<<fn+fn2;
	/*
	std::cout<<"hey"<<std::endl;
	Sleep(5000);
	std::cout<<"you"<<std::endl;
	 */
	/*

	//KT::ktCmd chk;
	//char cmd[] = "UL\0";
	//int len = strlen(cmd);
	//chk.sendPersCmd(cmd, len);
	//char cmd3[] = "GD NWFET_KM SourceZeroAll";
	//chk.sendPersCmd(cmd3);

   /*
    * ========================================================================
    *
    * MAIN BODY SECTION
    *
    * In this application, the Main Body communicates with the instrument
    * by writing a command to it and reading its response. This would be
    * the right place to put other instrument communication.
    *
    * ========================================================================
    */

   /*
    * The application writes the '*IDN?' command to the multimeter.
    
   ibwrt(Dev, "US", 2);
   


   ibwrt(Dev, cmd, length);
   ibwrt(Dev, "TI3", 3);
   
   /*
   
   ibwrt(Dev, "DV3, 0, 2.000000E-1, 1.000000E-2\n", 6L);
   if (Ibsta() & ERR)
   {
      GPIBCleanup(Dev, "Unable to write to multimeter");
      return 1;
   }
   */
   /*
    * The application reads the ASCII string from the multimeter into
    * the variable ValueStr.
    
   
   ibrd(Dev, ValueStr, ARRAYSIZE);
   if (Ibsta() & ERR)
   {
      GPIBCleanup(Dev, "Unable to read data from multimeter");
      return 1;
   }
   
   /*
    * Assume that the returned string contains ASCII data. NULL
    * terminate the string using the value in ibcntl which is the
    * number of bytes read in. Use printf to display the string.
    
   ValueStr[Ibcnt() - 1] = '\0';

   printf("Data read: %s\n", ValueStr);

   /*
    * ========================================================================
    *
    * CLEANUP SECTION
    *
    * ========================================================================
    */

   /*
    * The device is taken offline.
    
   ibonl(Dev, 0);
   */
   return 0;
}



