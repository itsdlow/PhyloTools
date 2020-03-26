/********************************************************************************************************
DeAngelo Wilson
January 18 2020

								Aligned_InternalDistanceMeasureCalculator
********************************************************************************************************/

#include "Aligned_InternalDMCalculator.h"

#include "FileObjectManager.h"

namespace distanceMeasure
{
	void Aligned_InternalDMCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)
	{

		//on sequence set --> refill FileObjectManager with new aligned_file		
		fileObjectManager.RefillFileObjectsBuffer(sequence_set_names, CalculatorAligner::create_sequence_set_aligned_file(fileObjectManager, sequence_set_names, sequence_set));//aligned_sequence_set_path);

		//pass it forward
		InternalDistanceMeasureCalculator::calculate_and_output_matrix(fileObjectManager, sequence_set_names, sequence_set, batch_id);
	}
	
	//Internal calc - quartet matrix creastion --> uses values calculated from LargeListTree... (cannot be used by Aligned_Internal calc --> override)
		//OVERRIDE:: aligns each quartet sequence set 
	void Aligned_InternalDMCalculator::write_quartet_matrix(FileObjectManager& fileObjectManager, const std::vector<int>& speciesSequenceSetIndexes, const std::vector<std::string>& sequence_set_names, const int fileCount)
	{
		//const FileObject* const pFileObjects = reinterpret_cast<const FileObject*>(this->getFileObjectManager().getFileObjectsAddr());
		//const int fileCount = this->getFileObjectManager().get_file_count();

		//TODO::FOR GIVEN SEQUENCE SET -- create sequence set line...
							//+ create aligned file --> refill sequenceSetBuffer
		const std::vector<std::string> subsequence_set_names = DistanceMeasureCalculator::CreateSubsequenceSet(sequence_set_names, speciesSequenceSetIndexes);
		const std::string subsequence_set = DistanceMeasureCalculator::CreateSubsequenceSetString(subsequence_set_names);
		fileObjectManager.RefillFileObjectsBuffer(subsequence_set_names, CalculatorAligner::create_sequence_set_aligned_file(fileObjectManager, subsequence_set_names, subsequence_set));

		
		const int quartetSize = 4;
		//line 0 of matrix for PHYLIP format (num of taxon)
		this->quartetResults.append(std::to_string(quartetSize).append("\n"));
		//for each species in quartet subsequence
		for (int row = 0; row < quartetSize; row++)
		{
			const int species_index = speciesSequenceSetIndexes.at(row);
			//taxon name of current fileObject
			//this->quartetResults.append(pFileObjects[indexV.at(row)].GetFileName());
			//this->quartetResults.append(sequence_set_names.at(indexV.at(row)));
			std::string name = sequence_set_names.at(species_index);
			DistanceMeasureCalculator::swap_space_with_underscores(name);
			this->quartetResults.append(name);
			
			//append all normalized distances between species - "name" and other quartet species
			for (auto it = speciesSequenceSetIndexes.begin(); it != speciesSequenceSetIndexes.end(); it++)
			{
				//NOT IMPLEMENTED:: Guard against not found FileObject (nullptr) -> is possible?
				//pass current (i) fileobject + next (j) fileobject to Distance_Calculator
				const float normalizedDistance = this->calculate_normalized_distance(*fileObjectManager.GetSequenceSetFileObject(sequence_set_names.at(species_index)), *fileObjectManager.GetSequenceSetFileObject(sequence_set_names.at(*it)));

				//write distance to results
				this->quartetResults.append(" ");
				this->quartetResults.append(std::to_string(normalizedDistance));
			}
			this->quartetResults.append("\n");
		}
		this->quartetResults.append("\n");
	}
	
}