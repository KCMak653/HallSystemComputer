//Keithley base sweep class

//Class to create voltage sweep object

#pragma once


//Input parameters
struct sweepParameters
{
	double startV; //Start of sweep [V]
	double stopV; //End of sweep [V]
	double SR; //Scan rate [V/s]
	double appV[4]; //Constant bias to apply
	int lRange; //Order of mag of lowest range [A]
	int range[4]; //Order of mag of I range [A]
	int comp[4]; //Compliance, max I value [A}
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
	//int nChannels; //Number of measurement channels
	//int nSwChannels; //Number of sweep channels
	char forceMode[4]; //SMU to sweep (1 -> 4)
	char measMode[4]; //SMU to keep constant
	bool sweepSMU[4]; //SMU to measure
};

namespace KT
{
	class ktSweep
	{
	public:
		//Constructor initializes the Keithley and applies
		//the machine settings
		ktSweep(const sweepParameters &entries);

		int initializeChannels(); //Initialize channel ranges, comp and apply const bias
		
		int setForceParams(int SMU);

		//Run the sweep
		int runSweep(double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray, int iStart);

		//Returns the size of the array needed based on startV, stopV, SR and int time
		int arraySizeNeeded();

		//Reverse the stopV and stopV - Change direction of sweep
		int reverseV();
		int setSR(double SR);
		//Apply a constant bias
		//int constBias(double constV);

		~ktSweep();

	private:
		double startV_; //Start of sweep [V]
		double stopV_; //End of sweep [V]
		double SR_; //Scan rate [V/s]
		double appV_[4]; //Constant bias [V]
		int lRange_; //Order of mag of lowest range [A]
		int range_[4]; //Order of mag of I range [A]
		int comp_[4]; //Compliance, max I value [A}
		int intTime_; //Integration time (1,2,3)(Fast, Normal, Long)
		int nMeasChannels_; //# of measurement channels
		KT::ktCmd* keith_; //Ptr to Keithley command
		int dtMeas_; //Time of each measurement [ms]
		double stepV_; //Voltage step between measurements [V]
		int sizeArrayNeeded_; //Size of array to store measurements
		bool sweepSMU_[4]; //SMU to sweep (1 -> 4)
		char measMode_[4]; //Measurement mode 'I', 'V'
		char forceMode_[4]; //Force mode 'I', 'V'
		int chSweep_[4]; //Chnls to sweep in order
		int nSwChannels_; //# of sweep channels
		

	};
}