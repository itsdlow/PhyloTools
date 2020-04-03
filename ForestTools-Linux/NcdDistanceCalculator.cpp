/********************************************************************************************************
DeAngelo Wilson
January 3 2020

					NcdDistanceMeasureCalculator -- derived dmc -- njp
********************************************************************************************************/


#include "NcdDistanceCalculator.h"

#include "SystemParameters.h"
#include "FileObject.h"
#include "VectorKeyCompare.h"

namespace distanceMeasure
{
	distanceMeasure::NcdDistanceCalculator::NcdDistanceCalculator(const int compressor_flag)
	{
		this->SetCompressorType(compressor_flag);
	}

	//TODO:: CREATE DERIVED NCD CALCULATORS
		//i.e. "Ncd7ZipCalculator"
	void distanceMeasure::NcdDistanceCalculator::SetCompressorType(const int compressor_flag)
	{
		//TODO:: extract to system parameters???
		switch (compressor_flag)
		{
		case 0:
			//all compressors
			break;
		case 1:
			//7zip
			this->extension = "7z";
			this->compress_command_format_string = SystemParameters::Get7ZipCommandString();
			break;
		case 2:
			//mfcompress1
			this->extension = "mfc1";
			this->compress_command_format_string = SystemParameters::GetMFC1CommandString();
			break;
		case 3:
			//mfcompress2
			this->extension = "mfc2";
			this->compress_command_format_string = SystemParameters::GetMFC2CommandString();
			break;
		case 4:
			//mfcompress3
			this->extension = "mfc3";
			this->compress_command_format_string = SystemParameters::GetMFC3CommandString();
			break;
		case 5:
			//winzip
			break;
		default:
			break;
		}
	}
	
	//driving method
	void distanceMeasure::NcdDistanceCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)
	{
		this->StartCalculationTimer();

		//create fasta file for each sequence of set...
			//create compressed file on each FASTA
				//store size of file in a Vector<std::pair<std::string, int size>> sequence_set_compressed_sizes
		CalculatorFastaCompressor::get_compressed_sequences_sizes(fileObjectManager, sequence_set_names, this->extension, this->compress_command_format_string);
		//std::string compressed_file = CalculatorFastaCompressor::create_compressed_sequence_set_file(fileObjectManager, sequence_set_names, this->extension, this->compress_command_format_string);

		//create normalized distances between each pairing
			//create and concatenate fasta file for both sequences
			//
		Unaligned_InternalDMCalculator::calculate_and_output_matrix(fileObjectManager, sequence_set_names, sequence_set, batch_id);

		this->StopCalculationTimer(batch_id, sequence_set);
	}

	std::string distanceMeasure::NcdDistanceCalculator::GetCalculatorName() const
	{
		return this->extension;
	}

	//... assumes compression files already created?
	float NcdDistanceCalculator::calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const
	{
		//assumes aligned sequences
		//const int m = file1.GetSequenceSize();
		//const int n = file2.GetSequenceSize();
		const std::string species1 = file1.GetSequenceName();
		const std::string species2 = file2.GetSequenceName();

		//if same sequence -- do not create pair
		if (species1 == species2)
		{
			return 0.0f;
		}
		//CREATE CONCATENATED compressed file on fileobjects
			//create fasta file for both fileobjects ==> concatenate
		int pair_size = CalculatorFastaCompressor::get_compressed_sequences_pair_size(file1, file2, this->extension, this->compress_command_format_string);

		//store species sizes
		const VectorKeyCompare pred1 = VectorKeyCompare(species1);
		const VectorKeyCompare pred2 = VectorKeyCompare(species2);
		//move to CalculatorFastaCompressor 
			//FindSpeciesEntry(pred)
		//TODO:: WIP 
		//get species 1 size --std::vector<VectorKeyCompare::species_and_size>::const_iterator
		auto speciesEntry1 = std::find_if(CalculatorFastaCompressor::species_compressed_fasta_sizes.begin(), CalculatorFastaCompressor::species_compressed_fasta_sizes.end(), pred1);
		//get species 2 size
		auto speciesEntry2 = std::find_if(CalculatorFastaCompressor::species_compressed_fasta_sizes.begin(), CalculatorFastaCompressor::species_compressed_fasta_sizes.end(), pred2);

		return this->computeNCDistance(pair_size, speciesEntry1->second, speciesEntry2->second);;
	}


	float distanceMeasure::NcdDistanceCalculator::computeNCDistance(int size_ij, int size_i, int size_j)
	{
		int max_size, min_size;
		if(size_i > size_j)
		{
			max_size = size_i;
			min_size = size_j;
		}
		else
		{
			max_size = size_j;
			min_size = size_i;
		}
		return static_cast<float>(size_ij - min_size) / static_cast<float>(max_size);
	}
	//not needed by NCD calcs...
	float distanceMeasure::NcdDistanceCalculator::normalize(int differenceCount, int sequencesize) const
	{
		return 0.0f;
	}
}
