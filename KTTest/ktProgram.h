//Keithley programs
//sweepVDS_IDS: sweep drain-source, voltage, record I on same SMU

#pragma once


//Sweep parameters
struct sweepVDS_IDSParameters
{
	double startV; //Start of sweep [V]
	double stopV; //End of sweep [V]
	double SR; //Scan rate [V/s]
	int lRange; //Order of mag of lowest range [A]
	int range; //Order of mag of I range [A]
	int comp; //Compliance, max I value [A}
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
	int sweepSMU; //SMU to force and measure (1 -> 4)
	int nCycles; //Number of cycles
	bool fullCycle; //Return to startV
};


namespace KT 
{
	class sweepVDS_IDS
	{
	public:
		//Constructor creates sweep object and applies all machine settings
		sweepVDS_IDS(const sweepVDS_IDSParameters &entries);

		//Runs the program, stores results in passed arrays
		int runProgram(double vFs[], int sizeArray, double iMs[], double tMs[], int dMs[]);

		//Returns the size of array needed to store measurements
		int arraySizeNeeded();

		~sweepVDS_IDS();
	private:
		sweepParameters sweepP_; //Parameters for sweep class
		KT::ktSweep* swp_; //Ptr to sweep object
		int swpSize_; // Size of a single sweep
		int nCycles_; //Number of cycles
		bool fullCycle_; //Return to startV
		int sizeArrayNeeded; //Size of array to hold values
	};
	/*
	class VDS_sweepVGS_IDS
	{
	public:
		//Constructor creates sweep object and applies all machine settings
		VDS_sweepVGS_IDS(const VDS_sweepVGS_IDSParameters &entries);

		//Runs the program, stores results in passed arrays
		int runProgram(double vFs[], int sizeArray, double iMs[], double tMs[], int dMs[]);

		//Returns the size of array needed to store measurements
		int arraySizeNeeded();

		~VDS_sweepVGS_IDS();
	private:
		sweepParameters sweepP_; //Parameters for sweep class
		KT::ktSweep* swp_; //Ptr to sweep object
		int swpSize_; // Size of a single sweep
		int nCycles_; //Number of cycles
		bool fullCycle_; //Return to startV
		int sizeArrayNeeded; //Size of array to hold values

	};
*/
}