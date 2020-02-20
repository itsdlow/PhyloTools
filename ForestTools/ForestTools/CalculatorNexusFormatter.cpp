
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

//TODO
const std::string distanceMeasure::CalculatorNexusFormatter::create_sequence_set_nexus_file(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names)
{
	//create new sequence_set Fileobjects , on sequence_set_names
	fileObjectManager.RefillFileObjectsBuffer( sequence_set_names, CalculatorAligner::create_sequence_set_aligned_file(fileObjectManager, sequence_set_names) );

	//...
	std::string sequence_set_nexus_string;
	std::size_t sequence_count = sequence_set_names.size();
	std::size_t species_lines_length(180u);
	sequence_set_nexus_string.reserve(sequence_count * species_lines_length);
	const int NCHAR = fileObjectManager.Get_SS_SequenceLength();

	//WINDOWS DEPENDENCE
	//nexus file formatting header
	char nexus_header[200];
	sprintf_s(nexus_header, SystemParameters::GetNexusHeaderFormatString().c_str(), 
		sequence_count, NCHAR, SystemParameters::GetNexusDataTypeString().c_str(), SystemParameters::GetNexusGapChar(), SystemParameters::GetNexusMissingChar());

	//WiNDOWS DEPENDENCE
	char nexus_file_path[100];
	sprintf_s(nexus_file_path, SystemParameters::GetNexusFileFormatString().c_str(), sequence_set_names.size());

	//WiNDOWS DEPENDENCE
	FILE* nexusFile;
	fopen_s(&nexusFile, nexus_file_path, "w");
	sequence_set_nexus_string.append(nexus_header);

	if (nexusFile)
	{
		size_t numBytesWritten = fwrite(sequence_set_nexus_string.c_str(), sequence_set_nexus_string.length(), 1, nexusFile);
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
		sequence_set_nexus_string.append(pFileObject->GetSequenceString());
		sequence_set_nexus_string.append("\n");
		//write to file
		size_t numBytesWritten = fwrite(sequence_set_nexus_string.c_str(), sequence_set_nexus_string.length(), 1, nexusFile);
		sequence_set_nexus_string.clear();
	}
	//NEXUS FOOTER???
	sequence_set_nexus_string.append(";end;");
	size_t numBytesWritten = fwrite(sequence_set_nexus_string.c_str(), sequence_set_nexus_string.length(), 1, nexusFile);

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
		if (isspace(description_string.at(i)) || description_string.at(i) == ',')
		{
			description_string.replace(i, 1u, 1u, '_');
		}
	}//--> create_one_word_species_description
}