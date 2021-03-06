/********************************************************************************************************
DeAngelo Wilson
January 18 2020

								Tool- calculatorAligner
********************************************************************************************************/

#include "CalculatorAligner.h"

#include "SystemParameters.h"
//#include <functional>

//alignment timing includes
#include "AlignedDistanceMeasureCalculator.h"

const std::string distanceMeasure::CalculatorAligner::create_sequence_set_aligned_file(AlignedDistanceMeasureCalculator* dmc, FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int total_sequence_count, const int hash_id) const
{
	std::string path;
	char aligned_file_path[150];
	//sprintf_s(aligned_file_path, SystemParameters::GetAlignedFileFormatString().c_str(), sequence_set_names.size(), hash_id);
	SystemParameters::GetAlignedFileString(aligned_file_path, total_sequence_count, sequence_set_names.size(), hash_id);

	//NOTE:: could re-enable optimization --> Must clear TempFiles on each FileSet BatchRun (in DMO)
	FILE* aligned_file = fopen(aligned_file_path, "rb");
	//NOTE:: ASSUMPTION:: NO incomplete .afa files (if .afa exists -- is correct)
	if(!aligned_file)
	{
		//TIME ALIGNMENT
		//double startTime = clock();
		std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

		const bool alignment_flag = dmc->GetCalculatorFlags()->align_sequences;
		//use orignal Fileobjects to create fasta file, on sequence_set_names
		const std::string fasta_file_path = CalculatorFastaFormatter::create_sequence_set_fasta_file_for_aligner(alignment_flag, fileObjectManager, sequence_set_names, hash_id);

		//If sequences DO NOT need to be aligned ==> must be equal sizes
		if(!alignment_flag)
		{
			path = fasta_file_path;
		}
		else
		{
			char alignment_command[200];
				//annotate .afa file -- 1st line w/ sequence set? check line 1: sequence set if file exists???
				//include hash (of sequence_set) in temp filename??? ****** NOT IMPLEMENTED
					//all different sequence sets unique...need to file_cleanup
						//system("exec rm -r /tmp/*") -- removes all files from folder temp
			SystemParameters::GetAlignmentCommandString(alignment_command, fasta_file_path.c_str(), aligned_file_path);
			//sprintf_s(alignment_command, SystemParameters::GetMuscleCommandString().c_str(), fasta_file_path.c_str(), aligned_file_path);
			//use muscle (3rd party) to align new_temp_FASTA file
			system(alignment_command);

			//stop time
				//write timings for sequence set to file	
				//add to total
			//const double alignmentTimeInMinutes = ((clock() - startTime) / CLOCKS_PER_SEC) / 60;
			const std::chrono::duration<double> alignmentTimeInSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - startTime);
			dmc->AddAlignmentTime(alignmentTimeInSeconds);

			path = aligned_file_path;
		}

	}
	else
	{
		path = aligned_file_path;
		//printf("Aligned file exists...\n");
		//close existing .afa file
		fclose(aligned_file);
	}
	//return new .afa (aligned FASTA sequence file) filename
	return path;
}
