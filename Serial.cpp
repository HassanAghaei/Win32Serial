// ********************************** Henri Terminal May 2017

#include "stdafx.h"
#include <iostream>   //cout 
#include <Windows.h>
#include <stdio.h>
#include <string>

using namespace std;

void main(void)
{


	SetConsoleTitle(L"Henri Terminal 2017");  // To change the name of the Console
	// To chnage the icon, you need to add a resource file and add:
	//AAA ICON myicon.ico  to it

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);  // Background is black with red fonts


Henri1:

	char  TempChar;
	char  SerialBuffer[256];
	DWORD NoBytesRead;
	HANDLE hComm;
	int i = 0;
	BOOL   Status;



	// Open the comport:


	std::wstring comnum;
	std::wcout << TEXT("\n\nEnter the port (ex.: COM6): ");

	std::wcin >> comnum;

	std::wstring comprefix = TEXT("\\\\.\\");
	std::wstring comID = comprefix + comnum;
	


	hComm = CreateFile(comID.c_str(),
		GENERIC_READ | GENERIC_WRITE,      // Read/Write Access
		0,                                 // No Sharing, ports cant be shared
		NULL,                              // No Security
		OPEN_EXISTING,                     // Open existing port only
		0,                                 // Non Overlapped I/O
		NULL);                             // Null for Comm Devices

	if (hComm == INVALID_HANDLE_VALUE)
		std::wcout << "Error! - Port " << comID.substr(7, 11) << "can't be opened";   // wcout not cout
	else
		std::wcout <<"Port  " <<comID.substr(7,11) << " Opened\n\r";  // specific part of the string


	// Serial Settings:

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hComm, &dcbSerialParams);     // What is the current settings?

	if (Status == FALSE)
		std::cout << " Error! in GetCommState()";

	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;   // 1 STOP BIT
	dcbSerialParams.Parity = NOPARITY;

	SetCommState(hComm, &dcbSerialParams);

	if (Status == FALSE)
	{
		std::cout << "  Error! in Setting DCB Structure";
	}
	else
	{
		std :: cout <<     "Setting DCB Structure Successfull" << endl ;

		std::cout << "-----------------------------------------" << endl;
		std::cout << "Baudrate "<< dcbSerialParams.BaudRate << endl;
		std::cout << "ByteSize " << dcbSerialParams.ByteSize << endl;
	    std::cout << "StopBits " << 1 << endl;
		std::cout << "Parity   " << dcbSerialParams.Parity << endl;
		std::cout << "-----------------------------------------" << endl;
	}

	//Timeout:

	COMMTIMEOUTS timeouts = { 0 };

	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(hComm, &timeouts) == FALSE)
		std::cout << "Error! in Setting Time Outs";
	else
		std::cout << "Setting Serial Port Timeouts Successfull!" << endl;


	// Write Process:

	char   lpBuffer[256] = " ";		       // lpBuffer should be  char or byte array, otherwise write wil fail
	//char * lpBuffer[5];
	DWORD  dNoOFBytestoWrite;              // No of bytes to write into the port
	DWORD  dNoOfBytesWritten;          // No of bytes written to the port



	std::cout << "Type in your charcter:" << endl;
	std::cin >> lpBuffer;
	//	std::cin.getline(lpBuffer, 256);     // You may use getline but it is tricky to get it to work with serial setting!
	lpBuffer[3] = { 0x0d };   // Emulate the Enter Key! 



	dNoOFBytestoWrite = sizeof(lpBuffer); // Calculating the no of bytes to write into the port

	Status = WriteFile(hComm,               // Handle to the Serialport
		lpBuffer,            // Data to be written to the port 
		dNoOFBytestoWrite,   // No of bytes to write into the port
		&dNoOfBytesWritten,  // No of bytes written to the port
		NULL);

	if (Status == TRUE)
		std::wcout << "\n\n  Written to Port " << comID.substr(7, 11) <<endl;
	else
	{
		//	printf("\n\n   Error %d in Writing to Serial Port", GetLastError());
		if (GetLastError() == 5 || GetLastError() == 6)

		{
			std::cout << "Try Agian!\n\r";
		}

	}


	//---- READ:




	Status = SetCommMask(hComm, EV_RXCHAR);

	if (Status == 0)
		std::cout <<   "Error! in Setting CommMask" ;
	else
		std::cout <<  "CommMask successfull!" << endl;






	if (Status == FALSE)
	{
		std::cout << "\n    Error! in Setting WaitCommEvent():" ;
	}
	else
	{
		std::cout <<    " Characters Received:" << endl;
		do
		{
			Status = ReadFile(hComm, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
			SerialBuffer[i] = TempChar;
			i++;
		} while (NoBytesRead > 0);


		// Print the Read:

		
		int j = 0;
		for (j = 3; j < i - 1; j++)		// j == 3 to skip the echo!
			std::cout << SerialBuffer[j] ;

	}
	CloseHandle(hComm);

	Sleep(1000);  // 1 sec. sleep
	goto Henri1; // jump to the beginning and ask the user to type in more charcters
	system("pause");
}
