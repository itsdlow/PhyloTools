

#include "PValueDistanceCalculator.h"

namespace distanceMeasure
{

	float PValueDistanceCalculator::operator()(const FileObject& file1, const FileObject& file2) const
	{
		//assumes aligned sequences
		const int m = file1.sequencesize;
		const int n = file2.sequencesize;
		if (m != n)
		{
			return 0;
		}

		int differenceCount = 0;
		//loop thru sequences comparing relative indexes
		for (int i = 0; i < m; i++)
		{
			//compare sequences[i] --
			if (file1.sequenceBuffer[i] != file2.sequenceBuffer[i])
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

	float PValueDistanceCalculator::normalize(int differenceCount, long sequencesize) const
	{
		return static_cast<float>(differenceCount) / sequencesize;
	}
}
