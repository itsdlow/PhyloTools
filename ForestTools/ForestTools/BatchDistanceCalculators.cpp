/******************************************************************************
DeAngelo Wilson
January 18 2020

						IntneralDistanceMEasureCalculator (derived-base)
******************************************************************************/

#include "BatchDistanceCalculators.h"

#include "MrBayesDistanceCalculator.h"
#include "PValueDistanceCalculator.h"
#include "LcsDistanceCalculator.h"

namespace distanceMeasure
{

	distanceMeasure::BatchDistanceCalculators::BatchDistanceCalculators()
	{
		//intiialzie calculators array
		for(int i = 0; i < this->calculator_count; i++)
		{
			this->calculators[i] = GetCalculator(i);
		}
	}

	std::string distanceMeasure::BatchDistanceCalculators::GetCalculatorName() const
	{
		return "Batch";
	}
	
	void BatchDistanceCalculators::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id)
	{
		for (int i = 0; i < this->calculator_count; i++)
		{
			this->calculators[i]->calculate_and_output_matrix(fileObjectManager, sequence_set_names, batch_id);
		}
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
			//case 3:
			//	dmc = new NcdDistanceCalculator();
		default:
			break;
		}
		return dmc;
	}
}

