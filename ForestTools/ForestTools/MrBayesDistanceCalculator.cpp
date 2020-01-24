

/********************************************************************************************************
DeAngelo Wilson
January 3 2020

					MrBayesDistanceMeasureCalculator -- derived dmc -- njp
********************************************************************************************************/


#include "MrBayesDistanceCalculator.h"

namespace distanceMeasure
{
	//main driver
	void distanceMeasure::MrBayesDistanceCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id)
	{
		//align seqeunces...
			//create fasta file on sequence_set
		//CalculatorAligner::CreateAligned
			//where to store aligned seqeunces.... (need orignal FileObjects for lifetime...)

		//create NEXUS FILE from .afa (aligned) file

		//system call to extra-tools\\MrBayes...
		
		//InternalDistanceMeasureCalculator::calculate_and_output_matrix(fileObjectManager, sequence_set_names, batch_id);
	}

}


