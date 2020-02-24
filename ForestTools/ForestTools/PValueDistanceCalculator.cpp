/********************************************************************************************************
DeAngelo Wilson
January 3 2020

					PValueDistanceMeasureCalculator -- derived dmc -- njp
********************************************************************************************************/


#include "PValueDistanceCalculator.h"
#include "FileObject.h"

namespace distanceMeasure
{
	////main driver
	//void distanceMeasure::PValueDistanceCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string> sequence_set_names, const int batch_id)
	//{
	//	//align seqeunces...
	//		//create fasta file on sequence_set
	//	//CalculatorAligner::CreateAligned
	//		//where to store aligned seqeunces.... (need orignal FileObjects for lifetime...)


	//	
	//	//pass FOM forward...
	//	InternalDistanceMeasureCalculator::calculate_and_output_matrix(fileObjectManager, sequence_set_names, batch_id);
	//}
	//driving method
	void distanceMeasure::PValueDistanceCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)
	{
		this->StartCalculationTimer();

		Aligned_InternalDMCalculator::calculate_and_output_matrix(fileObjectManager, sequence_set_names, sequence_set, batch_id);

		this->StopCalculationTimer(batch_id, sequence_set);
	}
	
	std::string distanceMeasure::PValueDistanceCalculator::GetCalculatorName() const
	{
		return "PValue";
	}
	
	float PValueDistanceCalculator::calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const
	{
		//assumes aligned sequences
		const int m = file1.GetSequenceSize();
		const int n = file2.GetSequenceSize();
		if (m != n)
		{
			return 0;
		}

		int differenceCount = 0;
		//loop thru sequences comparing relative indexes
		for (int i = 0; i < m; i++)
		{
			//compare sequences[i] --
			if (file1.GetSequenceCharAt(i) != file2.GetSequenceCharAt(i))
			{
				differenceCount++;
			}
		}
		//printf("simCount = %d\n", similarityCount);
		//Measure::
			//normalize(count) == 1 --> MAXIMALLY DIFFERENT sequences
			//normalize(count) == 0 --> IDENTICAL SEQUENCES
		return this->normalize(differenceCount, m);
	}

	float PValueDistanceCalculator::normalize(int differenceCount, int sequencesize) const
	{
		return static_cast<float>(differenceCount) / sequencesize;
	}
}
