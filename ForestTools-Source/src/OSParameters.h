/**************************************************************************************************************************
DeAngelo Wilson
February 26 2020

					OSParameters
						-- Perform analysis of trees for BatchDistanceCalculators
							--Compute Compatibilities for permutations of calculator_methods
								--outputs files to ../ForestFiles/Analysis/..
**************************************************************************************************************************/

#ifndef _OSParameters
#define _OSParameters

#include <string>

class OSParameters
{
public:
	OSParameters() = default;

	virtual ~OSParameters() = default;
	OSParameters(const OSParameters& bca) = delete;
	OSParameters& operator=(const OSParameters& bca) = delete;

private:

};

#endif


