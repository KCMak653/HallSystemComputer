// Microcontroller constant class

#pragma once

namespace MC
{
	class mcConst
	{
	public:
		mcConst(double iPos);

		int movePosition(double fPos);

		int move(double fPos, double speed); //steps per second?

		int changeDirection();
		
		~mcConst();

	private:
		double iPos_;
		double fPos_;
		double cPos_;
		int direc_; // -1 is left, 1 is right
		int rLimN_;
		int lLimN_;
		int mLSteps_;
		MC::mcCmd* mc_;
	};
}
