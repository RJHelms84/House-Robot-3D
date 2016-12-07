#pragma once

#include "Engine.h"

namespace Batman
{
	class LogFile
	{
	public:
		//constructor
		LogFile();
		//Destructor
		~LogFile();
		//print operators
		void Print(std::string s);
		void Print(char c[]);

		//overloading operators for streaming to file
		std::ofstream& operator << (char c[]);
		std::ofstream& operator << (std::string s);
		std::ofstream& operator << (double d);
		std::ofstream& operator << (float f);
		std::ofstream& operator << (int i);
		std::ofstream& operator << (bool b);
	};
};

//create instance of LogFile class called debug, and use static so it is globally accessible
static Batman::LogFile debug;