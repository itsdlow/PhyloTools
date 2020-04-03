/******************************************************************************
DeAngelo Wilson
January 18 2020

						IntneralDistanceMEasureCalculator (derived-base)
******************************************************************************/


#include "ExternalDistanceMeasureCalculator.h"

#include "FileObjectManager.h"

namespace distanceMeasure
{
	void ExternalDistanceMeasureCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)
	{
		//if alignment needed
		//ALIGN -- refill_FileObjectManager... done by derived calcs
		//calculate LargeTree giving sequence_names_list
		//this->CalculateLargeTreeDistanceMeasures(fileObjectManager, sequence_set_names);
		//this->CalculateAllQuartetsDistanceMeasures(sequence_set_names);

		//////create file for current sequence_set
		//this->write_batch_results(batch_id, sequence_set_names.size());
	}

	//internal calc specific function -- NOT NEEDED
	void distanceMeasure::ExternalDistanceMeasureCalculator::write_quartet_matrix(FileObjectManager& fileObjectManager, const std::vector<int>& speciesSequenceSetIndexes, const std::vector<std::string>& sequence_set_names, const int fileCount)
	{
		//by default DMC does not have method to write quartet matrix -- derived Internal CAlculators define...
	}
	float distanceMeasure::ExternalDistanceMeasureCalculator::normalize(int differenceCount, int sequencesize) const
	{
		return 0.0f;
	}
	float distanceMeasure::ExternalDistanceMeasureCalculator::calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const
	{
		return 0.0f;
	}
}