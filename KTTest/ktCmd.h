//Keithley command class

#pragma once

extern const int vForceVInd;
extern const int vForceSMUInd;
extern const int vForceVSize;

namespace KT
{
	class ktCmd
	{
	public:
		ktCmd();
		//int initialize();
		int srcZeroAll();

		int setComp(const int SMU, const int comp);
		int setLRange(const int SMU, const int lRange);
		int setRange(const int SMU, const int range);
		int setIntTime(const int intTime);

		int vForce(int SMU, double vF);
		int sendPersCmd(char cmd[], int len);
		//int iForce();
		//int vMeas();
		//int tMeas(double &measT);
		int iMeas(int SMU, double & measVal);
		bool ktStatus;
		~ktCmd();
	private:
		char vForceCMD_[29];
		void GPIBCleanup(const char * ErrorMsg);
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
