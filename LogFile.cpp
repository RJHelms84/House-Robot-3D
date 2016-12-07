#include "Engine.h"
using namespace std;

//create / open output.txt file
//for stream input to go
std::ofstream out("output.txt");

namespace Batman
{
	//constructor
	LogFile::LogFile() {}
	//destructor
	LogFile::~LogFile()
	{
		//ensure the output isn't corrupt
		//(is good) and then close the
		//output file
		if(out.good())
			out.close();
	}

	void LogFile::Print(string s)
	{
		//stream the string to the output file
		out << s << endl;
	}

	void LogFile::Print(char c[])
	{
		//stream the char to the output file
		out << c << endl;
	}

	std::ofstream& LogFile::operator << (char c[])
	{
		//stream the char to the output file
		out << c;
		return out;
	}

	std::ofstream& LogFile::operator << (string s)
	{
		out << s;
		return out;
	}

	std::ofstream& LogFile::operator << (double d)
	{
		out << d;
		return out;
	}

	std::ofstream& LogFile::operator << (float f)
	{
		out << f;
		return out;
	}

	std::ofstream& LogFile::operator << (int i)
	{
		out << i;
		return out;
	}

	std::ofstream& LogFile::operator << (bool b)
	{
		//stream the boolean with text values
		//to ouput files
		if(b)
			out << "True";
		else
			out << "False";

		return out;
	}
};