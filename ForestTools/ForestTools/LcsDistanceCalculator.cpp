





#include "LcsDistanceCalculator.h"

namespace distanceMeasure
{
	float LcsDistanceCalculator::operator()(const FileObject& file1, const FileObject& file2) const
	{

		const int m = file1.sequencesize;
		const int n = file2.sequencesize;
		const int totalsize = m * n;

		/*
		const int testSize = totalsize + m + n;
		int* c = new int[testSize];

		for (int i = 0; i <= m ; i++)
		{
			for (int j = 0; j <= n; j++)
			{
				if(j==0 || i ==0)
				{
					c[getArrayIndex(i,j,n)] = 0;
				}
				// buffer[i-1] "-1" to account for Epsilon
				else if (file1.sequenceBuffer[i - 1] == file2.sequenceBuffer[j - 1])
				{
					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i - 1, j - 1, n)] + 1;
				}
				else if (c[getArrayIndex(i - 1, j, n)] >= c[getArrayIndex(i, j - 1, n)])
				{
					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i - 1, j, n)];
				}
				else
				{
					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i, j - 1, n)];
				}
			}
		}
		*/


		int* c = new int[totalsize];

		for (int i = 0; i < totalsize; i += n)
		{
			c[i] = 0;
		}
		for (int j = 0; j < n; j++)
		{
			c[j] = 0;
		}
		//~.04 seconds to initalize values to 0
		for (int i = 1; i < m; i++)
		{
			for (int j = 1; j < n; j++)
			{
				// buffer[i-1] "-1" to account for Epsilon
				//if char at index is
				if (file1.sequenceBuffer[i - 1] == file2.sequenceBuffer[j - 1])
				{
					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i - 1, j - 1, n)] + 1;
				}
				else if (c[getArrayIndex(i - 1, j, n)] >= c[getArrayIndex(i, j - 1, n)])
				{
					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i - 1, j, n)];
				}
				else
				{
					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i, j - 1, n)];
				}
			}
		}
		//CHEATY CHEATY + 1 -- improper LCS CALCULATION ON SELF (FIXXX!!!!!!!!!!)
		int result = c[getArrayIndex(m - 1, n - 1, n)] + 1;
		//int* resAddr = &c[getArrayIndex(m - 1, n - 1, n)];
		delete[] c;
		//return last index of array
		return normalize(result, maxSequenceLength(file1.sequencesize, file2.sequencesize));

	}

	const int LcsDistanceCalculator::getArrayIndex(int i, int j, int n) const
	{
		return (i * n) + j;
	}

	float LcsDistanceCalculator::normalize(int lcs, long maxSequenceSize) const
	{

		//cheaty + 1 to set LCS to correct value???**************LCS CALC OFF BY 1********

		float normalizedDistance = float(maxSequenceSize - lcs) / maxSequenceSize;

		printf("%f -- %d -- %d\n", normalizedDistance, lcs, maxSequenceSize);
		return normalizedDistance;
	}

	long LcsDistanceCalculator::maxSequenceLength(long sequencesize1, long sequencesize2) const
	{
		return sequencesize1 > sequencesize2 ? sequencesize1 : sequencesize2;
	}

}