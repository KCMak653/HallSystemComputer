//Keithley Command Class

//This class creates the Keithley object and directly 
//communicates to it via writing and reading commands
//by the GPIB

#pragma once

extern const int vForceVInd;
extern const int vForceSMUInd;
extern const int vForceVSize;

namespace KT
{
	class ktCmd
	{
	public:
		//Constructor initializes command arrays and opens handle to
			//machine
		ktCmd();
		
		//Force 0V on all four SMUs
		int srcZeroAll();

		// Set the compliance in the vForce command and range command
		int setComp(const int SMU, const int comp);

		//Set the lowest possible current range
		int setLRange(const int SMU, const int lRange);

		//Set the current range
		int setRange(const int SMU, const int range);

		//Set the integration time:
			// 1: Short 0.1 PLC
			// 2: Normal 1.0 PLC
			// 3: Long 10 PLC
		int setIntTime(const int intTime);

		//Force a voltage
		int vForce(int SMU, double vF);

		//Send any command to the Keithley
		int sendPersCmd(char cmd[], int len);

		//Functions to be defined at a later time
		//int iForce();
		//int vMeas();
		//int tMeas(double &measT);

		//Measure current
		int iMeas(int SMU, double & measVal);

		//Status of communication
		bool ktStatus;

		//Deconstructor closes handle to machine
		~ktCmd();
	private:
		//Character array for force command
		char vForceCMD_[29];

		//Function called to close device handle if there is an error
		void GPIBCleanup(const char * ErrorMsg);

		//Update the force command with force value
		int updateCMD(double val);
		//char iForceCMD_[];
		//char vMeasCMD_[];
		char iTrigCMD_[4];
		char setRangeCMD_[17];
		char setLRangeCMD_[11];
		char setIntTimeCMD_[4];
		int Dev_;
		
		//char iMeasCMD_[];
	};
}
