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
		//NOTE:: timings skewed because only 1st aligned calculator aligns...
			//calculate and output sequence Alignment timings into seperate file
				//adjust timings of calc to not include timing of alignment...
					//set -- DMC::alignment_timing_offset --> set on each sequence set
						//must also be applied to quartets aligning...
							//whenever aligning --> calculate align_time -- ADD to DistanceMeasureCalculator::align_time_offset
								//this is 0 by default and reset each Sequence Set
								//subtracted from calculation time each StopCalculationTimer()
						//write align timings / sequence set and total...
		
		//on sequence set --> refill FileObjectManager with new aligned_file		
		fileObjectManager.RefillFileObjectsBuffer(sequence_set_names, CalculatorAligner::create_sequence_set_aligned_file(this, fileObjectManager, sequence_set_names, sequence_set));//aligned_sequence_set_path);
		//this->AddAlignmentTime(alignTime);
		
		//pass it forward
		InternalCalculatorTools::CalculateDistanceMeasuresAndTrees(this, fileObjectManager, sequence_set_names, sequence_set, batch_id);
	}
	
	//Internal calc - quartet matrix creastion --> uses values calculated from LargeListTree... (cannot be used by Aligned_Internal calc --> override)
		//OVERRIDE:: aligns each quartet sequence set 
	void Aligned_InternalDMCalculator::write_quartet_matrix(FileObjectManager& fileObjectManager, const std::vector<int>& speciesSequenceSetIndexes, const std::vector<std::string>& sequence_set_names, const int fileCount)
	{
		//FOR GIVEN SEQUENCE SET -- create sequence set line...
							//+ create aligned file --> refill sequenceSetBuffer
		const std::vector<std::string> subsequence_set_names = DistanceMeasureCalculator::CreateSubsequenceSet(sequence_set_names, speciesSequenceSetIndexes);
		const std::string subsequence_set = DistanceMeasureCalculator::CreateSubsequenceSetString(subsequence_set_names);
		fileObjectManager.RefillFileObjectsBuffer(subsequence_set_names, CalculatorAligner::create_sequence_set_aligned_file(this, fileObjectManager, subsequence_set_names, subsequence_set));

		
		const int quartetSize = 4;
		//line 0 of matrix for PHYLIP format (num of taxon)
		std::string sequence_set_res(std::to_string(quartetSize).append("\n"));
		//for each species in quartet subsequence
		for (int row = 0; row < quartetSize; row++)
		{
			const int species_index = speciesSequenceSetIndexes.at(row);
			//taxon name of current fileObject
			//this->quartetResults.append(pFileObjects[indexV.at(row)].GetFileName());
			//this->quartetResults.append(sequence_set_names.at(indexV.at(row)));
			std::string name = sequence_set_names.at(species_index);
			DistanceMeasureCalculator::swap_space_with_underscores(name);
			sequence_set_res.append(name);
			
			//append all normalized distances between species - "name" and other quartet species
			for (auto it = speciesSequenceSetIndexes.begin(); it != speciesSequenceSetIndexes.end(); it++)
			{
				//NOT IMPLEMENTED:: Guard against not found FileObject (nullptr) -> is possible?
				//pass current (i) fileobject + next (j) fileobject to Distance_Calculator
				const float normalizedDistance = this->calculate_normalized_distance(*fileObjectManager.GetSequenceSetFileObject(sequence_set_names.at(species_index)), *fileObjectManager.GetSequenceSetFileObject(sequence_set_names.at(*it)));

				//write distance to results
				sequence_set_res.append(" ");
				sequence_set_res.append(std::to_string(normalizedDistance));
			}
			sequence_set_res.append("\n");
		}
		sequence_set_res.append("\n");
		InternalCalculatorTools::AppendToQuartetResults(sequence_set_res);
	}
	
}