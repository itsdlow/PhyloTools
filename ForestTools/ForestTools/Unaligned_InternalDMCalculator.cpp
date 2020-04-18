/********************************************************************************************************
DeAngelo Wilson
January 18 2020

								Unaligned_InternalDistanceMeasureCalculator
********************************************************************************************************/

#include "Unaligned_InternalDMCalculator.h"

#include "FileObjectManager.h"

namespace distanceMeasure
{

	distanceMeasure::Unaligned_InternalDMCalculator::Unaligned_InternalDMCalculator(RunFlags* flags):
	DistanceMeasureCalculator(flags)
	{
	}
	
	void Unaligned_InternalDMCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)
	{

		//IF:: Alignment needed (MrBayes + P-Value) must align for each seqeunce_set!
		//on sequnece set --> refill FileObjectManager with new Unaligned_file		
			//CalculatorAligner::create_Unaligned_sequence_set_file();
		fileObjectManager.RefillFileObjectsBuffer(sequence_set_names, CalculatorFastaFormatter::create_sequence_set_fasta_file(fileObjectManager, sequence_set_names, sequence_set));//Unaligned_sequence_set_path);

		//pass it forward -- Calculate LargeTree & Quartet Trees Distance Measures
			// + create tree (fastME)	
		InternalCalculatorTools::CalculateDistanceMeasuresAndTrees(this, fileObjectManager, sequence_set_names, sequence_set, batch_id);
	}

	//Internal calc - quartet matrix creastion --> uses values calculated from LargeListTree... (cannot be used by Aligned_Internal calc --> override)
	void Unaligned_InternalDMCalculator::write_quartet_matrix(FileObjectManager& fileObjectManager, const std::vector<int>& speciesSequenceSetIndexes, const std::vector<std::string>& sequence_set_names, const int fileCount)
	{
		const int quartetSize = 4;
		std::string sequence_set_res(std::to_string(quartetSize).append("\n"));

		//line 0 of matrix for PHYLIP format (num of taxon)
		//quartetResults.append(std::to_string(quartetSize).append("\n"));
		for (int row = 0; row < quartetSize; row++)
		{
			//taxon name of current fileObject
			//this->quartetResults.append(pFileObjects[indexV.at(row)].GetFileName());
			//this->quartetResults.append(sequence_set_names.at(indexV.at(row)));
			std::string name = sequence_set_names.at(speciesSequenceSetIndexes.at(row));
			//TODO:: refine so dont need to always swap -- change FOM to store one-word-sequence_names
			DistanceMeasureCalculator::swap_space_with_underscores(name);
			sequence_set_res.append(name);

			//append all normalized distances between species - "name" and other quartet species
			for (auto it = speciesSequenceSetIndexes.begin(); it != speciesSequenceSetIndexes.end(); it++)
			{
				sequence_set_res.append(" ");
				sequence_set_res.append(std::to_string(InternalCalculatorTools::GetLamdaMatrixDistanceAt(DistanceMeasureCalculator::getArrayIndex(speciesSequenceSetIndexes.at(row), *it, fileCount))));

			}
			sequence_set_res.append("\n");
		}
		sequence_set_res.append("\n");
		//TODO:: MUST WRITE TO FILE AND CLEAR InternalCalculatorTools::'quartetResults' string
		InternalCalculatorTools::AppendToQuartetResults(sequence_set_res);
	}
}