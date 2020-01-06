/******************************************************************************
DeAngelo Wilson
January 3 2020

						LcsDistanceCalculator (strategy -- derived)
******************************************************************************/
#ifndef _LcsDistanceCalculator
#define _LcsDistanceCalculator

#include "DistanceMeasureCalculator.h"

namespace distanceMeasure
{
	class LcsDistanceCalculator: public DistanceMeasureCalculator
	{
	public:
		//BIG 4
		LcsDistanceCalculator() = default;
		LcsDistanceCalculator(const LcsDistanceCalculator&) = delete;
		LcsDistanceCalculator& operator=(const LcsDistanceCalculator&) = delete;
		~LcsDistanceCalculator() = default;

		float operator()(const FileObject& file1, const FileObject& file2) const override;
		float normalize(int differenceCount, int sequencesize) const override;

	private:
		//helper funcs
		const int getArrayIndex(int i, int j, int n) const;

		inline int maxSequenceLength(int sequencesize1, int sequencesize2) const;
	};
}



#endif // !_LcsDistanceCalculator
