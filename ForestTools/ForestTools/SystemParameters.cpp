/******************************************************************************
DeAngelo Wilson
February 15 2020

						System Parameters (singleton)
******************************************************************************/

#include "SystemParameters.h"

#include "DistanceMeasureCalculator.h"
#include "CalculatorFactory.h"


SystemParameters* SystemParameters::pInstance = nullptr;

void SystemParameters::Initialize()
{
	//set current OS
	SystemParameters::InitializeSystemDependentCommands();
	//Set strategy OsParameters...
		//give working directory --> to create ForestFiles at...
}

void SystemParameters::InitializeSystemDependentCommands()
{
	//#ifdef _WIN32 -- not defined on POSIX systems... (Cygwin, mingw32)
	#ifdef __linux__
		SystemParameters::Instance().OS_WINDOWS = true;
		printf("linux...\n");
	#endif

	//INITIALIZE UNIX VERSUS WINDOWS COMMANDS
	if (SystemParameters::Instance().OS_WINDOWS)
	{
		//7zip
		SystemParameters::Instance().zip7_command_string = "extra_tools\\7-Zip\\7z.exe a %s %s > nul";//'> nul' ignores system() output
		//MFC
		SystemParameters::Instance().mfc1_command_string = "extra_tools\\MFCompress-win64-1.01\\MFCompress-win64-1.01\\MFCompressC64.exe -1 -o %s %s";
		SystemParameters::Instance().mfc2_command_string = "extra_tools\\MFCompress-win64-1.01\\MFCompress-win64-1.01\\MFCompressC64.exe -2 -o %s %s";
		SystemParameters::Instance().mfc3_command_string = "extra_tools\\MFCompress-win64-1.01\\MFCompress-win64-1.01\\MFCompressC64.exe -3 -o %s %s";
		//MrBayes
		SystemParameters::Instance().mrbayes_command_string = "extra_tools\\MrBayes-3.2.7-WIN\\bin\\mb.3.2.7-win64.exe %s";
		//Muscle -- sequence alignment
		SystemParameters::Instance().muscle_command_string = "extra_tools\\muscle.exe -in %s -out %s";
		//fastME
		SystemParameters::Instance().fastme_command_string = "extra_tools\\fastme-2.1.5\\binaries\\fastme.exe -i %s -D %d -o %s";
		//phylotools --
		SystemParameters::Instance().clean_newick_regex = ":[0-9]+\\.?[0-9]+[e\\-+]*[0-9]*";
		SystemParameters::Instance().clean_dir_format_string = "del \"%s\\*\"";
		//
		SystemParameters::Instance().mrbayes_files_dir = "ForestFiles\\TempFiles\\MrBayes";

	}
	else
	{
		//7Zip
		SystemParameters::Instance().zip7_command_string = "./extra_tools/p7zip_16.02/bin/7z a %s %s > /dev/null";//'> nul' ignores system() output
		//MFC
		SystemParameters::Instance().mfc1_command_string = "./extra_tools/MFCompress-linux64-1.01/MFCompressC -1 -o %s %s";
		SystemParameters::Instance().mfc2_command_string = "./extra_tools/MFCompress-linux64-1.01/MFCompressC -2 -o %s %s";
		SystemParameters::Instance().mfc3_command_string = "./extra_tools/MFCompress-linux64-1.01/MFCompressC -3 -o %s %s";
		//MrBayes
		SystemParameters::Instance().mrbayes_command_string = "./extra_tools/MrBayes-3.2.7a/MrBayes-3.2.7a/src/mb %s";
		//Muscle -- sequence alignment
		SystemParameters::Instance().muscle_command_string = "./extra_tools/muscle -in %s -out %s";
		//fastME
		//SystemParameters::Instance().fastme_command_string = "./extra_tools/fastme-2.1.5/binaries/fastme-2.1.5-linux64 -i %s -D %d -o %s";
		SystemParameters::Instance().fastme_command_string = "./extra_tools/fastme-2.1.5/src/fastme -i %s -D %d -o %s";
		//phylotools -- TODO:: CHECK IF PROPER REGEX FOR LINUX...
		SystemParameters::Instance().clean_newick_regex = ":[0-9]+\.?[0-9]+[e\-+]*[0-9]*";
		SystemParameters::Instance().clean_dir_format_string = "rm -v %s/*";

		
		// directories... use os_slash variable to create rather than seperate windows/unix commands...
		SystemParameters::Instance().mrbayes_files_dir = "ForestFiles/TempFiles/MrBayes";
	}

	//INITIALIZE CALCULATOR FACTORY
	SystemParameters::InitializeCalculatorFactory();
}

void SystemParameters::InitializeCalculatorFactory()
{
	distanceMeasure::CalculatorFactory::Initialize();
}

void SystemParameters::Terminate()
{
	distanceMeasure::CalculatorFactory::Terminate();
	delete SystemParameters::pInstance;
}

void SystemParameters::InitializeSequenceSetParameters(int sequence_count, float sequenceListsSizeFractionLarge, float sequenceListsSizeFractionSmall, float sequenceListsCountFractionLarge, float sequenceListsCountFractionSmall)
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
OS_WINDOWS(true),
max_sequence_list_size(0),
subset_size_small(0),
subset_size_large(0),
subset_count_large(0),
subset_count_small(0),
subset_count_fraction_large(0),
subset_count_fraction_small(0)
{
	
}

int SystemParameters::privGetCalculatorTypeCount()
{
	//get count from calc factory
	return distanceMeasure::CalculatorFactory::GetCalculatorTypeCount();
}
unsigned int SystemParameters::GetCalculatorMask(int id)
{
	return distanceMeasure::CalculatorFactory::GetCalculatorBitmask(id);
}
unsigned int SystemParameters::GetAllCalculatorsMask()
{
	return distanceMeasure::CalculatorFactory::GetAllCalculatorsBitmask();
}


void SystemParameters::GetSequenceSetAlignmentTimingString(char* buffer, int batch_id, double calculation_time, const char* sequence_set_str)
{
	sprintf(buffer, SystemParameters::Instance().privGetSequenceSetAlignmentTimingFormatString().c_str(), batch_id, calculation_time, sequence_set_str);
}
void SystemParameters::GetAlignmentTimingsLogFileString(char* buffer, const char* calculator_name, int total_sequence_count)
{
	sprintf(buffer, SystemParameters::Instance().privGetAlignmentTimingsLogFileFormatString().c_str(), calculator_name, total_sequence_count);
}
void SystemParameters::GetSequenceSetTimingString(char* buffer, int batch_id, double calculation_time, const char* sequence_set_str)
{
	sprintf(buffer, SystemParameters::Instance().privGetSequenceSetTimingFormatString().c_str(), batch_id, calculation_time, sequence_set_str);
}
void SystemParameters::GetTimingsLogFileString(char* buffer, const char* calculator_name, int total_sequence_count)
{
	sprintf(buffer, SystemParameters::Instance().privGetTimingsLogFileFormatString().c_str(), calculator_name, total_sequence_count);
}

void SystemParameters::GetFastaFileString(char* buffer, const size_t sequence_count, const int hash_id)
{
	sprintf(buffer, SystemParameters::Instance().privGetFastaFileFormatString().c_str(), sequence_count, hash_id);
}

void SystemParameters::GetAlignedFileString(char* buffer, const int total_sequence_count, const size_t subsequence_count, const int hash_id)
{
	sprintf(buffer, SystemParameters::Instance().privGetAlignedFileFormatString().c_str(), total_sequence_count, subsequence_count, hash_id);
}
void SystemParameters::GetAlignmentCommandString(char* buffer, const char* in, const char* out)
{
	sprintf(buffer, SystemParameters::Instance().privGetMuscleCommandFormatString().c_str(), in, out);
}

void SystemParameters::GetNexusHeaderString(char* buffer, const size_t sequence_count, const int NCHAR)
{
	sprintf(buffer, SystemParameters::Instance().privGetNexusHeaderFormatString().c_str(), sequence_count, NCHAR, 
		SystemParameters::GetNexusDataTypeString().c_str(), SystemParameters::Instance().privGetNexusGapChar(), SystemParameters::Instance().privGetNexusMissingChar());

}
void SystemParameters::GetNexusFileString(char* buffer, const size_t sequence_count)
{
	sprintf(buffer, SystemParameters::Instance().privGetNexusFileFormatString().c_str(), sequence_count);
}

void SystemParameters::GetMrBayesCommand(char* buffer, const char* batch_block_file_path)
{
	sprintf(buffer, SystemParameters::Instance().privGetMrBayesCommandString().c_str(), batch_block_file_path);

}

void SystemParameters::GetAnalysisTableFilePath(char* buffer, const int sequence_count)
{
	sprintf(buffer, SystemParameters::Instance().privGetAnalysisTableFileFormatString().c_str(), sequence_count);
}

void SystemParameters::GetCompressedFastaFileString(char* buffer, const int file_count)
{
	sprintf(buffer, SystemParameters::Instance().privGetCompressedFastaFileFormatString().c_str(), file_count);
}
void SystemParameters::GetCompressedFilename(char* buffer, const char* extension)
{
	sprintf(buffer, SystemParameters::Instance().privGetCompressedFilenameFormat().c_str(), extension);
}
void SystemParameters::GetCompressionCommand(char* buffer, const char* derived_command, const char* out_file, const char* in_file)
{
	sprintf(buffer, derived_command, out_file, in_file);
}