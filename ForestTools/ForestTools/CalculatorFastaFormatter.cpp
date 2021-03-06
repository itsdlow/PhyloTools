/********************************************************************************************************
DeAngelo Wilson
January 18 2020

								Tool- calculatorFastaFormatter
********************************************************************************************************/

#include "CalculatorFastaFormatter.h"

#include "FileObjectManager.h"
#include "FileObject.h"
#include "SystemParameters.h"

//TODO:: (also for aligner...) Prevent re-creation of temp_files by giving them HASH_ID --> compute hash based of sequence set!!!
const std::string distanceMeasure::CalculatorFastaFormatter::create_sequence_set_fasta_file(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int hash_id) const
{
	char fasta_filename[150];
	std::size_t sequence_count = sequence_set_names.size();
	//sprintf_s(fasta_filename, SystemParameters::GetFastaFileFormatString().c_str(), sequence_count, hash_id);
	SystemParameters::GetFastaFileString(fasta_filename, sequence_count, hash_id);

	//NOTE:: could re-enable optimization --> Must clear TempFiles on each FileSet BatchRun (in DMO)
	////try to open file
	FILE* existing_fasta_file = fopen(fasta_filename, "rb");
	//NOTE:: ASSUMPTION:: NO incomplete .fasta files (if .fasta exists -- is correct)
	if (!existing_fasta_file)
	{
		//create FASTA file string
		std::string sequence_set_fasta_string;
		std::size_t species_lines_length(180u);
		sequence_set_fasta_string.reserve(sequence_count * species_lines_length);

		//use orignal Fileobjects to create fasta file, on sequence_set_names
		for(auto it = sequence_set_names.begin(); it != sequence_set_names.end(); it++)
		{
			const FileObject* pFileObject = fileObjectManager.GetFileObject(*it);
			sequence_set_fasta_string.append(pFileObject->GetFastaDescriptionString());
			sequence_set_fasta_string.append("\n");
			sequence_set_fasta_string.append(pFileObject->GetSequenceString());
			sequence_set_fasta_string.append("\n\n");
		}
		
		FILE* fastaFile = fopen(fasta_filename, "w");
		//FILE* fastaFile = fopen(fasta_filename, "w");
		//TODO:: IF FILE OPEN -- and size > 0 do not overwrite (check before writing...)
		if(fastaFile)
		{
			size_t numBytesWritten = fwrite(sequence_set_fasta_string.c_str(), sequence_set_fasta_string.length(), 1, fastaFile);
			AZUL_UNUSED_VAR(numBytesWritten);

			fclose(fastaFile);
		}
		else
		{
			printf("Could not create temp fasta file...\n");
			exit(0);
		}
	}
	else
	{
		//close existing fasta file
		fclose(existing_fasta_file);
	}
	//write fileobjects in fasta format


	
	//return new .cfasta (custom -- FASTA sequence file) filename
	return std::string(fasta_filename);
}


//TODO:: (also for aligner...) Prevent re-creation of temp_files by giving them HASH_ID --> compute hash based of sequence set!!!
const std::string distanceMeasure::CalculatorFastaFormatter::create_sequence_set_fasta_file_for_aligner(bool alignment_check_flag, FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int hash_id) const
{
	//NOTE:: if sequence need to be aligned
	if(alignment_check_flag)
	{
		return this->create_sequence_set_fasta_file(fileObjectManager, sequence_set_names, hash_id);
	}
	//NOTE:: ELSE ==> DO NOT ALIGNED (check if sequence set sequences have same size)
	
	char fasta_filename[150];
	std::size_t sequence_count = sequence_set_names.size();
	//sprintf_s(fasta_filename, SystemParameters::GetFastaFileFormatString().c_str(), sequence_count, hash_id);
	SystemParameters::GetFastaFileString(fasta_filename, sequence_count, hash_id);

	//create FASTA file string
	std::string sequence_set_fasta_string;
	std::size_t species_lines_length(180u);
	sequence_set_fasta_string.reserve(sequence_count * species_lines_length);

	auto it = sequence_set_names.begin();
	const int size = fileObjectManager.GetFileObject(*it)->GetSequenceSize();
	
	//use orignal Fileobjects to create fasta file, on sequence_set_names
	for (; it != sequence_set_names.end(); ++it)
	{
		const FileObject* pFileObject = fileObjectManager.GetFileObject(*it);

		//NOTE:: all sequences must have same length
		if (pFileObject->GetSequenceSize() != size)
		{
			printf("Sequence Set sequences sizes not unanimous -- Sequences must be aligned\n");
			exit(0);
		}
		
		sequence_set_fasta_string.append(pFileObject->GetFastaDescriptionString());
		sequence_set_fasta_string.append("\n");
		sequence_set_fasta_string.append(pFileObject->GetSequenceString());
		sequence_set_fasta_string.append("\n\n");
	}

	FILE* fastaFile = fopen(fasta_filename, "w");
	//FILE* fastaFile = fopen(fasta_filename, "w");
	//TODO:: IF FILE OPEN -- and size > 0 do not overwrite (check before writing...)
	if (fastaFile)
	{
		size_t numBytesWritten = fwrite(sequence_set_fasta_string.c_str(), sequence_set_fasta_string.length(), 1, fastaFile);
		AZUL_UNUSED_VAR(numBytesWritten);

		fclose(fastaFile);
	}
	else
	{
		printf("Could not create temp fasta file...\n");
		exit(0);
	}

	//return new .cfasta (custom -- FASTA sequence file) filename
	return std::string(fasta_filename);
}