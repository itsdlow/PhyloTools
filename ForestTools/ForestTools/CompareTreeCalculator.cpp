#include "CompareTreeCalculator.h"

#include "SystemParameters.h"


distanceMeasure::CompareTreeCalculator::CompareTreeCalculator(RunFlags* flags, const std::string& name, const std::string& tree_filename)
	:NullDistanceMeasureCalculator(flags, name),
filename(tree_filename)
{
}

void distanceMeasure::CompareTreeCalculator::GetLargeListTreeFileName(char* buffer, const int batch_number, const size_t sequence_count) const
{
	//this->pFlags.
	//sprintf(buffer, SystemParameters::GetLargeListTreeFileFormatString().c_str(), this->GetCalculatorName().c_str(), sequence_count, batch_number, SystemParameters::GetCurrentFileSetBatchNumber());
}
