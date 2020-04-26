/******************************************************************************
DeAngelo Wilson
January 24 2020

                        Distance Measure Calculator
******************************************************************************/


#include "DistanceMeasureCalculator.h"

#include "SystemParameters.h"
#include <ctime>


////only implemented by BatchDistanceCalculators
//void distanceMeasure::DistanceMeasureCalculator::batch_analyze_sequence_set(const std::vector<std::string>& sequence_set_names, const int batch_id)
//{
//	//do nothing -- cannot analyze sequence_set on 1 method -- use BatchCalculators
//}

distanceMeasure::DistanceMeasureCalculator::DistanceMeasureCalculator(RunFlags* flags):
pFlags(flags)
{
}

distanceMeasure::DistanceMeasureCalculator::~DistanceMeasureCalculator()
{
	delete this->pFlags;
}

//Sequence_set_size (N) Choose (4) --> number of quartet matrices 
int distanceMeasure::DistanceMeasureCalculator::GetQuartetCombinations(int n)
{
	//size of quartet
	int k = 4;
	
	if (k > n) return 0;
	if (k * 2 > n) k = n - k;
	if (k == 0) return 1;

	int result = n;
	for (int i = 2; i <= k; ++i) {
	    result *= (n - i + 1);
	    result /= i;
	}
	return result;
}

//Sequence_set_size (N) Choose (4) --> number of quartet matrices 
int distanceMeasure::DistanceMeasureCalculator::GetCombinations(int n, int r)
{
	//size of quartet
	int k = r;

	if (k > n) return 0;
	if (k * 2 > n) k = n - k;
	if (k == 0) return 1;

	int result = n;
	for (int i = 2; i <= k; ++i) {
		result *= (n - i + 1);
		result /= i;
	}
	return result;
}

void distanceMeasure::DistanceMeasureCalculator::swap_space_with_underscores(std::string& description_string)
{
	//replace all spaces w/ "__underscores__"
	for (auto i = 0u; i < description_string.size(); i++)
	{
		if (isspace(description_string.at(i)))
		{
			description_string.replace(i, 1u, 1u, '_');
		}
	}//--> create_one_word_species_names
}


std::vector<std::string> distanceMeasure::DistanceMeasureCalculator::CreateSubsequenceSet(const std::vector<std::string>& sequence_set_names, const std::vector<int>& subSequenceSetIndexes)
{
	std::vector<std::string> subSequenceSet;
	//std::vector<int> subSequenceSetIndexes{ i,j,k,l };
	subSequenceSet.reserve(subSequenceSetIndexes.size());

	for (unsigned int index = 0u; index < subSequenceSetIndexes.size(); index++)
	{
		//for each given index (i,j,k,l) add name to result_set
		subSequenceSet.push_back(sequence_set_names.at(subSequenceSetIndexes.at(index)));
	}
	return subSequenceSet;
}

std::string distanceMeasure::DistanceMeasureCalculator::CreateSubsequenceSetString(const std::vector<std::string>& subsequence_set_names)
{
	std::string str(std::to_string(subsequence_set_names.size()).append("\t"));
	for(auto it = subsequence_set_names.begin(); it != subsequence_set_names.end(); it++)
	{
		std::string temp = std::string(*it);
		swap_space_with_underscores(temp);
		//append name to subsequence set string
		str.append(" ");
		str.append(temp);
	}
	str.append("\n");
	
	return str;
}
/*
 *
 */


//fills given buffer
void distanceMeasure::DistanceMeasureCalculator::GetClusteredMatrixFileName(char* buffer, const size_t buffer_size, const int batch_number, const size_t sequence_count) const
{

	sprintf_s(buffer, buffer_size, SystemParameters::GetClusteredMatrixFileFormatString().c_str(), this->GetCalculatorName().c_str(), sequence_count, batch_number);
}
void distanceMeasure::DistanceMeasureCalculator::GetClusteredTreeFileName(char* buffer, const size_t buffer_size, const int batch_number, const size_t sequence_count) const
{
	sprintf_s(buffer, buffer_size, SystemParameters::GetClusteredTreeFileFormatString().c_str(), this->GetCalculatorName().c_str(), sequence_count, batch_number);
}

void distanceMeasure::DistanceMeasureCalculator::GetLargeListMatrixFileName(char* buffer, const size_t buffer_size, const int batch_number, const size_t sequence_count) const
{
	sprintf_s(buffer, buffer_size, SystemParameters::GetLargeListMatrixFileFormatString().c_str(), this->GetCalculatorName().c_str(), sequence_count, batch_number);
}
void distanceMeasure::DistanceMeasureCalculator::GetLargeListTreeFileName(char* buffer, const size_t buffer_size, const int batch_number, const size_t sequence_count) const
{
	sprintf_s(buffer, buffer_size, SystemParameters::GetLargeListTreeFileFormatString().c_str(), this->GetCalculatorName().c_str(), sequence_count, batch_number);
}
void distanceMeasure::DistanceMeasureCalculator::GetQuartetsMatrixFileName(char* buffer, const size_t buffer_size, const int batch_number, const size_t sequence_count) const
{
	sprintf_s(buffer, buffer_size, SystemParameters::GetQuartetMatricesFileFormatString().c_str(), this->GetCalculatorName().c_str(), sequence_count, batch_number);

}
void distanceMeasure::DistanceMeasureCalculator::GetQuartetsTreeFileName(char* buffer, const size_t buffer_size, const int batch_number, const size_t sequence_count) const
{
	sprintf_s(buffer, buffer_size, SystemParameters::GetQuartetTreesFileFormatString().c_str(), this->GetCalculatorName().c_str(), sequence_count, batch_number);
}
void distanceMeasure::DistanceMeasureCalculator::GetFastMECommand(char* buffer, const size_t buffer_size, char* input, int count, char* output) const
{
	//"extra_tools\\fastme-2.1.5\\binaries\\fastme.exe -i %s -D %d -o %s"
	sprintf_s(buffer, buffer_size, SystemParameters::GetFastmeCommandString().c_str(), input, count, output);
}


/***************************************************************
 *						Timing functions
 ***************************************************************/

//times calculation for - 1 - SEQUENCE SET
void distanceMeasure::DistanceMeasureCalculator::StartCalculationTimer()
{
	this->startTime = clock();
}
void distanceMeasure::DistanceMeasureCalculator::StopCalculationTimer(int batchID, const std::string& sequenceSet)
{
	//get calculation time in minutes
	this->calculationTime = ((clock() - startTime) / CLOCKS_PER_SEC) / 60;
	//if calculator had to align -- include time it takes to align
	
	this->totalCalculationTime += this->calculationTime;

	this->LogSequenceSetTiming(batchID, this->calculationTime, sequenceSet);
}

void distanceMeasure::DistanceMeasureCalculator::LogSequenceSetTiming(int batchID, double calculationTime, const std::string& sequenceSet) const
{
	//write to log file
	if(this->pTimingsLogFile)
	{
		const int sequence_timing_format_string_size = 75;
		const int time_log_line_size = static_cast<int>(sequenceSet.size()) + sequence_timing_format_string_size;
		char* const time_log_line = new char[time_log_line_size];

		//WINDOWS DEPENDENCE -- extract to system parameters...
		sprintf_s(time_log_line, time_log_line_size, SystemParameters::GetSequenceSetTimingFormatString().c_str(), batchID, calculationTime, sequenceSet.c_str());
		const std::string timingLine(time_log_line);
		
		delete[] time_log_line;
		
		size_t numBytesWritten = fwrite(timingLine.c_str(), timingLine.length(), 1, this->pTimingsLogFile);
		fflush(this->pTimingsLogFile);
	}
}

void distanceMeasure::DistanceMeasureCalculator::InitializeSequenceSetTimingsLog()
{
	char log_file_path[100];
	sprintf_s(log_file_path, SystemParameters::GetTimingsLogFileFormatString().c_str(), this->GetCalculatorName().c_str());

	//open file
	fopen_s(&this->pTimingsLogFile, log_file_path, "w");
}
void distanceMeasure::DistanceMeasureCalculator::LogTotalCalculationTime()
{
	//Alert distanceMeasureFunc --> done processes sequence Sets
	//Log (write) total calcaulation time + close file
	//printf("\nCalculation Time For Sequence Set Lists: %f seconds", this->totalCalculationTime);
	//write to log file
	if (this->pTimingsLogFile)
	{
		char time_log_line[100];
		//WINDOWS DEPENDENCE -- extract to system parameters...
		sprintf_s(time_log_line, "\nCalculation Time For Sequence Set Lists: %f minutes\n", this->totalCalculationTime);
		std::string timingLine(time_log_line);

		size_t numBytesWritten = fwrite(timingLine.c_str(), timingLine.length(), 1, this->pTimingsLogFile);
		fclose(this->pTimingsLogFile);
	}
}