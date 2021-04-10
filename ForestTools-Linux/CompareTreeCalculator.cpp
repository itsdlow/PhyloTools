#include "CompareTreeCalculator.h"

#include "SystemParameters.h"


distanceMeasure::CompareTreeCalculator::CompareTreeCalculator(RunFlags* flags, const std::string& name)
	:NullDistanceMeasureCalculator(flags, name)
{
}

void distanceMeasure::CompareTreeCalculator::GetLargeListTreeFileName(char* buffer, const int batch_number, const size_t sequence_count) const
{
	std::string path;
	SystemParameters::GetCurrentFileSetCompareTreePath(path);
	strcpy(buffer, path.c_str());
	
	//strcpy(buffer, this->pFlags->GetCurrentCompareTreeFile(SystemParameters::GetCurrentFileSetBatchNumber()).c_str());
	//sprintf(buffer, this->pFlags->GetCurrentCompareTreeFile(SystemParameters::GetCurrentFileSetBatchNumber()).c_str());
}
