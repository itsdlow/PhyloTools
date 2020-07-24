/******************************************************************************
DeAngelo Wilson
January 18 2020

			InternalCalculatorTools (strategy -- derived-base)
					--DistanceMatrix created by hand
******************************************************************************/
#ifndef _RunFlags
#define _RunFlags

namespace distanceMeasure
{
	//COULD act as blackboard between calculators...
	struct RunFlags
	{
		RunFlags():
		calculators_bitmask(0),
		calculator_count(0),
		generate_quartets(false),
		generate_analysis(true),
		closeness_factor(0.0f)
		{
		}
		
		RunFlags(bool genQuartets, float closeness_limit):
		calculators_bitmask(0),
		calculator_count(0),
		generate_quartets(genQuartets),
		generate_analysis(true),
		closeness_factor(closeness_limit)
		{
		}
		RunFlags(bool genQuartets, float closeness_limit, unsigned int calculators_bitmask, int calculator_count) :
		calculators_bitmask(calculators_bitmask),
		calculator_count(calculator_count),
		generate_quartets(genQuartets),
		generate_analysis(true),
		closeness_factor(closeness_limit)
		{
		}
		
		unsigned int calculators_bitmask;
		int calculator_count;
		bool generate_quartets;
		bool generate_analysis;
		float closeness_factor;
	};
}



#endif
