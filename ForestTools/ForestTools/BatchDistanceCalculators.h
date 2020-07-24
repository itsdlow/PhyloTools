/******************************************************************************
DeAngelo Wilson
January 3 2020

						BatchDistanceCalculators (strategy -- derived)
******************************************************************************/
#ifndef _BatchDistanceCalculators
#define _BatchDistanceCalculators

#include "DistanceMeasureCalculator.h"

namespace distanceMeasure
{
	//forward decls
	class BatchCalculatorsAnalyzer;
	
	class BatchDistanceCalculators : public DistanceMeasureCalculator
	{
	public:
		//BIG 4
		BatchDistanceCalculators(RunFlags* flags, const std::string& name);
		BatchDistanceCalculators(const BatchDistanceCalculators&) = delete;
		BatchDistanceCalculators& operator=(const BatchDistanceCalculators&) = delete;
		virtual ~BatchDistanceCalculators();

		void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id) override;

		//internal calc specific
		float calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const override;
		float normalize(int differenceCount, int sequencesize) const override;
		void write_quartet_matrix(FileObjectManager& fileObjectManager, const std::vector<int>& speciesSequenceSetIndexes, const std::vector<std::string>& sequence_set_names, const int fileCount, const int quartet_count) override;

		
		//std::string GetCalculatorName() const override;

		//opens log file -- for derived calc(s)
		void InitializeSequenceSetTimingsLog(const int total_sequence_count) override;
		//write total calc time to log FILE --> closes file
		void LogTotalCalculationTime() override;

	private:
		const int calculator_count;

		////array of calculators to perform batch analysis on
		DistanceMeasureCalculator** calculators;

		BatchCalculatorsAnalyzer* poBatchAnalyzer;
	};
}



#endif // !_BatchDistanceCalculators
