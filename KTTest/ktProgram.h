//Keithley programs
//sweepVDS_IDS: Sweep V, measure I - Any 3 electrode configuration possible
	//i.e Apply const Bias to SMU, sweep other, measure another

#pragma once
#include<string.h>
#include<string>

//Sweep parameters
struct sweepProgramParameters
{
	double startV; //Start of sweep [V]
	double stopV; //End of sweep [V]
	double SR; //Scan rate [V/s]
	double appV[];
	int lRange; //Order of mag of lowest range [A]
	int range[4]; //Order of mag of I range [A]
	int comp[4]; //Compliance, max I value [A}
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
	bool sweepSMU[4]; //SMUs to sweep
	char measMode[4]; //Measurement mode
	char forceMode[4]; //Applied constant bias 
	int nCycles; //Number of cycles
	bool fullCycle; //Return to startV
};

struct constProgramParameters
{
	double appV[4]; //Constant bias to apply [V]
	double measTime; //Total measurement time [s]
	double dt; //Measurement frequency [ms]
	int lRange; //Order of mag of lowest range [A]
	int range[4]; //Order of mag of I range [A]
	int comp[4]; //Compliance, max I value [A}
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
	//int nChannels;
	//int constSMU; //SMU to keep constant
	char forceMode[4]; //MOde to force: 'V', 'I', 'N'
	char measMode[4]; //Mode to meas: 'V', 'I', 'N'
};

struct stepProgramParameters
{
	double startV; //Start step voltage [V]
	double stopV; //Max voltage to apply [V]
	double stepV; //Size of voltage step [V]
	double appV[4]; //Constant bias to apply [V]
	double stepTime; //Step measurement time [s]
	double dt; //Measurement frequency [ms]
	int lRange; //Order of mag of lowest range [A]
	int range[4]; //Order of mag of I range [A]
	int comp[4]; //Compliance, max I value [A}
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
	//int nChannels; //Number of measurement channels
	int nCycles; //Number of cycles
	bool fullCycle;
	//int constSMU; //SMU to keep constant
	char forceMode[4]; //Mode to force
	char measMode[4]; //Mode to meas
	bool stepSMU; //Bool of SMUs to step
};

struct pulseProgramParameters
{
	double appV[4]; //Constant bias to apply [V]
	double pulseV; //Pulse voltage [V]
	double pulseOffV; //Off voltage
	double initTime; //Initial settle time [s]
	double stepTime; //Time betwen pulses [s]
	double pulseTime;// Pulse time[s]
	int nPulses; // Number of pulses
	double dt; //Measurement frequency [ms]
	int lRange; //Order of mag of lowest range [A]
	int range[4]; //Order of mag of I range [A]
	int comp[4]; //Compliance, max I value [A}
	int intTime; //Integration time (1,2,3)(Fast, Normal, Long)
	//int nChannels; //Number of measurement channels
	bool pulseSMU[4]; //bool of SMUs to pulse
	char measMode[4]; //Mode to meas
	char forceMode[4]; //Mode tor force
};

namespace KT 
{
	class sweepProgram
	{
	public:
		//Constructor creates sweep object and applies all machine settings
		sweepProgram(const sweepProgramParameters &entries);

		//Runs the program, stores results in passed arrays
		int runProgram(double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray);

		//Returns the size of array needed to store measurements
		int arraySizeNeeded();
		int setSR(double SR);
		//Save the data
		int saveData(std::string fn, double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray);

		~sweepProgram();
	private:
		sweepParameters sweepP_; //Parameters for sweep class
		KT::ktSweep* swp_; //Ptr to sweep object
		int swpSize_; // Size of a single sweep
		int nCycles_; //Number of cycles
		bool fullCycle_; //Return to startV
		int sizeArrayNeeded; //Size of array to hold values
		int runTime_; //Total runtime of program in seconds
		int nMeasChannels_; //# of meas channels
		int chMeas_[4]; //Measruement channels in order
		bool sweepSMU_[4]; //Channels to sweep
	};
	
	class constProgram
	{
	public:
		//Constructor creates sweep object and applies all machine settings
		constProgram(const constProgramParameters &entries);

		//Runs the program, stores results in passed arrays
		int runProgram(double iMs[], double tMs[], int dMs[],int sizeArray);

		//Returns the size of array needed to store measurements
		int arraySizeNeeded();


		//Save the data
		int saveData(std::string fn,  double iMs[], double tMs[], int dMs[],int sizeArray);

		~constProgram();
	private:
		constParameters constP_; //Parameters for const class
		KT::ktConst* cnst_; //Ptr to sweep object
		int cnstSize_; // Size of a single sweep
		int sizeArrayNeeded; //Size of array to hold values
		int runTime_; //Total runtime of program in seconds
		int nMeasChannels_; //Number of meas channels
		int chMeas_[4]; //Meas channels in order

	};

	class stepProgram
	{
	public:
		stepProgram(const stepProgramParameters &entries);
		int runProgram(double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray);
		int arraySizeNeeded();
		//int saveData(std::string fn, double vFs[], int sizeArray, double iMs[], double tMs[]);
		int runFlight(double iMs[], double tMs[], int dMs[], int iStart, int nS, double v);
		int saveData(std::string fn, double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray);
		~stepProgram();
	private:
		constParameters stepP_;
		KT::ktConst* step_;
		int stepSize_;
		int nCycles_;
		bool fullCycle_;
		int nSteps_;
		int nStepsTot_;
		double stopV_;
		double startV_;
		double stepV_;
		bool stepSMU_[4];
		int nMeasChannels_;
		int chMeas_[4];
		// int chStep_[4];
		// int nChStep_;
		//double appV_[4];
		//double constSMU_;
		int sizeArrayNeeded_;
		void reverseV();
		int runTime_; //Total runtime of program in seconds
		

	};

	class pulseProgram
	{
	public:
		pulseProgram(const pulseProgramParameters &entries);
		int runProgram(double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray);
		int arraySizeNeeded();
		//int saveData(std::string fn, double vFs[], int sizeArray, double iMs[], double tMs[]);
		
		int runPulse(double iMs[], double tMs[], int dMs[], int iStart, int nS, double v);
		int saveData(std::string fn, double vFs[], double iMs[], double tMs[], int dMs[], int sizeArray);
		~pulseProgram();
	private:
		constParameters pulseP_;
		KT::ktConst* pulse_;
		int initSize_;
		int pulseSize_;
		int stepSize_;
		int nPulses_;
		double pulseV_;
		double pulseOffV_;
		double initTime_;
		double pulseTime_;
		double stepTime_;
		bool pulseSMU_[4];
		int nMeasChannels_;
		int chMeas_[4];
		//double constSMU_;
		int sizeArrayNeeded_;
		//void reverseV();
		int runTime_; //Total runtime of program in seconds
		

	};

}