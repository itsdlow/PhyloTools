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
	//NOTE:: FLAGS SET IN MAIN -- copied to other calculators
		//COULD act as blackboard between calculators...
	struct RunFlags
	{
		RunFlags(bool genQuartets, float closeness_limit):
		calculators_bitmask(0),
		calculator_count(0),
		generate_quartets(genQuartets),
		closeness_factor(closeness_limit)
		{
		}

		int calculators_bitmask;
		int calculator_count;
		bool generate_quartets;
		float closeness_factor;
	};
}



#endif
