
/********************************************************************************************************
DeAngelo Wilson
January 18 2020

								Tool- calculatorNexusFormatter
********************************************************************************************************/

#include "CalculatorNexusFormatter.h"

#include "FileObjectManager.h"
#include "FileObject.h"
//#include <fstream>
#include "SystemParameters.h"

std::string distanceMeasure::CalculatorNexusFormatter::create_sequence_set_nexus_file(AlignedDistanceMeasureCalculator* dmc, FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int total_sequence_count, const int hash_id) const
{
	//create new sequence_set Fileobjects , on sequence_set_names
	fileObjectManager.RefillFileObjectsBuffer( sequence_set_names, CalculatorAligner::create_sequence_set_aligned_file(dmc, fileObjectManager, sequence_set_names, total_sequence_count, hash_id) );

	std::string sequence_set_nexus_string;
	const std::size_t sequence_count = sequence_set_names.size();
	const std::size_t species_lines_length(180u);
	sequence_set_nexus_string.reserve(sequence_count * species_lines_length);
	const int NCHAR = fileObjectManager.GetCurrentSetSequenceLength();

	//nexus file formatting header
	char nexus_header[200];
	//sprintf_s(nexus_header, SystemParameters::GetNexusHeaderFormatString().c_str(), 
	//	sequence_count, NCHAR, SystemParameters::GetNexusDataTypeString().c_str(), SystemParameters::GetNexusGapChar(), SystemParameters::GetNexusMissingChar());
	SystemParameters::GetNexusHeaderString(nexus_header, sequence_count, NCHAR);
	
	char nexus_file_path[100];
	//sprintf_s(nexus_file_path, SystemParameters::GetNexusFileFormatString().c_str(), sequence_set_names.size());
	SystemParameters::GetNexusFileString(nexus_file_path, sequence_count);

	FILE* nexusFile = fopen(nexus_file_path, "w");
	sequence_set_nexus_string.append(nexus_header);

	if (nexusFile)
	{
		size_t numBytesWritten = fwrite(sequence_set_nexus_string.c_str(), sequence_set_nexus_string.length(), 1, nexusFile);
		AZUL_UNUSED_VAR(numBytesWritten);

		sequence_set_nexus_string.clear();
	}
	else
	{
		printf("Could not create temp nexus file...\n");
		exit(0);
	}
	
	
	//use sequence set Fileobjects to create nexus file, on sequence_set_names
	for (auto it = sequence_set_names.begin(); it != sequence_set_names.end(); it++)
	{
		const FileObject* pFileObject = fileObjectManager.GetSequenceSetFileObject(*it);

		//NOT IMPLEMENTED:: default name retrieval-- use fasta descriptions
		/*std::string fasta_description_string(pFileObject->GetFastaDescriptionString());
		swap_space_with_underscores(fasta_description_string);
		*/
		std::string sequenceName(pFileObject->GetSequenceName());
		swap_space_with_underscores(sequenceName);
		
		sequence_set_nexus_string.append(sequenceName);
		sequence_set_nexus_string.append("\t");



		//Must ensure all sequence chars are apart of mrBayes alphabet...
		std::string sequence_string(pFileObject->GetSequenceString());
		//Set all non alphabet characters to 'X'
		std::string valid_sequence_chars(SystemParameters::GetNexusDataTypeAlphabet());
		// find the position of each occurence of the characters in the string
		for (size_t pos = 0; (pos = sequence_string.find_first_not_of(valid_sequence_chars, pos)) != std::string::npos; ++pos)
		{
			sequence_string.replace(pos, 1u, 1u, SystemParameters::GetNexusDataTypeUnknownChar());
		}

		
		//
		//sequence_set_nexus_string.append(pFileObject->GetSequenceString());
		sequence_set_nexus_string.append(sequence_string);
		//
		sequence_set_nexus_string.append("\n");
		//write to file
		size_t numBytesWritten = fwrite(sequence_set_nexus_string.c_str(), sequence_set_nexus_string.length(), 1, nexusFile);
		AZUL_UNUSED_VAR(numBytesWritten);

		sequence_set_nexus_string.clear();
	}
	//NEXUS FOOTER???
	sequence_set_nexus_string.append(";end;");
	size_t numBytesWritten = fwrite(sequence_set_nexus_string.c_str(), sequence_set_nexus_string.length(), 1, nexusFile);
	AZUL_UNUSED_VAR(numBytesWritten);


	fclose(nexusFile);
	//return new .nxs (nexus format file on aligned sequences) filename
	return std::string(nexus_file_path);
}

void distanceMeasure::CalculatorNexusFormatter::swap_space_with_underscores(std::string& description_string) const
{
	//replace all spaces w/ "__underscores__"
	for(auto i = 0u; i < description_string.size(); i++)
	{
		if( isspace(description_string.at(i)))
		{
			description_string.replace(i, 1u, 1u, '_');
		}
	}//--> create_one_word_species_names
}
//legacy -- used with fasta sequence descritpions
void distanceMeasure::CalculatorNexusFormatter::clean_sequence_description(std::string& description_string) const
{
	//remove '>' char from FASTA description
	description_string.erase(0, 1);

	//replace all spaces w/ "__underscores__"
	for (auto i = 0u; i < description_string.size(); i++)
	{
		if (isspace(description_string.at(i)) || description_string.at(i) == ',' || description_string.at(i) == '/')
		{
			description_string.replace(i, 1u, 1u, '_');
		}
	}//--> create_one_word_species_description
}