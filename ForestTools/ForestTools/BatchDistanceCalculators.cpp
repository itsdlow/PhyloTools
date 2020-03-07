/******************************************************************************
DeAngelo Wilson
January 18 2020

						IntneralDistanceMEasureCalculator (derived-base)
******************************************************************************/

#include "BatchDistanceCalculators.h"

#include "MrBayesDistanceCalculator.h"
#include "PValueDistanceCalculator.h"
#include "LcsDistanceCalculator.h"
#include "NcdDistanceCalculator.h"

//#include "PhyloAnalysis.h"

namespace distanceMeasure
{

	distanceMeasure::BatchDistanceCalculators::BatchDistanceCalculators(const int calculator_count):
	BatchCalculatorsAnalyzer(calculator_count),
	calculator_count(calculator_count),
	//calculators(new DistanceMeasureCalculator* [calculator_count])
	calculators(nullptr)
	{
		//give Calculator analyzer ref to calculators
		//NOT IMPLEMENTED::
		//take as input parameter --> calc_count + bit_mask?
			//defines what calculator methods to include in batch_calculation
			//
		calculators = new DistanceMeasureCalculator* [this->calculator_count];
		//pass reference of calculators to batchAnalyzer... used to get specific calc_names + tree file-names
		BatchCalculatorsAnalyzer::SetCalculatorsArray(calculators);

		//intiialzie calculators array -- WITH ALL CALCULATOR METHODS
		for(int i = 0; i < this->calculator_count; i++)
		{
			this->calculators[i] = GetCalculator(i);
		}
		//this->calculators[0] = new PValueDistanceCalculator();
		//this->calculators[1] = new LcsDistanceCalculator();
	}

	std::string distanceMeasure::BatchDistanceCalculators::GetCalculatorName() const
	{
		return "Batch";
	}
	
	void BatchDistanceCalculators::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)
	{
		this->StartCalculationTimer();
		for (int i = 0; i < this->calculator_count; i++)
		{
			//this->calculators[i]->InitializeSequenceSetTimingsLog();
			this->calculators[i]->calculate_and_output_matrix(fileObjectManager, sequence_set_names, sequence_set, batch_id);
			//totalCalculationTime += this->calculators[i]->LogTotalCalculationTime();
		}
		this->StopCalculationTimer(batch_id, sequence_set);

		BatchCalculatorsAnalyzer::batch_analyze_sequence_set(sequence_set_names, batch_id);
	}

	void BatchDistanceCalculators::InitializeSequenceSetTimingsLog()
	{
		DistanceMeasureCalculator::InitializeSequenceSetTimingsLog();
		//COMPOSITE... initialize all calc timingsLogFILE
		for (int i = 0; i < this->calculator_count; i++)
		{
			//this->calculators[i]->InitializeSequenceSetTimingsLog();
			this->calculators[i]->InitializeSequenceSetTimingsLog();
			//totalCalculationTime += this->calculators[i]->LogTotalCalculationTime();
		}
	}
	//composite pattern --> tell all derived calcs...
	void BatchDistanceCalculators::LogTotalCalculationTime()
	{
		//TODO:: must revise... top allow for unordered SequenceList.txt?
		//last sequence_set must be written to file + file close
		BatchCalculatorsAnalyzer::WriteAnalysisTables();
		
		for (int i = 0; i < this->calculator_count; i++)
		{
			this->calculators[i]->LogTotalCalculationTime();
		}

		DistanceMeasureCalculator::LogTotalCalculationTime();
	}


	distanceMeasure::BatchDistanceCalculators::~BatchDistanceCalculators()
	{
		for (int i = 0; i < this->calculator_count; i++)
		{
			delete this->calculators[i];
		}
	}

	DistanceMeasureCalculator* distanceMeasure::BatchDistanceCalculators::GetCalculator(int i)
	{
		DistanceMeasureCalculator* dmc = nullptr;

		switch (i)
		{
		case 0:
			dmc = new LcsDistanceCalculator();
			break;
		case 1:
			dmc = new PValueDistanceCalculator();
			break;
		case 2:
			dmc = new MrBayesDistanceCalculator();
			break;
		case 3:
			dmc = new NcdDistanceCalculator(1);
			break;
		case 4:
			dmc = new NcdDistanceCalculator(3);
			break;
		default:
			break;
		}
		return dmc;
	}
}

