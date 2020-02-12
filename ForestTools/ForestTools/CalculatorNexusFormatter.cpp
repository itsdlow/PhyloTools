
/********************************************************************************************************
DeAngelo Wilson
January 18 2020

								Tool- calculatorNexusFormatter
********************************************************************************************************/

#include "CalculatorNexusFormatter.h"

#include "FileObjectManager.h"
#include "FileObject.h"
//#include <fstream>


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

	//todo -- move to systems parameters
	//char DATATYPE_STRING[10] = "DNA";
	char GAP_CHAR = '-';
	char MISSING_CHAR = '?';
	//get NCHAR -- max length of --sequence_set--sequences?
	int NCHAR = fileObjectManager.Get_SS_SequenceLength();

	//WINDOWS DEPENDENCE
	//nexus file formatting header
	char nexus_header[200];
	sprintf_s(nexus_header, "#NEXUS\n[comment... data, etc....]\n\n\nBEGIN data;\n\tDIMENSIONS NTAX=%zu NCHAR=%d;\n\tFORMAT DATATYPE = DNA GAP = %c MISSING = %c;\n\tMATRIX\n", 
		sequence_count, NCHAR, GAP_CHAR, MISSING_CHAR);

	//TODO -- move format string to systemsParameters
	//WiNDOWS DEPENDENCE
	char nexus_file_path[100];
	sprintf_s(nexus_file_path, "ForestFiles/TempFiles/temp_%zu.nxs", sequence_set_names.size());

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
		std::string fasta_description_string(pFileObject->GetFastaDescriptionString());
		swap_space_with_underscores(fasta_description_string);
		sequence_set_nexus_string.append(fasta_description_string);
		sequence_set_nexus_string.append("\t");
		sequence_set_nexus_string.append(pFileObject->GetSequenceString());
		sequence_set_nexus_string.append("\n");
		//write to file
		size_t numBytesWritten = fwrite(sequence_set_nexus_string.c_str(), sequence_set_nexus_string.length(), 1, nexusFile);
		sequence_set_nexus_string.clear();
	}
	//NEXUS FOOTER???

	fclose(nexusFile);
	//return new .nxs (nexus format file on aligned sequences) filename
	return std::string(nexus_file_path);
}

void distanceMeasure::CalculatorNexusFormatter::swap_space_with_underscores(std::string& description_string)
{
	//replace all spaces w/ "__underscores__"
	for(auto i = 0u; i < description_string.size(); i++)
	{
		if( isspace(description_string.at(i)) )
		{
			description_string.replace(i, 1u, 1u, '_');
		}
	}//--> create_one_word_species_description
}
