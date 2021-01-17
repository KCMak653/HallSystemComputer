//Keithley apply constant bias with pulse

//Class to create constant bias object

#pragma once


//Input parameters
struct pulseParameters
{
	double constV; //Constant bias to apply [V]
	double constV2; //Constant bias to apply [V]
	double pulseV; //Pulse voltage [V]
	double initTime; //Initial settle time [s]
	double stepTime; //Time betwen pulses [s]
	double pulseTime;// Pulse time[s]
	int nPulses; // Number of pulses
	double dt; //Measurement frequency [ms]
	int lRange; //Order of mag of lowest range [A]
	int range; //Order of mag of I range [A]
	int comp; //Compliance, max I value [A}
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
	int constSMU; //SMU to keep constant
	int constSMU2; //Another SMU to keep constant
	int pulseSMU; //SMU to pulse
	int measSMU; //SMU to measure
};

namespace KT
{
	class ktPulse
	{
	public:
		//Constructor to initialzie keithley and set member variables
		ktPulse(const pulseParameters &entries);

		int runTest(double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray, int iStart);
		//int forceConstV(int constSMU2, double vConst);
		//int setV(double v);

		int arraySizeNeeded();

		~ktPulse();

	private:
		double dt_; //Measurement frequency [ms]
		double initTime_; //Total measurement time [s]
		double stepTime_; //Time btwn pulses [s]
		double pulseTime_; //Pulse time[s]
		double constV_; //Constant bias [V]
		double constV2_; //Second constant bias [V]
		double pulseV_; //Pulse voltage [V]
		int nPulses_; //# of pulses
		int lRange_; //Order of mag of lowest range [A]
		int range_; //Order of mag of I range [A]
		int comp_; //Compliance, max I value [A}
		int intTime_; //Integration time (1,2,3)(Fast, Normal, Long)
		KT::ktCmd* keith_; //Ptr to Keithley command
		int dtMin_; //Minimum time of each measurement [ms]
		int sizeArrayNeeded_; //Size of array to store measurements
		int constSMU_; //SMU to keep const
		int constSMU2_; //Second SMU to keep const
		int pulseSMU_; //SMU to pulse
		int measSMU_; //SMU to measure
	};
}




