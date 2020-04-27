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
	struct RunFlags
	{
		RunFlags(bool genQuartets, float closeness_limit):
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
