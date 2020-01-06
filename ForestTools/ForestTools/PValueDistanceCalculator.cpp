

#include "PValueDistanceCalculator.h"

namespace distanceMeasure
{

	float PValueDistanceCalculator::operator()(const FileObject& file1, const FileObject& file2) const
	{
		//assumes aligned sequences
		const int m = file1.GetSequenceSize();
		const int n = file2.GetSequenceSize();
		if (m != n)
		{
			return 0;
		}

		int differenceCount = 0;
		//loop thru sequences comparing relative indexes
		for (int i = 0; i < m; i++)
		{
			//compare sequences[i] --
			if (file1.GetSequenceCharAt(i) != file2.GetSequenceCharAt(i))
			{
				differenceCount++;
			}
		}
		//printf("simCount = %d\n", similarityCount);
		//Measure::
			//normalize(count) == 1 --> MAXIMALLY DIFFERENT sequences
			//normalize(count) == 0 --> IDENTICAL SEQUENCES
		return this->normalize(differenceCount, m);
	}

	float PValueDistanceCalculator::normalize(int differenceCount, int sequencesize) const
	{
		return static_cast<float>(differenceCount) / sequencesize;
	}
}
