/******************************************************************************
DeAngelo Wilson
March 27 2020

						Aligned Distance Measure Calculator
******************************************************************************/
#include "AlignedDistanceMeasureCalculator.h"

//#include <ctime>
#include "SystemParameters.h"
#include "DynamicSizedArray.h"

/***************************************************************
 *						Timing functions
 ***************************************************************/

distanceMeasure::AlignedDistanceMeasureCalculator::AlignedDistanceMeasureCalculator(RunFlags* flags, const std::string& name):
DistanceMeasureCalculator(flags, name),
alignmentOffsetTime(std::chrono::duration<double>::zero()),
totalAlignmentTime(std::chrono::duration<double>::zero())
{
}

//times calculation for - 1 - SEQUENCE SET
void distanceMeasure::AlignedDistanceMeasureCalculator::StartCalculationTimer()
{
	DistanceMeasureCalculator::StartCalculationTimer();
	
	//resets...
	this->ResetAlignmentTime();
}
//TODO:: refactor... timings w/ alignment && w/o alignment should be in same file....
void distanceMeasure::AlignedDistanceMeasureCalculator::StopCalculationTimer(int batchID, const std::string& sequenceSet)
{
	//get calculation time in minutes
	//const double totalSequenceSetCalculationTime = ((clock() - startTime) / CLOCKS_PER_SEC) / 60;
	const std::chrono::duration<double> totalSequenceSetCalculationTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - this->startTime);

	this->calculationTime = totalSequenceSetCalculationTime - this->alignmentOffsetTime;
	this->totalCalculationTime += this->calculationTime;
	this->LogSequenceSetTiming(batchID, this->calculationTime.count(), sequenceSet);

	this->totalAlignmentTime += this->alignmentOffsetTime;
	this->LogSequenceSetAlignmentTiming(batchID, this->alignmentOffsetTime.count(), sequenceSet);
}
void distanceMeasure::AlignedDistanceMeasureCalculator::LogSequenceSetAlignmentTiming(int batchID, double calculationTime, const std::string& sequenceSet) const
{
	//if alignment took place -- write sequence set alignment timings
	if(calculationTime > 0)
	{
		//write to log file
		if (this->pAlignmentTimingsLogFile)
		{
			const int format_param_size = 100;
			const int size = static_cast<int>(sequenceSet.size()) + format_param_size;
			const DynamicSizedArray time_log_line(size);
			// TODO: BUFFER OVERRUN INVALIDATING DMC...
			//NOTE:: Probably should (somehow) dynamically determine size
			//char time_log_line[1000];
			SystemParameters::GetSequenceSetAlignmentTimingString(time_log_line.array, batchID, calculationTime / 60, sequenceSet.c_str());
			const std::string timingLine(time_log_line.array);

			size_t numBytesWritten = fwrite(timingLine.c_str(), timingLine.length(), 1, this->pAlignmentTimingsLogFile);
			fflush(this->pAlignmentTimingsLogFile);
		}
	}

}

void distanceMeasure::AlignedDistanceMeasureCalculator::InitializeSequenceSetTimingsLog(const int total_sequence_count)
{
	DistanceMeasureCalculator::InitializeSequenceSetTimingsLog(total_sequence_count);
	char alignment_log_file_path[500];
	//sprintf_s(alignment_log_file_path, SystemParameters::GetAlignmentTimingsLogFileFormatString().c_str(), this->GetCalculatorName().c_str(), total_sequence_count);
	SystemParameters::GetAlignmentTimingsLogFileString(alignment_log_file_path, this->GetCalculatorName().c_str(), total_sequence_count);
	//open file
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
		//ToDO::extract to system parameters...
		sprintf(time_log_line, "\nAlignment Time For Sequence Set Lists: %f minutes\n", this->totalAlignmentTime.count() / 60);
		std::string timingLine(time_log_line);

		size_t numBytesWritten = fwrite(timingLine.c_str(), timingLine.length(), 1, this->pAlignmentTimingsLogFile);
		fclose(this->pAlignmentTimingsLogFile);
	}
}