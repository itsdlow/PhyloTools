/********************************************************************************************************
DeAngelo Wilson
January 18 2020

								Tool- calculatorFastaFormatter
********************************************************************************************************/

#include "CalculatorFastaCompressor.h"

#include "FileObjectManager.h"
#include "FileObject.h"
#include "SystemParameters.h"

#include <fstream>

void distanceMeasure::CalculatorFastaCompressor::get_compressed_sequences_sizes(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& extension, const std::string& compress_command_format_string)
{
	//erase fasta sizes from previous fileset...
	this->species_compressed_fasta_sizes.erase(species_compressed_fasta_sizes.begin(), species_compressed_fasta_sizes.end());

	
	std::string file_object_fasta_string;
	int count = 1;
	const int sequence_set_size = static_cast<int>(sequence_set_names.size());
	//use orignal Fileobjects to create fasta file, on sequence_set_names
	for (auto it = sequence_set_names.begin(); it != sequence_set_names.end(); it++)
	{
		const FileObject* pFileObject = fileObjectManager.GetFileObject(*it);
		file_object_fasta_string.append(pFileObject->GetFastaDescriptionString());
		file_object_fasta_string.append("\n");
		file_object_fasta_string.append(pFileObject->GetSequenceString());
		file_object_fasta_string.append("\n");

		//TODO:: MOVE TO small function of FastaFormatter
			//std::string fasta_filename = CalculatorFastaFormatter::CreateFastaFile(file_object_fasta_string, sequence_count);
					//or -- give fileobject... (instead of fasta_string)***
		//create file
		//write fileobjects in fasta format
		char fasta_filename[150];
		//sprintf_s(fasta_filename, SystemParameters::GetCompressedFastaFileFormatString().c_str(), 1);
		SystemParameters::GetCompressedFastaFileString(fasta_filename, 1);
		
		FILE* fastaFile = fopen(fasta_filename, "w");
		//FILE* fastaFile = fopen(fasta_filename, "w");

		if (fastaFile)
		{
			size_t numBytesWritten = fwrite(file_object_fasta_string.c_str(), file_object_fasta_string.length(), 1, fastaFile);
			AZUL_UNUSED_VAR(numBytesWritten);

			fclose(fastaFile);
			file_object_fasta_string.clear();
		}
		else
		{
			printf("Could not create temp fasta file...\n");
			exit(0);
		}
		//*****************************************************

		//compress
		char output_filename[50];
		//sprintf_s(output_filename, SystemParameters::GetCompressedFilename().c_str(), extension.c_str());
		SystemParameters::GetCompressedFilename(output_filename, extension.c_str());
		//create compressor - system call
		char compress_command[200];
		//sprintf_s(compress_command, compress_command_format_string.c_str(), output_filename, fasta_filename);
		SystemParameters::GetCompressionCommand(extension, compress_command, compress_command_format_string.c_str(), output_filename, fasta_filename);
		system(compress_command);

		////read file for size
		const int size = GetCompressedFileSize_NCD(output_filename, fasta_filename);
		//store size
		this->species_compressed_fasta_sizes.emplace_back(pFileObject->GetSequenceName(), size);
		printf("Calculating compressed Sequence Set: %d/%d\r", count++, sequence_set_size);
	}
}

int distanceMeasure::CalculatorFastaCompressor::get_compressed_sequences_pair_size(const FileObject& file1, const FileObject& file2, const std::string& extension, const std::string& compress_command_format_string) const
{
	std::string file_object_pair_fasta_string;

	file_object_pair_fasta_string.append(file1.GetFastaDescriptionString());
	file_object_pair_fasta_string.append("\n");
	file_object_pair_fasta_string.append(file1.GetSequenceString());
	file_object_pair_fasta_string.append("\n");
	file_object_pair_fasta_string.append(file2.GetFastaDescriptionString());
	file_object_pair_fasta_string.append("\n");
	file_object_pair_fasta_string.append(file2.GetSequenceString());
	file_object_pair_fasta_string.append("\n");
	
	//TODO:: MOVE TO small function of FastaFormatter
		//std::string fasta_filename = CalculatorFastaFormatter::CreateFastaFile(file_object_fasta_string, sequence_count);
				//or -- give fileobject... (instead of fasta_string)***
	//create file
	//write fileobjects in fasta format
	char fasta_filename[150];
	//sprintf_s(fasta_filename, SystemParameters::GetCompressedFastaFileFormatString().c_str(), 2);
	SystemParameters::GetCompressedFastaFileString(fasta_filename, 2);

	
	FILE* fastaFile = fopen(fasta_filename, "w");
	//FILE* fastaFile = fopen(fasta_filename, "w");

	if (fastaFile)
	{
		size_t numBytesWritten = fwrite(file_object_pair_fasta_string.c_str(), file_object_pair_fasta_string.length(), 1, fastaFile);
		AZUL_UNUSED_VAR(numBytesWritten);
		
		fclose(fastaFile);
		//file_object_pair_fasta_string.clear();
	}
	else
	{
		printf("Could not create temp fasta file...\n");
		exit(0);
	}
	//*****************************************************


	
	//compress
	char output_filename[50];
	//sprintf_s(output_filename, SystemParameters::GetCompressedFilename().c_str(), extension.c_str());
	SystemParameters::GetCompressedFilename(output_filename, extension.c_str());
	//create compressor - system call
	char compress_command[200];
	//sprintf_s(compress_command, compress_command_format_string.c_str(), output_filename, fasta_filename);
	SystemParameters::GetCompressionCommand(extension, compress_command, compress_command_format_string.c_str(), output_filename, fasta_filename);
	system(compress_command);

	//read file for size
	return GetCompressedFileSize_NCD(output_filename, fasta_filename);
}

int distanceMeasure::CalculatorFastaCompressor::GetCompressedFileSize_NCD(char* output_filename, char* fasta_filename)
{
	//read file for size
	std::ifstream compressedFile(output_filename, std::ios::binary);

	if (!compressedFile.is_open())
	{
		printf("File at path: %s - could not compress fasta file to %s\n", fasta_filename, output_filename);
		exit(0);
	}
	std::streampos begin, end;
	begin = compressedFile.tellg();
	compressedFile.seekg(0, std::ios::end);
	end = compressedFile.tellg();
	compressedFile.close();
	const int size = static_cast<int>(end - begin);//size of compressed file in bytes

	//delete compressed file
	if( remove(output_filename) != 0 )
	{
		printf("Failed to delete compressed file: %s\n", output_filename);
	}
	
	return size;
}