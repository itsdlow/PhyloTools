/******************************************************************************
DeAngelo Wilson
January 3 2020

						PValueDistanceCalculator (strategy -- derived)
******************************************************************************/
#ifndef _PValueDistanceCalculator
#define _PValueDistanceCalculator

#include "DistanceMeasureCalculator.h"

namespace distanceMeasure
{
	class PValueDistanceCalculator: public DistanceMeasureCalculator
	{
	public:
		//BIG 4
		PValueDistanceCalculator() = default;
		PValueDistanceCalculator(const PValueDistanceCalculator&) = delete;
		PValueDistanceCalculator& operator=(const PValueDistanceCalculator&) = delete;
		~PValueDistanceCalculator() = default;

		float operator()(const FileObject& file1, const FileObject& file2) const override;
		float normalize(int differenceCount, long sequencesize) const override;

	private:


	};

}

#endif // !_PValueDistanceCalculator
