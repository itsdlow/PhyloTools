/******************************************************************************
DeAngelo Wilson
January 18 2020

			InternalDistanceMeasureCalculator (strategy -- derived-base)
					--DistanceMatrix created by hand
******************************************************************************/
#ifndef _InternalDistanceMeasureCalculator
#define _InternalDistanceMeasureCalculator

#include "DistanceMeasureCalculator.h"

#include <string>
#include <vector>

namespace distanceMeasure
{
	class InternalDistanceMeasureCalculator: public DistanceMeasureCalculator
	{
	public:
		//BIG 4
		InternalDistanceMeasureCalculator() = default;
		InternalDistanceMeasureCalculator(const InternalDistanceMeasureCalculator&) = delete;
		InternalDistanceMeasureCalculator& operator=(const InternalDistanceMeasureCalculator&) = delete;
		~InternalDistanceMeasureCalculator() = default;

		float calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const override = 0;
		float normalize(int differenceCount, int sequencesize) const override = 0;
		void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id) override;

		//internal calc specific funcs
					//fill results buffer with distance Measures + LAMDAMATRIX for allquartetsMethod
		void CalculateLargeTreeDistanceMeasures(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names);
		void CalculateAllQuartetsDistanceMeasures(const std::vector<std::string>& sequence_set_names);
		
		void write_quartet_matrix(int i, int j, int k, int l, const std::vector<std::string>& sequence_set_names, const int fileCount);
		
		//write results buffer to output FILE, closes FILE
		void write_batch_results(const int batch_number, const size_t sequence_count);
		
	private:
		FILE* pResults;
		FILE* pQuartetResults;

		std::string results;
		std::string quartetResults;

		//2D array (distanceMATRIX) of floats
		std::vector<float> lamdaMatrix;

	};
}


#endif // !_InternalDistanceMeasureCalculator
