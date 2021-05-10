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
		unsigned int calculators_bitmask;
		int calculator_count;
		float closeness_factor;
		bool generate_quartets;
		bool generate_analysis;
		bool compare_tree_calculator;//set true if using compare tree calc in analaysis --> CANNOT do Batch Sequence Listing (only FileSet)
		bool align_sequences;

		RunFlags():
		calculators_bitmask(0),
		calculator_count(0),
		closeness_factor(0.0f),
		generate_quartets(false),
		generate_analysis(true),
		compare_tree_calculator(false),
		align_sequences(false)
		{
		}
		
		RunFlags(bool genQuartets, float closeness_limit):
		calculators_bitmask(0),
		calculator_count(0),
		closeness_factor(closeness_limit),
		generate_quartets(genQuartets),
		generate_analysis(true),
		compare_tree_calculator(false),
		align_sequences(false)

		{
		}
		RunFlags(bool genQuartets, float closeness_limit, unsigned int calculators_bitmask, int calculator_count) :
		calculators_bitmask(calculators_bitmask),
		calculator_count(calculator_count),
		closeness_factor(closeness_limit),
		generate_quartets(genQuartets),
		generate_analysis(true),
		compare_tree_calculator(false),
		align_sequences(false)
		{
		}
		
		void SetCompareTreeCalculator()
		{
			this->compare_tree_calculator = true;
		}

	};
}



#endif
