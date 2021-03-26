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
#include "mcCmd.h"
#include "mcConst.h"
#include<conio.h>
#include<sstream>
#include <thread>
#include<vector>

void constTest();
void sweepTest();
void stepTest();
void pulseTest();
void threadTest();
void rcTest();
void multiDevTC();
void mcTest();


int __cdecl main(void)
{
	sweepTest();
    return 0;
}
void threadTest(){
	/*
	constVDS_IDSParameters constP;

	
	constP.measTime = 10;
	constP.dt = 1000;
	constP.lRange = 6;
	constP.range = 6;
	constP.comp = 4;
	constP.intTime = 1;
	constP.measSMU = 3;
	constP.appV[0] = 0;
	constP.appV[1] = 0;
	constP.appV[2] = .2;
	constP.appV[3] = 0;
	//constP.constSMU =2;

	KT::constVDS_IDS swp(constP);

	int arraySize = swp.arraySizeNeeded();

	std::cout << "Array size is: " << arraySize << std::endl;


	//rc3.chAllOn();
	//double * vFs = new double[arraySize];
	double* iMs = new double[arraySize];
	double* tMs = new double[arraySize];
	int* dMs = new int[arraySize];

	//std::thread t1(&MC::mcConst::move, &md, 6.8, 5);
	std::thread t2(&KT::constVDS_IDS::runProgram, &swp, iMs, tMs, dMs, arraySize);

	//t1.join();
	t2.join();


	//swp.runProgram();
	for (int i = 0; i < arraySize; i++) {
		//std::cout<<"F: :"<<vFs[i]<<std::endl;
		std::cout << "i: " << iMs[i] << std::endl;
		std::cout << "t: " << tMs[i] << std::endl;
		std::cout << "d: " << dMs[i] << std::endl;
	}


	std::string fn = "vdsidsConst_test2";
	//std::string fn2 = "Param_";
	swp.saveData(fn, iMs, tMs, dMs, arraySize);
	//delete vFs;
	delete iMs;
	delete tMs;
	delete dMs;
	*/
}
void mcTest(){
	/*
		//std::cin>>c;
	
	char c = 'm';
	MC::mcCmd md;
	//md.setRefPt();
	
	int conv = 370;
	double numML = 1;
	double mLposO = 5;
	double mLpos = mLposO;
	int lLim = - (mLpos - 5)*conv;
	int rLim = (15 - mLpos)*conv;
	std::cout<<"lLim: " << lLim <<std::endl;
	std::cout<<"rLim: " <<rLim <<std::endl;
	//md.setNSteps(200);
	int pos = md.getPosition();
	std::cout<<pos<<std::endl;//md.move();
	md.setNSteps(200);
	while (c != 's')
	{
		if (c=='r'){
			md.reverseDirection();
		}
		else if (c=='o')
		{
			md.moveOrigin();
		}
		else if(c=='n')
		{
			std::cin>>numML;
			int nSteps = numML*conv;
			md.setNSteps(nSteps);
		}
		else
		{
			md.move();
		}
		pos = md.getPosition();
		mLpos = mLposO + pos / conv; 
		std::cout<<"Current position: " << pos<<std::endl;
		std::cout<<"Current mL position: " << mLpos<<std::endl;
		std::cin >> c;
	}
	//md.move();

	//md.moveRefPt(100);
	*/
}
void multiDevTC(){
	/*
	std::ostringstream oss;
	std::string fname;
	char c;
	double SRs[] = {0.017, 0.034, 0.173, 0.349};
	int devs[] = {5,7};

	sweepVDS_IDSParameters sweepP;
	sweepP.sweepSMU = 2;
	sweepP.constSMU = 3;
	sweepP.measSMU = 3; //Change to 3
	sweepP.startV = 0;
	sweepP.stopV = -0.7;
	sweepP.SR = 0.017;
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
	
	//rc.chOn(4);
	//rc3.chAllOn();
	double * vFs = new double[arraySize];
	double * iMs = new double[arraySize];
	double * tMs = new double[arraySize];
	int * dMs = new int[arraySize];
	


	
	for (int pH = 0; pH<4; pH++) { //Change to number of actual
		for (int dev = 0; dev<2; dev++){

			//std::cout<<"Press s to skip or g to begin test for: " <<std::endl;
			std::cout << "Device: " << devs[dev] << " and sr: " << (int) (SRs[pH]*1e3) << std::endl;
			//std::cin >> c;

			//if (c == 'g') {
			std::cout<< "Program is running"<<std::endl;
			swp.runProgram(vFs, iMs, tMs, dMs, arraySize);
			oss << "PBS_CV_G6_" << std::to_string((long long) devs[dev])<<"_SR_" 
				<< std::to_string((long long) ((int)(SRs[pH]*1e3))) << "_DayX_newSU";
			fname = oss.str();
			std::cout<<"Filename: " <<fname << std::endl;

			rc.chOff(1);
			rc.chOn(4);
			
				for (int i = 0; i<arraySize; i++){
					std::cout<<"F: :"<<vFs[i]<<std::endl;
					std::cout<<"i: "<<iMs[i]<<std::endl;
					std::cout<<"t: "<< tMs[i]<<std::endl;
					std::cout<<"d: "<<dMs[i]<<std::endl;
				}
				
			swp.saveData(fname, vFs, iMs, tMs, dMs, arraySize);
			oss.str("");
			oss.clear();
			//}
			//else std::cout << "Skipped"  <<std::endl;
		}
		rc.chOff(4);
		rc.chOn(1);
		swp.setSR(SRs[pH]);
	}

	rc.chOff(4);
	rc.chOff(1);
	//std::cin >> c;
	
	
	delete vFs;
	delete iMs;
	delete tMs;
	delete dMs;
*/
}

void pulseTest(){
	
	pulseProgramParameters pulseP;

	pulseP.pulseV = 5;
	pulseP.pulseOffV = 1;
	pulseP.initTime = 3;
	pulseP.stepTime = 1;
	pulseP.pulseTime = 1;
	pulseP.nPulses = 3;
	pulseP.dt = 200;

	pulseP.lRange = 3;

	pulseP.intTime = 1;

	bool pulseSMU[4] = { FALSE, TRUE, FALSE, FALSE };
	int range[] = { 0,0, 0, 0 };
	int comp[] = { 1, 1,1,1 };
	char measMode[] = { 'N', 'I','I', 'N' };
	char forceMode[] = { 'N', 'V','V', 'N' };
	double appV[] = { 0, 1, 5, 0 };
	for (int i = 0; i < 4; i++) {
		pulseP.range[i] = range[i];
		pulseP.comp[i] = comp[i];
		pulseP.forceMode[i] = forceMode[i];
		pulseP.measMode[i] = measMode[i];
		pulseP.appV[i] = appV[i];
		pulseP.pulseSMU[i] = pulseSMU[i];
	}

	KT::pulseProgram pulse(pulseP);

	int arraySize = pulse.arraySizeNeeded();
	std::cout<<"Array size is: "<<arraySize<<std::endl;
	
	int nC = 2;
	
	double * vFs = new double[arraySize];
	double * iMs = new double[arraySize * nC];
	double * tMs = new double[arraySize];
	int * dMs = new int[arraySize];
	//swp.runFlight(vFs, iMs, tMs, dMs, 0);
	pulse.runProgram(vFs, iMs, tMs, dMs, arraySize);
	
	for (int i = 0; i<arraySize; i++){
		std::cout<<"F: :"<<vFs[i]<<std::endl;
		std::cout<<"i: "<<iMs[i]<<std::endl;
		std::cout<<"t: "<< tMs[i]<<std::endl;
		std::cout<<"d: "<<dMs[i]<<std::endl;
	}
	
	//std::cout<<arraySize<<std::endl;
	
	std::string fn = "pulse_test";
	//std::string fn2 = "Param_";
	
	pulse.saveData(fn, vFs, iMs, tMs, dMs, arraySize);

	
	delete vFs;
	delete iMs;
	delete tMs;
	delete dMs;
	
}
void constTest(){
	
	constProgramParameters constP;
	
	
	constP.measTime = 1800;
	constP.dt = 1000;
	constP.lRange = 6;

	constP.intTime = 1;

	int range[] = { 0,0, 0, 0 };
	int comp[] = { 1, 1,1,1 };
	char measMode[] = { 'N', 'N','V', 'N' };
	char forceMode[] = { 'N', 'N','I', 'N' };
	double appV[] = { 0, 0, 0, 0 };
	for (int i = 0; i < 4; i++) {
		constP.range[i] = range[i];
		constP.comp[i] = comp[i];
		constP.forceMode[i] = forceMode[i];
		constP.measMode[i] = measMode[i];
		constP.appV[i] = appV[i];
	}
	//constP.constSMU =2;
	
	KT::constProgram swp(constP);

	int arraySize = swp.arraySizeNeeded();
	std::cout << arraySize << std::endl;
	
	std::cout<<"Array size is: "<<arraySize<<std::endl;
	
	int nC = 1;
	//rc3.chAllOn();
	//double * vFs = new double[arraySize];
	double * iMs = new double[arraySize*nC];
	double * tMs = new double[arraySize];
	int * dMs = new int[arraySize];
	swp.runProgram(iMs, tMs, dMs, arraySize);
	for (int i = 0; i<arraySize; i++){
		//std::cout<<"F: :"<<vFs[i]<<std::endl;
		std::cout<<"i: "<<iMs[i]<<std::endl;
		std::cout<<"t: "<< tMs[i]<<std::endl;
		std::cout<<"d: "<<dMs[i]<<std::endl;
	}

	
	std::string fn = "test";
	//std::string fn2 = "Param_";
	swp.saveData(fn, iMs, tMs, dMs, arraySize);
	//delete vFs;
	delete iMs;
	delete tMs;
	delete dMs;
	
}

void rcTest(){
	/*
	RC::rcCmd rc3;
	rc3.chOn(3);
	rc3.chOn(7);
	rc3.chOn(4);
	rc3.chOn(6);
	rc3.chOff(2);
	rc3.chAllOff();
	*/
}
void stepTest(){
	
	stepProgramParameters sweepP;
	sweepP.startV = 4;
	sweepP.stopV = 6;
	sweepP.stepV = 1;
	sweepP.dt = 200;
	sweepP.stepTime = 2;

	sweepP.lRange = 3;

	sweepP.intTime = 1;
	sweepP.nCycles = 2;
	sweepP.fullCycle = 1;


	bool stepSMU[4] = { FALSE, TRUE, TRUE, FALSE };
	int range[] = { 0,0, 0, 0 };
	int comp[] = { 1, 1,1,1 };
	char measMode[] = { 'N', 'I','I', 'N' };
	char forceMode[] = { 'N', 'V','V', 'N' };
	double appV[] = { 0, 1, 0, 0 };
	for (int i = 0; i < 4; i++) {
		sweepP.range[i] = range[i];
		sweepP.comp[i] = comp[i];
		sweepP.forceMode[i] = forceMode[i];
		sweepP.measMode[i] = measMode[i];
		sweepP.appV[i] = appV[i];
		sweepP.stepSMU[i] = stepSMU[i];
	}



	KT::stepProgram swp(sweepP);
	
	
	int arraySize = swp.arraySizeNeeded();
	std::cout<<"Array size is: "<<arraySize<<std::endl;
	int nC = 2;
	double * vFs = new double[arraySize];
	double * iMs = new double[arraySize*nC];
	double * tMs = new double[arraySize];
	int * dMs = new int[arraySize];
	//swp.runFlight(vFs, iMs, tMs, dMs, 0);
	swp.runProgram(vFs, iMs, tMs, dMs, arraySize);
	
	for (int i = 0; i<arraySize; i++){
		std::cout<<"F: :"<<vFs[i]<<std::endl;
		std::cout<<"i: "<<iMs[i]<<std::endl;
		std::cout<<"t: "<< tMs[i]<<std::endl;
		std::cout<<"d: "<<dMs[i]<<std::endl;
	}
	
	//std::cout<<arraySize<<std::endl;
	
	std::string fn = "step_test";
	//std::string fn2 = "Param_";
	
	swp.saveData(fn, vFs, iMs, tMs, dMs, arraySize);
	
	delete vFs;
	delete iMs;
	delete tMs;
	delete dMs;
	
}
void sweepTest(){

	sweepProgramParameters sweepP;

	sweepP.startV = 2;
	sweepP.stopV = 3;
	sweepP.SR = 0.5;

	sweepP.lRange = 3;

	sweepP.intTime = 1;
	sweepP.nCycles = 2;
	sweepP.fullCycle = TRUE;

	bool sweepSMU[4] = { FALSE, TRUE, FALSE, FALSE };
	int range[] = { 0,0, 0, 0 };
	int comp[] = { 1, 1,1,1 };
	char measMode[] = { 'N', 'I','I', 'N' };
	char forceMode[] = { 'N', 'V','V', 'N' };
	double appV[] = { 0, 0, 4, 0 };
	int nC = 0;
	for (int i = 0; i < 4; i++) {
		sweepP.range[i] = range[i];
		sweepP.comp[i] = comp[i];
		sweepP.forceMode[i] = forceMode[i];
		sweepP.measMode[i] = measMode[i];
		sweepP.appV[i] = appV[i];
		sweepP.sweepSMU[i] = sweepSMU[i];
		if (measMode[i] != 'N') nC++;

	}
	
	KT::sweepProgram swp(sweepP);
	

	int arraySize = swp.arraySizeNeeded();
	std::cout<<"Array size is: "<<arraySize<<std::endl;
	//rc3.chOn(5);
	//rc3.chAllOn();
	double * vFs = new double[arraySize];
	double * iMs = new double[arraySize*nC];
	double * tMs = new double[arraySize];
	int * dMs = new int[arraySize];
	swp.runProgram(vFs, iMs, tMs, dMs, arraySize);
	for (int i = 0; i<arraySize; i++){
		std::cout<<"F: :"<<vFs[i]<<std::endl;
		std::cout<<"i: "<<iMs[i]<<std::endl;
		std::cout<<"t: "<< tMs[i]<<std::endl;
		std::cout<<"d: "<<dMs[i]<<std::endl;
	}

	
	std::string fn = "sweep_test";
	//std::string fn2 = "Param_";
	swp.saveData(fn, vFs, iMs, tMs, dMs, arraySize);
	delete vFs;
	delete iMs;
	delete tMs;
	delete dMs;
	
	
}
