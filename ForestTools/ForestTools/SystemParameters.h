/******************************************************************************
DeAngelo Wilson
January 24 2020

						System Parameters (singleton)
******************************************************************************/


//TODO
//Format strings for files.. reference rogers rb file

#ifndef _SystemParameters
#define _SystemParameters

#include <string>

class SystemParameters
{
public:
	//API interface
	
	//void Terminate();
	//void Initialize();

	//std::string GetFileFormatString() { return SystemParameters::Instance().format_string; };
	
private:
	SystemParameters() = default;

	static SystemParameters& Instance()
	{
		if(pInstance)
		{
			pInstance = new SystemParameters();
		}
		return *pInstance;
	}
	static SystemParameters* pInstance;

	//private members
	//std::string format_string;
	
};

#endif

