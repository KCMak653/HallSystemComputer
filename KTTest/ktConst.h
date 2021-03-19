//Keithley apply constant bias class, base for vStep class

//Class to create constant bias object

#pragma once


//Input parameterss
struct constParameters
{
	double appV[4]; //Constant bias to apply [V]
	double measTime; //Total measurement time [s]
	double dt; //Measurement frequency [ms]
	int lRange; //Order of mag of lowest range [A]
	int range; //Order of mag of I range [A]
	int comp; //Compliance, max I value [A}
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
	int measSMU; //SMU to measure
	char forceMode; //Mode to force: 'V', 'I'
	char measMode; //Mode to meas: 'V', 'I'
};

namespace KT
{
	class ktConst
	{
	public:
		//Constructor to initialzie keithley and set member variables
		ktConst(const constParameters &entries);

		int runTest(double iMs[], double tMs[], int dMs[], int sizeArray, int iStart);
		//int forceConstV(int constSMU2, double vConst);
		int setIV(int SMU, double v);

		int setMeasTime(double t);

		int arraySizeNeeded();

		~ktConst();

	private:
		double dt_; //Measurement frequency [ms]
		double measTime_; //Total measurement time [s]
		double appV_ [4]; //Array of biases to apply
		int lRange_; //Order of mag of lowest range [A]
		int range_; //Order of mag of I range [A]
		int comp_; //Compliance, max I value [A}
		int intTime_; //Integration time (1,2,3)(Fast, Normal, Long)
		char measMode_; //Measurement mode 'V', 'I'
		char forceMode_; //Force mode 'V', 'I'
		KT::ktCmd* keith_; //Ptr to Keithley command
		int dtMin_; //Minimum time of each measurement [ms]
		int sizeArrayNeeded_; //Size of array to store measurements
		//int constSMU_; //SMU to keep const
		//int 
		int measSMU_; //SMU to measure
	};
}




