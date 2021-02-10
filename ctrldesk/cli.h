#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>

using namespace std;

class CLI
{
	public:
		//Constructor
		CLI();

		//This method is returning error code
		DWORD getError();

		//This method is checking availability of errors
		//Returns "true" if there any errors or "false" if not
		bool checkForErrors();

		//This method returns path of images
		vector <string> getPath(vector <string> argv_vector);

	private:
		//This method converts wstring into string
		string ConvertWideToUtf8(wstring wstr);

		//This method converts string into wstring
		wstring ConvertUtf8ToWide(string str);

		//This variable contains error code
		DWORD dwError = 0;
		
};
