/********************************************************************************************************
DeAngelo Wilson
January 18 2020

								Tool- calculatorAligner
********************************************************************************************************/

#include "CalculatorAligner.h"

#include "SystemParameters.h"
#include <functional>

//alignment timing includes
#include <ctime>
#include "AlignedDistanceMeasureCalculator.h"

const std::string distanceMeasure::CalculatorAligner::create_sequence_set_aligned_file(AlignedDistanceMeasureCalculator* dmc, FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int hash_id) const
{
	char aligned_file_path[150];
	//WINDOWS DEPENDENCE
	//sprintf_s(aligned_file_path, SystemParameters::GetAlignedFileFormatString().c_str(), sequence_set_names.size(), hash_id);
	sprintf(aligned_file_path, SystemParameters::GetAlignedFileFormatString().c_str(), sequence_set_names.size(), hash_id);

	//TODO::...check if .afa file already created + size != 0
	FILE* aligned_file = fopen(aligned_file_path, "rb");
	//NOTE:: ASSUMPTION:: NO incomplete .afa files (if .afa exists -- is correct)
	if(!aligned_file)
	{
		//TIME ALIGNMENT
		double startTime = clock();
		
		//use orignal Fileobjects to create fasta file, on sequence_set_names
		const std::string fasta_file_path = CalculatorFastaFormatter::create_sequence_set_fasta_file(fileObjectManager, sequence_set_names, hash_id);

		char alignment_command[200];
			//annotate .afa file -- 1st line w/ sequence set? check line 1: sequence set if file exists???
			//include hash (of sequence_set) in temp filename??? ****** NOT IMPLEMENTED
				//all different sequence sets unique...need to file_cleanup
					//system("exec rm -r /tmp/*") -- removes all files from folder temp
		sprintf(alignment_command, SystemParameters::GetMuscleCommandString().c_str(), fasta_file_path.c_str(), aligned_file_path);
		//use muscle (3rd party) to align new_temp_FASTA file
		system(alignment_command);

		//stop time
			//write timings for sequence set to file	
			//add to total
		const double alignmentTimeInMinutes = ((clock() - startTime) / CLOCKS_PER_SEC) / 60;
		dmc->AddAlignmentTime (alignmentTimeInMinutes);
	}
	
	//return new .afa (aligned FASTA sequence file) filename
	return std::string(aligned_file_path);
}
