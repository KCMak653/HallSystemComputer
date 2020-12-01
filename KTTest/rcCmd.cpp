#include <string>
#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>
#include "rcCmd.h"

//COM communication parameters

#define BAUDRATE_RC CBR_9600 //Baud rate
#define BYTESIZE_RC 8 //Byte size
#define STOPBITS_RC ONESTOPBIT //Number of stop bits
#define PARITY_RC NOPARITY //Parity state

//timeouts

#define RDINTTIME 50
#define RDTOTTIME 50
#define RDTOTMULT 10
#define WTTOTTIME 50
#define WTTOTMULT 10

namespace RC 
{
	rcCmd::rcCmd(){
		std::cout<<"Opening Serial Port"<<std::endl;
		char comPort[] = "COM3";
		char binCmd[] ="00000000";
		strcpy(CMD_, binCmd);
		strcpy(ComPortName_, comPort);

		hComm_ = CreateFile(ComPortName_, 
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		if (hComm_ == INVALID_HANDLE_VALUE){
			std::cout<<"Error in opening serial port"<<std::endl;
		}

		
		DCB dcbSerialParams = {0};
		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
		Status = GetCommState(hComm_, &dcbSerialParams);

		if (!Status){
			std::cout<<"Error in GetCommState()"<<std::endl;
		}

		dcbSerialParams.BaudRate = BAUDRATE_RC;
		dcbSerialParams.ByteSize = BYTESIZE_RC;
		dcbSerialParams.StopBits = STOPBITS_RC;
		dcbSerialParams.Parity = PARITY_RC;

		Status = SetCommState(hComm_, &dcbSerialParams);

		if (!Status){
			std::cout<<"Error in setting DCB Structure"<<std::endl;
		}

		//Set the timeouts in ms

		COMMTIMEOUTS timeouts = {0};
		timeouts.ReadIntervalTimeout = RDINTTIME;	
		timeouts.ReadTotalTimeoutConstant = RDTOTTIME;
		timeouts.ReadTotalTimeoutMultiplier = RDTOTMULT;
		timeouts.WriteTotalTimeoutConstant = WTTOTTIME;
		timeouts.WriteTotalTimeoutMultiplier = WTTOTMULT;

		Status = SetCommTimeouts(hComm_, &timeouts);

		if (!Status){
			std::cout<<"Error in setting timeouts"<<std::endl;
		}
		std::cout<<"Relay Channels Initialized"<<std::endl;
		
		//std::cout<<"Ok here"<<std::endl;
		//CloseHandle(hComm_);
	}

	int rcCmd::sendCmd_()
	{
		purge();
		std::string binStr = CMD_;
		std::cout<<binStr<<std::endl;
		int val = std::stoi(binStr, 0, 2);
		unsigned char cm1[] = {val};
		int dNoOfBytestoWrite = 1;

		DWORD dNoOfBytesWritten = 1;

		Status = WriteFile(hComm_, 
				cm1,
				dNoOfBytestoWrite,
				&dNoOfBytesWritten,
				NULL);

		
		if (!Status){
			std::cout<<"Error sending command"<<std::endl;
		}
		
		return 0;
	}
	int rcCmd::chOn(int dev)
	{
		CMD_[8-dev] = '1';
		sendCmd_();
		return 0;
	}

	int rcCmd::chOff(int dev)
	{
		CMD_[8-dev] = '0';
		sendCmd_();
		return 0;
	}

	int rcCmd::chAllOn()
	{
		for (int i=0; i<8; i++){
			CMD_[i] = '1';
		}
		sendCmd_();
		return 0;
	}

	int rcCmd::chAllOff()
	{
		for (int i=0; i<8; i++){
			CMD_[i] = '0';
		}
		sendCmd_();
		return 0;
	}
	int rcCmd::purge(){
		DWORD dwFlags = PURGE_RXABORT;
		bool purgeStatus;
		purgeStatus = PurgeComm(hComm_, dwFlags);
		dwFlags = PURGE_RXCLEAR;
		purgeStatus = PurgeComm(hComm_, dwFlags);
		dwFlags = PURGE_TXABORT;
		purgeStatus = PurgeComm(hComm_, dwFlags);
		dwFlags = PURGE_TXCLEAR;
		purgeStatus = PurgeComm(hComm_, dwFlags);

		if (!purgeStatus){
			std::cout<<"Error with purge"<<std::endl;
			return 1;
		}
		else
		{
			std::cout<<"Cmds purged"<<std::endl;
			return 0;
		}
	}

	rcCmd::~rcCmd(){
		//At some point include command to close all channels
		// chAllOff();
		CloseHandle(hComm_);
		std::cout<<"Relay Channels Shut Down"<<std::endl;
	}
		
}