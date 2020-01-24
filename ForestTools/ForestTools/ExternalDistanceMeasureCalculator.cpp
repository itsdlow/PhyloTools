/******************************************************************************
DeAngelo Wilson
January 18 2020

						IntneralDistanceMEasureCalculator (derived-base)
******************************************************************************/


#include "ExternalDistanceMeasureCalculator.h"

#include "FileObjectManager.h"

namespace distanceMeasure
{
	void ExternalDistanceMeasureCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id)
	{
		//if alignment needed
		//ALIGN -- refill_FileObjectManager... done by derived calcs
		//calculate LargeTree giving sequence_names_list
		//this->CalculateLargeTreeDistanceMeasures(fileObjectManager, sequence_set_names);
		//this->CalculateAllQuartetsDistanceMeasures(sequence_set_names);

		//////create file for current sequence_set
		//this->write_batch_results(batch_id, sequence_set_names.size());
	}
}