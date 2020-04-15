/******************************************************************************
DeAngelo Wilson
March 27 2020

						Aligned Distance Measure Calculator
******************************************************************************/
#include "AlignedDistanceMeasureCalculator.h"

#include <ctime>
#include "SystemParameters.h"


/***************************************************************
 *						Timing functions
 ***************************************************************/

//times calculation for - 1 - SEQUENCE SET
void distanceMeasure::AlignedDistanceMeasureCalculator::StartCalculationTimer()
{
	DistanceMeasureCalculator::StartCalculationTimer();
	
	//resets...
	this->ResetAlignmentTime();
}
void distanceMeasure::AlignedDistanceMeasureCalculator::StopCalculationTimer(int batchID, const std::string& sequenceSet)
{
	//get calculation time in minutes
	const double totalSequenceSetCalculationTime = ((clock() - startTime) / CLOCKS_PER_SEC) / 60;
	this->calculationTime = totalSequenceSetCalculationTime - this->alignmentOffsetTime;
	this->totalCalculationTime += this->calculationTime;
	this->LogSequenceSetTiming(batchID, this->calculationTime, sequenceSet);

	this->totalAlignmentTime += this->alignmentOffsetTime;
	this->LogSequenceSetAlignmentTiming(batchID, this->alignmentOffsetTime, sequenceSet);
}
void distanceMeasure::AlignedDistanceMeasureCalculator::LogSequenceSetAlignmentTiming(int batchID, double calculationTime, const std::string& sequenceSet) const
{
	//if alignment took place -- write sequence set alignment timings
	if(calculationTime > 0)
	{
		//write to log file
		if (this->pAlignmentTimingsLogFile)
		{
			//NOTE:: Probably should (somehow) dynamically determine size
			char time_log_line[1000];
			//WINDOWS DEPENDENCE -- extract to system parameters...
			sprintf(time_log_line, SystemParameters::GetSequenceSetAlignmentTimingFormatString().c_str(), batchID, calculationTime, sequenceSet.c_str());
			const std::string timingLine(time_log_line);

			size_t numBytesWritten = fwrite(timingLine.c_str(), timingLine.length(), 1, this->pAlignmentTimingsLogFile);
			fflush(this->pAlignmentTimingsLogFile);
		}
	}

}

void distanceMeasure::AlignedDistanceMeasureCalculator::InitializeSequenceSetTimingsLog()
{
	DistanceMeasureCalculator::InitializeSequenceSetTimingsLog();
	char alignment_log_file_path[100];
	//WINDOWS DEPENDENCE
	//sprintf_s(alignment_log_file_path, SystemParameters::GetAlignmentTimingsLogFileFormatString().c_str(), this->GetCalculatorName().c_str());
	sprintf(alignment_log_file_path, SystemParameters::GetAlignmentTimingsLogFileFormatString().c_str(), this->GetCalculatorName().c_str());

	//open file
	//fopen_s(&this->pAlignmentTimingsLogFile, alignment_log_file_path, "w");
	this->pAlignmentTimingsLogFile = fopen(alignment_log_file_path, "w");

}
void distanceMeasure::AlignedDistanceMeasureCalculator::LogTotalCalculationTime()
{
	//Alert distanceMeasureFunc --> done processes sequence Sets
	//Log (write) total calcaulation time + close file
	//printf("\nCalculation Time For Sequence Set Lists: %f seconds", this->totalCalculationTime);
	//write to log file
	DistanceMeasureCalculator::LogTotalCalculationTime();
	if (this->pAlignmentTimingsLogFile)
	{
		char time_log_line[100];
		//WINDOWS DEPENDENCE -- extract to system parameters...
		sprintf(time_log_line, "\nAlignment Time For Sequence Set Lists: %f minutes\n", this->totalAlignmentTime);
		std::string timingLine(time_log_line);

		size_t numBytesWritten = fwrite(timingLine.c_str(), timingLine.length(), 1, this->pAlignmentTimingsLogFile);
		fclose(this->pAlignmentTimingsLogFile);
	}
}