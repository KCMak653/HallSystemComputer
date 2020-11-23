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
#include<time.h>
#include<Windows.h>
#include<time.h>
//struct sweepParameters
//{
	//double startV, stopV, SR;
//};

int __cdecl main(void)
{
   
	//KT::ktCmd test1;
	//KT.initialize();
	//test1.srcZeroAll();
	//test1.vForce(2, .1);
	//double mV =0.98;
	//test1.iMeas(2, mV);
	//std::cout<<mV<<std::endl;
	//test1.srcZeroAll();
	//KT.GPIBCleanup(17, "Hello");
	/*
	clock_t this_time = clock();
	clock_t last_time = this_time;
	Sleep(300);
	last_time = clock();
	double time_count = (double)(last_time);
	time_count = time_count*1.23;
	std::cout<<time_count<<std::endl;
	std::cout<<last_time<<std::endl;
	std::cout<<this_time<<std::endl;
	*/
	/*
	sweepParameters sweepP;
	sweepP.SMU = 2;
	sweepP.startV = 0;
	sweepP.stopV = .002;
	sweepP.SR = 0.001;
	sweepP.lRange = 3;
	sweepP.range =4;
	sweepP.comp = 4;
	sweepP.intTime = 1;
	KT::ktSweep swp(sweepP);
	int arraySize = swp.arraySizeNeeded();
	//std::cout<<j<<std::endl;
	double * vFs = new double[arraySize];
	double * iMs = new double[arraySize];
	double * tMs = new double[arraySize];
	swp.runSweep(vFs, arraySize, iMs, tMs);
	for (int i = 0; i<arraySize; i++){
		std::cout<<vFs[i]<<std::endl;
		std::cout<<iMs[i]<<std::endl;
		std::cout<<tMs[i]<<std::endl;
	}
	delete vFs;
	delete iMs;
	delete tMs;
	*/
	std::cout<<"hey"<<std::endl;
	Sleep(5000);
	std::cout<<"you"<<std::endl;
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



