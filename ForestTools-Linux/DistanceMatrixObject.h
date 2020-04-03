/******************************************************************************
DeAngelo Wilson
January 3 2020*

						DistanceMatrixObject
							--Tree creation driver
******************************************************************************/

#ifndef _DistanceMatrixObject
#define _DistanceMatrixObject


//forward declarations
	//#include DMOfwd.h;
#include "FileObjectManager.h"

//#include "DistanceMeasureCalculator.h"
//all calc's need to be available...************************************
	//#include "FileObject.h"

#include <vector>
#include <string>

namespace distanceMeasure
{
	class DistanceMeasureCalculator;
	//typedef std::function<float (const FileObject& file1, const FileObject& file2)> DistanceMeasureCalculator;

	class DistanceMatrixObject
	{
	public:

		//BIG 4
		DistanceMatrixObject() = delete;
		~DistanceMatrixObject();	
		DistanceMatrixObject(const DistanceMatrixObject& dmo) = delete;
		DistanceMatrixObject& operator=(const DistanceMatrixObject& dmo) = delete;

		
		
		//custom constructor	
		explicit DistanceMatrixObject(std::string sequence_names_dir, std::string sequences_dir, DistanceMeasureCalculator* dmc);
		
		//helper functions
		//
			//fill results buffer with distance Measures + LAMDAMATRIX for allquartetsMethod
		//void CalculateLargeTreeDistanceMeasures(const std::vector<std::string>& sequence_set_names);
		//void CalculateAllQuartetsDistanceMeasures(const std::vector<std::string>& sequence_set_names);

		//TESTING FUNCTION!!!! sets lamda vector with array values (UNSAFE)
		//void setCalculateDistanceMeasureTEST(float* array);

		
		//write results buffer to output FILE, closes FILE
		//void write_batch_results(const int batch_number, const size_t sequence_count);
		//batch DistanceMEasure_CAlculator funcs
		void batch_matrix_calculation(const std::string& sequences_list_dir);
		//void batch_tree_creation();

		//void batch_calculate_trees(const std::string& sequences_list_dir);

		//void batch_analyze_trees();
		
		//accessor methods
		DistanceMeasureCalculator* getDistanceMeasureFunc() const;
		const FileObjectManager& getFileObjectManager() const;
		//FILE* getFILEPtr() const;

	private:
		//FILE* pResults;
		//FILE* pQuartetResults;
		//FILE* pTimesLogFile;
		
		FileObjectManager fileObjectManager;

		//std::string results;
		//std::string quartetResults;

		DistanceMeasureCalculator* distanceMeasureFunc;

		//2D array (distanceMATRIX) of floats
		//std::vector<float> lamdaMatrix;

		//private helper
		//process a line of "sequenceList" file --> batch matrix_creation
		const std::vector<std::string> ProcessSequenceSet(const std::string& sequence_set) const;
		const std::string swap_underscores(const std::string& str) const;

		
		//called in distanceMeasure calculation to noramlize lcs value for results matrix
		//const float normalize(int lcs, long maxSequenceSize) const;

		//GENERIC NORMALIZE???
		//const float normalize(int differenceCount, int alignedSequenceSize) const;
		
		//given 3 pairwise sum float values -->
			//checks 4PointCondition (returns 0 on sucess) --> 
				//sets miinPairwiseSum to min
		//int fourPointConditionCheck(float sum1, float sum2, float sum3, float& minPairwiseSum) const;

		//inline int maxSequenceLength(int sequencesize1, int sequencesize2) const;
		//inline const int getArrayIndex(int row, int col, int rowCount) const;
		//inline void writeQuartetMatrix(int i, int j, int k, int l, const std::vector<std::string>& sequence_set_names, const int fileCount);
	};

	//struct pvalueCalculator
	//{
	//	float operator()(const FileObject& file1, const FileObject& file2) const
	//	{
	//		//assumes aligned sequences
	//		const int m = file1.sequencesize;
	//		const int n = file2.sequencesize;
	//		if(m != n)
	//		{
	//			return 0;
	//		}

	//		int differenceCount = 0;
	//		//loop thru sequences comparing relative indexes
	//		for(int i = 0; i < m; i++)
	//		{
	//			//compare sequences[i] --
	//			if(file1.sequenceBuffer[i] != file2.sequenceBuffer[i])
	//			{
	//				differenceCount++;
	//			}
	//		}
	//		//printf("simCount = %d\n", similarityCount);
	//		//Measure::
	//			//normalize(count) == 1 --> MAXIMALLY DIFFERENT sequences
	//			//normalize(count) == 0 --> IDENTICAL SEQUENCES
	//		return normalize(differenceCount, m);
	//	}

	//	float normalize(int differenceCount, long sequencesize) const
	//	{
	//		return static_cast<float>(differenceCount) / sequencesize;
	//	}

	//};
	//struct LCSCalculator
	//{
	//	float operator()(const FileObject& file1, const FileObject& file2) const
	//	{

	//		const int m = file1.sequencesize;
	//		const int n = file2.sequencesize;
	//		const int totalsize = m * n;

	//		/*
	//		const int testSize = totalsize + m + n;
	//		int* c = new int[testSize];

	//		for (int i = 0; i <= m ; i++) 
	//		{
	//			for (int j = 0; j <= n; j++) 
	//			{
	//				if(j==0 || i ==0)
	//				{
	//					c[getArrayIndex(i,j,n)] = 0;
	//				}
	//				// buffer[i-1] "-1" to account for Epsilon
	//				else if (file1.sequenceBuffer[i - 1] == file2.sequenceBuffer[j - 1])
	//				{
	//					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i - 1, j - 1, n)] + 1;
	//				}
	//				else if (c[getArrayIndex(i - 1, j, n)] >= c[getArrayIndex(i, j - 1, n)])
	//				{
	//					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i - 1, j, n)];
	//				}
	//				else
	//				{
	//					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i, j - 1, n)];
	//				}
	//			}
	//		}
	//		*/
	//		

	//		int* c = new int[totalsize];

	//		for (int i = 0; i < totalsize; i += n)
	//		{
	//			c[i] = 0;
	//		}
	//		for (int j = 0; j < n; j++) 
	//		{
	//			c[j] = 0;
	//		}
	//		//~.04 seconds to initalize values to 0
	//		for (int i = 1; i < m ; i++) 
	//		{
	//			for (int j = 1; j < n; j++) 
	//			{
	//				// buffer[i-1] "-1" to account for Epsilon
	//				//if char at index is
	//				if (file1.sequenceBuffer[i - 1] == file2.sequenceBuffer[j - 1])
	//				{
	//					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i - 1, j - 1, n)] + 1;
	//				}
	//				else if (c[getArrayIndex(i - 1, j, n)] >= c[getArrayIndex(i, j - 1, n)])
	//				{
	//					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i - 1, j, n)];
	//				}
	//				else
	//				{
	//					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i, j - 1, n)];
	//				}
	//			}
	//		}
	//		//CHEATY CHEATY + 1 -- improper LCS CALCULATION ON SELF (FIXXX!!!!!!!!!!)
	//		int result = c[getArrayIndex(m-1, n-1, n)] + 1;
	//		//int* resAddr = &c[getArrayIndex(m - 1, n - 1, n)];
	//		delete[] c;
	//		//return last index of array
	//		return normalize(result, maxSequenceLength(file1.sequencesize, file2.sequencesize));

	//	}

	//	const int getArrayIndex(int i, int j, int n) const
	//	{
	//			return (i*n) + j;
	//	}

	//	const float normalize(int lcs, long maxSequenceSize) const
	//	{

	//		//cheaty + 1 to set LCS to correct value???**************LCS CALC OFF BY 1********
	//	
	//		float normalizedDistance = float(maxSequenceSize - lcs) / maxSequenceSize;

	//		printf("%f -- %d -- %d\n", normalizedDistance, lcs, maxSequenceSize);
	//		return normalizedDistance;
	//	}

	//	inline long maxSequenceLength(long sequencesize1, long sequencesize2) const
	//	{
	//		return sequencesize1 > sequencesize2 ? sequencesize1 : sequencesize2;
	//	}


	//};

	


}


/*




int FileObject::LCS(char*& sequence, const int &size)
{
	const int m = this->sequencesize;
	const int n = size;
	const int totalsize = m * n;

	int* c = new int[totalsize];

	for (int i = 0; i < totalsize; i += n)
	{
		c[i] = 0;
	}
	for (int j = 0; j < n; j++) {
		c[j] = 0;
	}
	//~.04 seconds to initalize values to 0
	for (int i = 1; i < m; i++) {
		for (int j = 1; j < n; j++) {
			// buffer[i-1] "-1" to account for Epsilon
			if (this->sequenceBuffer[i - 1] == sequence[j - 1])
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
	int result = c[getArrayIndex(m - 1, n - 1, n)];

	delete[] c;
	//return last index of array
	return result;
}

int FileObject::getArrayIndex(int i, int j, int n)
{
	return (i*n) + j;
}




*/

#endif // !_DistanceMatrixObject
