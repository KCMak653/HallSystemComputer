//Micro controller Command Class

//This class creates the micro controller object and directly 
//communicates to it via writing and reading commands
//by the GPIB

#pragma once

extern const int vForceVInd;
extern const int vForceSMUInd;
extern const int vForceVSize;

namespace MC
{
	class mcCmd
	{
	public:
		//Constructor initializes command arrays and opens handle to
			//machine
		mcCmd();
		
		int setSpeed(int speed);
		
		int setNSteps(int steps);
		
		int reverseDirection();
		int setDirection(int direction);
		int move();
		int setRefPt();
		int moveRefPt(int pos);
		int moveOrigin();
		int getPosition();

		
		bool mcStatus;
		
		~mcCmd();
	private:
		void GPIBCleanup(const char * ErrorMsg);
		
		char moveCMD_[3];
		char setSpeedCMD_[7];
		char setNStepsCMD_[9];
		char setDirectionCMD_[3];
		char setRefPtCMD_[3];
		char AmoveRefPtCMD_[9];
		char moveRefPtCMD_[9];
		char moveOriginCMD_[3];
		int position_;
		int Dev_;
		int currDirec_;
		int steps_;
	};
}