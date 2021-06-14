

#ifndef _VectorKeyCompare
#define _VectorKeyCompare


#include <string>

namespace distanceMeasure
{
	struct VectorKeyCompare
	{
		typedef const std::pair<std::string, int> species_and_size;

		const std::string species_of_interest;

		VectorKeyCompare(const std::string& str) :
			species_of_interest(str)
		{

		}
		bool operator()(species_and_size& element) const
		{
			return element.first == species_of_interest;
		}
	};
}

#endif

