

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
		
		//create NEXUS FILE from .afa (aligned) sequence file
		const std::string nexus_file_path = CalculatorNexusFormatter::create_sequence_set_nexus_file(fileObjectManager, sequence_set_names);
		char mrbayes_file_path[80];
		//WINDOWS DEPENDENCE
		sprintf_s(mrbayes_file_path, "ForestFiles/TempFiles/temp_%zu.afa", sequence_set_names.size());

		//TODO -- move format string to systemsParameters***
		//WiNDOWS DEPENDENCE 
			//UNIX MRBayes COmmand -> input redirection
			//mb < batch.txt > log.txt & <-- (run task in background - do not wait)
		char mrbayes_command[200];
		char mrbayes_block[300];

		//TODO::NEED TO ADJUST NEXUS_FILE_PATH -- relative to mrbayes/bin/exe
		//get relative path for nexus file (add -- "../../../ForestFiles...")
		std::string relative_nexus_file_path = ("../../../");//WINDOWS DEPENDENCE
		relative_nexus_file_path.append(nexus_file_path);
		//need to include all mrbayes settings in command line
			//create batch MRBAYES BLOCK file
		std::string mrbayes_block_file_path = create_mrbayes_default_command_block_file(relative_nexus_file_path);

		//system call to extra-tools\\MrBayes... on MRBAYES command block file
		sprintf_s(mrbayes_command, "extra_tools\\MrBayes-3.2.7-WIN\\bin\\mb.3.2.7-win64.exe %s", mrbayes_block_file_path.c_str());
		//write mrbayes command to MrbayesCommand.txt (overwrite with each sequenceSet)
		//system(mrbayes_command);
	}

	std::string distanceMeasure::MrBayesDistanceCalculator::create_mrbayes_default_command_block_file(const std::string& relative_nxs_path)
	{
		return std::string();
	}

}


