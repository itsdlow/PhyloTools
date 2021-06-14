#include "NullDistanceMeasureCalculator.h"


distanceMeasure::NullDistanceMeasureCalculator::NullDistanceMeasureCalculator(RunFlags* flags, const std::string& name):
DistanceMeasureCalculator(flags, name)
{
}

void distanceMeasure::NullDistanceMeasureCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)
{
	//do nothing
}

void distanceMeasure::NullDistanceMeasureCalculator::write_quartet_matrix(FileObjectManager& fileObjectManager, const std::vector<int>& speciesSequenceSetIndexes, const std::vector<std::string>& sequence_set_names, const int fileCount, const int quartet_count)
{
	//do nothing
}
