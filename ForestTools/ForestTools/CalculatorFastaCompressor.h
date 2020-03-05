/**************************************************************************************************************************
DeAngelo Wilson
January 18 2020

					CalculatorFastaCompressor
						-- Used by NCD_calc to create compressed files for calcaulating Normalized_compression_Distance
							--must create FASTA file for sequence set first
**************************************************************************************************************************/

#ifndef _CalculatorFastaCompressor
#define _CalculatorFastaCompressor

#include "CalculatorFastaFormatter.h"

#include <string>
#include <vector>
#include <utility>

namespace distanceMeasure
{
	class FileObject;
	class FileObjectManager;

	class CalculatorFastaCompressor: private CalculatorFastaFormatter
	{
	public:
		CalculatorFastaCompressor() = default;
		~CalculatorFastaCompressor() = default;
		CalculatorFastaCompressor(const CalculatorFastaCompressor& ca) = delete;
		CalculatorFastaCompressor& operator=(const CalculatorFastaCompressor& ca) = delete;

		void get_compressed_sequences_sizes(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& extension, const std::string& compress_command_format_string);

		//reference vector<pairs> get size of concatenated fileobjects 
		int get_compressed_sequences_pair_size(const FileObject& file1, const FileObject& file2, const std::string& extension, const std::string& compress_command_format_string) const;
		
		//const std::string create_compressed_sequence_set_file( FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& extension, const std::string& compress_command_format_string);

		//const std::string create_sequence_set_fasta_file(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names);
		std::vector<std::pair<std::string, int> > species_compressed_fasta_sizes;
	private:

		
		//stored directly in InternalDistanceMeasureCalculator::results --> file
		//std::vector<std::pair<std::string, int> > species_pair_sizes;
	};
}

#endif


