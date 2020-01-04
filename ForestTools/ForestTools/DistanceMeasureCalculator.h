/******************************************************************************
DeAngelo Wilson
January 3 2020

						DistanceMeasureCalculator (strategy -- base)
******************************************************************************/
#ifndef _DistanceMeasureCalculator
#define _DistanceMeasureCalculator

#include "FileObject.h"

namespace distanceMeasure
{
	class DistanceMeasureCalculator
	{
	public:
		//BIG 4
		DistanceMeasureCalculator() = default;
		DistanceMeasureCalculator(const DistanceMeasureCalculator&) = delete;
		DistanceMeasureCalculator& operator=(const DistanceMeasureCalculator&) = delete;
		~DistanceMeasureCalculator() = default;

		virtual float operator()(const FileObject& file1, const FileObject& file2) const = 0;
		virtual float normalize(int differenceCount, long sequencesize) const = 0;

	private:


	};
}


#endif // !_DistanceMeasureCalculator
