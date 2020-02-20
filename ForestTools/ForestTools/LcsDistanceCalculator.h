/******************************************************************************
DeAngelo Wilson
January 3 2020

						LcsDistanceCalculator (strategy -- derived)
******************************************************************************/
#ifndef _LcsDistanceCalculator
#define _LcsDistanceCalculator

#include "InternalDistanceMeasureCalculator.h"

namespace distanceMeasure
{
	class LcsDistanceCalculator: public InternalDistanceMeasureCalculator
	{
	public:
		//BIG 4
		LcsDistanceCalculator() = default;
		LcsDistanceCalculator(const LcsDistanceCalculator&) = delete;
		LcsDistanceCalculator& operator=(const LcsDistanceCalculator&) = delete;
		virtual ~LcsDistanceCalculator() = default;

		float calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const override;
		float normalize(int differenceCount, int sequencesize) const override;

		std::string GetCalculatorName() const override;

	private:
		//helper funcs
		//const int getArrayIndex(int i, int j, int n) const;

		inline int maxSequenceLength(int sequencesize1, int sequencesize2) const;
	};
}



#endif // !_LcsDistanceCalculator
