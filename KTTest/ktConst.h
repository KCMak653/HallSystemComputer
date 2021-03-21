//Keithley apply constant bias class, base for vStep class

//Class to create constant bias object

#pragma once


//Input parameterss
struct constParameters
{
	double appV[4]; //Constant bias to apply [V] or [A]
	double measTime; //Total measurement time [s]
	double dt; //Measurement frequency [ms]
	int lRange; //Order of mag of current lowest range [A]
	int range[4]; //I range (1-1nA, inc by 1e1) or V range (1 - 20V, 2 - 200V, 3 - 200V, 4-200mV, 5 - 2V)
	int comp[4]; //Compliance, max I or V value [A or V], order of mag
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
	//int nChannels; //Number of measurement channels
	char forceMode[4]; //Mode to force: 'V', 'I', 'N' no force
	char measMode[4]; //Mode to meas: 'V', 'I', 'N' - no measure
};

namespace KT
{
	class ktConst
	{
	public:
		//Constructor to initialzie keithley and set member variables
		ktConst(const constParameters &entries);

		int initializeChannels(); //Initialize channel ranges, comp and apply const bias

		//Set up parameters for force, command used when setting up force command for changing values
		int setForceParams(int SMU);

		int runTest(double iMs[], double tMs[], int dMs[], int sizeArray, int iStart);
		//int forceConstV(int constSMU2, double vConst);

		int setIV(bool SMU[], double v); //Change force V or I and apply to select SMU - only one value for changing

		int setMeasTime(double t);

		int arraySizeNeeded();

		~ktConst();

	private:
		double dt_; //Measurement frequency [ms]
		double measTime_; //Total measurement time [s]
		double appV_ [4]; //Array of biases to apply
		int lRange_; //Order of mag of lowest range [A]
		int range_[4]; //Order of mag of I range [A]
		int comp_[4]; //Compliance, max I value [A}
		int intTime_; //Integration time (1,2,3)(Fast, Normal, Long)
		char measMode_[4]; //Measurement mode 'V', 'I'
		int chMeas_[4]; //Keep track of channels to measure in order
		char forceMode_[4]; //Force mode 'V', 'I'
		int nMeasChannels_; //Number of channels that will be measured on
		KT::ktCmd* keith_; //Ptr to Keithley command
		int dtMin_; //Minimum time of each measurement [ms]
		int sizeArrayNeeded_; //Size of array to store measurements
		//int constSMU_; //SMU to keep const
		//int 
		//int measSMU_[4]; //SMU to measure
	};
}




