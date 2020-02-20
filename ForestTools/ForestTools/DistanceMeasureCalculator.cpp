/******************************************************************************
DeAngelo Wilson
January 24 2020

                        Distance MEasure Calculator
******************************************************************************/


#include "DistanceMeasureCalculator.h"

//Sequence_set_size (N) Choose (4) --> number of quartet matrices 
int distanceMeasure::DistanceMeasureCalculator::GetQuartetCombinations(int n)
{
	//size of quartet
	int k = 4;
	
	if (k > n) return 0;
	if (k * 2 > n) k = n - k;
	if (k == 0) return 1;

	int result = n;
	for (int i = 2; i <= k; ++i) {
	    result *= (n - i + 1);
	    result /= i;
	}
	return result;
}

void distanceMeasure::DistanceMeasureCalculator::swap_space_with_underscores(std::string& description_string)
{
	//replace all spaces w/ "__underscores__"
	for (auto i = 0u; i < description_string.size(); i++)
	{
		if (isspace(description_string.at(i)))
		{
			description_string.replace(i, 1u, 1u, '_');
		}
	}//--> create_one_word_species_names
}