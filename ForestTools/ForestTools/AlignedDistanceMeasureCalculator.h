/******************************************************************************
DeAngelo Wilson
January 3 2020

						AlignedDistanceMeasureCalculator (strategy -- base)
******************************************************************************/
#ifndef _AlignedDistanceMeasureCalculator
#define _AlignedDistanceMeasureCalculator

#include "DistanceMeasureCalculator.h"

namespace distanceMeasure
{

	class AlignedDistanceMeasureCalculator: public DistanceMeasureCalculator
	{
	public:
		//BIG 4
		AlignedDistanceMeasureCalculator() = default;
		AlignedDistanceMeasureCalculator(const AlignedDistanceMeasureCalculator&) = delete;
		AlignedDistanceMeasureCalculator& operator=(const AlignedDistanceMeasureCalculator&) = delete;
		virtual ~AlignedDistanceMeasureCalculator() = default;

		//main calculator driver func -- output matrix as text-file
			//calculate_sequence_set_trees()
		void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id) override = 0;
		float calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const override = 0;
		float normalize(int differenceCount, int sequencesize) const override = 0;

		//CALLED BY DistanceMatrixCalculator
		//opens log file -- for derived calc
		void InitializeSequenceSetTimingsLog() override;
		//write total calc time to log FILE --> closes file
		void LogTotalCalculationTime() override;

		void AddAlignmentTime(double offset) { this->alignmentOffsetTime += offset; };
		void ResetAlignmentTime() { this->alignmentOffsetTime = 0; };
	protected:
		FILE* pAlignmentTimingsLogFile = nullptr;
		//alignment time for entire sequence set + quartet combinations of SS
		double alignmentOffsetTime = 0;
		//total alignment time
		double totalAlignmentTime = 0;


		//debug - timings helpers
		void StartCalculationTimer() override;
		void StopCalculationTimer(int batchID, const std::string& sequenceSet) override;


	private:
		//CALLED BY StopCalculationTimer()
		void LogSequenceSetAlignmentTiming(int batchID, double calculationTime, const std::string& sequenceSet) const;


	};
}


#endif // !_AlignedDistanceMeasureCalculator
