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
		RunFlags(bool genQuartets):
		generate_quartets(genQuartets)
		{
		}

		int calculators_bitmask;
		int calculator_count;
		bool generate_quartets;		
	};
}



#endif
