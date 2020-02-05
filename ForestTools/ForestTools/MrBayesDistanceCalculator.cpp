

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
		//CalculatorFastaFormatter::create_sequence_set_fasta_file(fileObjectManager, sequence_set_names);//given thread id for temp_threadID file anming convention
		//CalculatorAligner::CreateAligned
			//where to store aligned seqeunces.... (need orignal FileObjects for lifetime...)
			//
			//*****************************
		//const std::string aligned_filename = CalculatorAligner::create_sequence_set_aligned_file(fileObjectManager, sequence_set_names);
		
		//create NEXUS FILE from .afa (aligned) file
		CalculatorNexusFormatter::create_sequence_set_nexus_file(fileObjectManager, sequence_set_names);
		
			//TODO -- move format string to systemsParameters
	//WiNDOWS DEPENDENCE
		//	char mrbayes_command[300];
		//need to include all mrbayes settings in command line
		//system call to extra-tools\\MrBayes...
		//	sprintf_s(mrbayes_command, "extra_tools\\MrBayes-3.2.7-WIN\\bin\\mb.3.2.7-win64.exe -in %s -out %s", aligned_file_path.c_str(), mrbayes_file_path);

		//InternalDistanceMeasureCalculator::calculate_and_output_matrix(fileObjectManager, sequence_set_names, batch_id);
	}

}


