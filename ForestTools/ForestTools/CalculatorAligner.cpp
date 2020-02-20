/********************************************************************************************************
DeAngelo Wilson
January 18 2020

								Tool- calculatorAligner
********************************************************************************************************/

#include "CalculatorAligner.h"

#include "SystemParameters.h"

const std::string distanceMeasure::CalculatorAligner::create_sequence_set_aligned_file(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names)
{
	//use orignal Fileobjects to create fasta file, on sequence_set_names
	const std::string fasta_file_path = CalculatorFastaFormatter::create_sequence_set_fasta_file(fileObjectManager, sequence_set_names);

	char alignment_command[200];
	char aligned_file_path[80];
	//WINDOWS DEPENDENCE
	sprintf_s(aligned_file_path, SystemParameters::GetAlignedFileFormatString().c_str(), sequence_set_names.size());

	//TODO::...check if .afa file already created???
		//annotate .afa file -- 1st line w/ sequence set? check line 1: sequence set if file exists???
		//include hash (of sequence_set) in temp filename??? ****** NOT IMPLEMENTED
			//all different sequence sets unique...need to file_cleanup
				//system("exec rm -r /tmp/*") -- removes all files from folder temp
	sprintf_s(alignment_command, SystemParameters::GetMuscleCommandString().c_str(), fasta_file_path.c_str(), aligned_file_path);
	//use muscle (3rd party) to align new_temp_FASTA file
	system(alignment_command);

	
	//return new .afa (aligned FASTA sequence file) filename
	return std::string(aligned_file_path);
}