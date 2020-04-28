/******************************************************************************
DeAngelo Wilson
February 15 2020

						System Parameters (singleton)
******************************************************************************/

#include "SystemParameters.h"

#include "DistanceMeasureCalculator.h"

SystemParameters* SystemParameters::pInstance = nullptr;

void SystemParameters::InitializeSystemDependentCommands()
{
	#ifdef _WIN32
	SystemParameters::Instance().OS_WINDOWS = true;
	
	#endif

	//INITIALIZE UNIX VERSUS WINDOWS COMMANDS
	if (SystemParameters::Instance().OS_WINDOWS)
	{
		SystemParameters::Instance().zip7_command_string = "extra_tools\\7-Zip\\7z.exe a %s %s > nul";
	}
	else
	{
		SystemParameters::Instance().zip7_command_string = "7z a %s %s";//".extra_tools/7-Zip/7z a %s %s > /dev/null";
	}
}

void SystemParameters::Initialize(int sequence_count, float sequenceListsSizeFractionLarge, float sequenceListsSizeFractionSmall, float sequenceListsCountFractionLarge, float sequenceListsCountFractionSmall)
{
	SystemParameters::Instance().max_sequence_list_size = sequence_count;
	SystemParameters::Instance().subset_size_small = static_cast<int>(static_cast<float>(sequence_count) * sequenceListsSizeFractionSmall);
	SystemParameters::Instance().subset_size_large = static_cast<int>(static_cast<float>(sequence_count)* sequenceListsSizeFractionLarge);
	//subset count -- 10% of total combinations possible
	const int total_large_combos = distanceMeasure::DistanceMeasureCalculator::GetCombinations(sequence_count, SystemParameters::Instance().subset_size_large);
	const int total_small_combos = distanceMeasure::DistanceMeasureCalculator::GetCombinations(sequence_count, SystemParameters::Instance().subset_size_small);
	SystemParameters::Instance().subset_count_large = static_cast<int>(static_cast<float>(total_large_combos) * sequenceListsCountFractionLarge);
	SystemParameters::Instance().subset_count_small = static_cast<int>(static_cast<float>(total_small_combos) * sequenceListsCountFractionSmall);
	SystemParameters::Instance().subset_count_fraction_large = sequenceListsCountFractionLarge;
	SystemParameters::Instance().subset_count_fraction_small = sequenceListsCountFractionSmall;
}

SystemParameters::SystemParameters():
OS_WINDOWS(false),
max_sequence_list_size(0),
subset_size_small(0),
subset_size_large(0),
subset_count_large(0),
subset_count_small(0),
subset_count_fraction_large(0),
subset_count_fraction_small(0)
{
}
