/********************************************************************************************************
DeAngelo Wilson
January 18 2020

								Tool- calculatorAligner
********************************************************************************************************/

#include "CalculatorAligner.h"

const std::string distanceMeasure::CalculatorAligner::create_sequence_set_aligned_file(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names)
{
	//use orignal Fileobjects to create fasta file, on sequence_set_names
	const std::string fasta_file_path = CalculatorFastaFormatter::create_sequence_set_fasta_file(fileObjectManager, sequence_set_names);

	char alignment_command[200];
	char aligned_file_path[80];
	sprintf_s(aligned_file_path, "ForestFiles/TempFiles/temp_%zu.afa", sequence_set_names.size());

	//WiNDOWS DEPENDENCE
	//TODO -- move format string to systemsParameters
	sprintf_s(alignment_command, "extra_tools\\muscle.exe -in %s -out %s", fasta_file_path.c_str(), aligned_file_path);
	//use muscle (3rd party) to align new_temp_FASTA file
	system(alignment_command);

	
	//return new .afa (aligned FASTA sequence file) filename
	return std::string(aligned_file_path);
}