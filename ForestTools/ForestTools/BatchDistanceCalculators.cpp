/******************************************************************************
DeAngelo Wilson
January 18 2020

						IntneralDistanceMEasureCalculator (derived-base)
******************************************************************************/

#include "BatchDistanceCalculators.h"

#include "BatchCalculatorsAnalyzer.h"

//#include "SystemParameters.h"
#include "CalculatorFactory.h"
#include "AnalyzerFactory.h"

namespace distanceMeasure
{

	distanceMeasure::BatchDistanceCalculators::BatchDistanceCalculators(RunFlags* flags, const std::string& name):
	DistanceMeasureCalculator(flags, name),
	calculator_count(flags->calculator_count),
	calculators(new DistanceMeasureCalculator* [this->calculator_count]),
	//poBatchAnalyzer(new BatchCalculatorsAnalyzer(this->calculators, flags->calculator_count, flags->generate_analysis, flags->generate_quartets))
	poBatchAnalyzer(phylo::AnalyzerFactory::Create(this->calculators, flags->calculator_count, flags->generate_analysis, flags->generate_quartets))
	{
		//NOTE:: ^^ FIRST CALL TO AnalyzerFactory --> may need to initialize...
			//analyzer factory not needed....
		phylo::AnalyzerFactory::Terminate();
		
		//intiialzie calculators array -- WITH ALL -- BIT_SET -- CALCULATOR METHODS
		int index = 0;
		int count = 0;
		unsigned int bitmask = flags->calculators_bitmask;
		while(bitmask != 0 && this->calculator_count > index)
		{
			const unsigned int bit = bitmask & 1;
			if (bit == 1)
			{
				this->calculators[index] = CalculatorFactory::Create(count, flags);
				index++;
			}
			//else //zero
			//{
				//...ignore
			//}
			bitmask >>= 1;
			
			count++;
		}
	}
	
	//called for each sequence set...
	//Driving function -- of Batch Distance Measure Calculator
	void BatchDistanceCalculators::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)
	{
		this->StartCalculationTimer();
		for (int i = 0; i < this->calculator_count; i++)
		{
			this->calculators[i]->calculate_and_output_matrix(fileObjectManager, sequence_set_names, sequence_set, batch_id);
		}
		this->StopCalculationTimer(batch_id, sequence_set);

		//COULD REMOVE 'if' BY CHANGING "BatchCalculatorAnalyzer" -> pImpl strategy pattern (does nothing if set to no-analysis strategy)
		//NOTE:: CHANGE ANALYZER --> allow ONLY non-quartet analysis to occur when quartets not generated
		if(this->GetCalculatorFlags()->generate_analysis)
		{
			this->poBatchAnalyzer->batch_analyze_sequence_set(sequence_set_names, batch_id, this->GetCalculatorFlags()->generate_quartets); 
		}
	}



	
	void BatchDistanceCalculators::InitializeSequenceSetTimingsLog(const int total_sequence_count)
	{
		DistanceMeasureCalculator::InitializeSequenceSetTimingsLog(total_sequence_count);
		//COMPOSITE... initialize all calc timingsLogFILE
		for (int i = 0; i < this->calculator_count; i++)
		{
			//this->calculators[i]->InitializeSequenceSetTimingsLog();
			this->calculators[i]->InitializeSequenceSetTimingsLog(total_sequence_count);
			//totalCalculationTime += this->calculators[i]->LogTotalCalculationTime();
		}
	}
	
	//CALLED BY: DistanceMatrixObject
	//composite pattern --> tell all derived calcs...
	void BatchDistanceCalculators::LogTotalCalculationTime()
	{
		//TODO:: must revise... top allow for unordered SequenceList.txt?
		//last sequence_set must be written to file + file close
		this->poBatchAnalyzer->WriteAnalysisTables();
		
		for (int i = 0; i < this->calculator_count; i++)
		{
			this->calculators[i]->LogTotalCalculationTime();
		}

		DistanceMeasureCalculator::LogTotalCalculationTime();
	}


	distanceMeasure::BatchDistanceCalculators::~BatchDistanceCalculators()
	{
		delete this->poBatchAnalyzer;
		
		for (int i = 0; i < this->calculator_count; i++)
		{
			delete this->calculators[i];
		}
	}

	//DistanceMeasureCalculator* distanceMeasure::BatchDistanceCalculators::GetCalculator(int i)
	//{
	//	DistanceMeasureCalculator* dmc = nullptr;

	//	switch (i)
	//	{
	//	case 0:
	//		dmc = new LcsDistanceCalculator(this->pFlags);
	//		break;
	//	case 1:
	//		dmc = new PValueDistanceCalculator(this->pFlags);
	//		break;
	//	case 2:
	//		dmc = new MrBayesDistanceCalculator(this->pFlags);
	//		break;
	//	case 3:
	//		//7zip
	//		dmc = new NcdDistanceCalculator(this->pFlags, 1);
	//		break;
	//	case 4:
	//		//mfc1
	//		dmc = new NcdDistanceCalculator(this->pFlags, 2);
	//		break;
	//	case 5:
	//		//mfc2
	//		dmc = new NcdDistanceCalculator(this->pFlags, 3);
	//		break;
	//	case 6:
	//		//mfc3
	//		dmc = new NcdDistanceCalculator(this->pFlags, 4);
	//		break;
	//	default:
	//		break;
	//	}
	//	return dmc;
	//}


	//internal calc specific function -- NOT NEEDED
	void distanceMeasure::BatchDistanceCalculators::write_quartet_matrix(FileObjectManager& fileObjectManager, const std::vector<int>& speciesSequenceSetIndexes, const std::vector<std::string>& sequence_set_names, const int fileCount, const int quartet_count)
	{
		//by default DMC does not have method to write quartet matrix -- derived Internal CAlculators define...
	}
	float distanceMeasure::BatchDistanceCalculators::normalize(int differenceCount, int sequencesize) const
	{
		return 0.0f;
	}
	float distanceMeasure::BatchDistanceCalculators::calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const
	{
		return 0.0f;
	}
}

