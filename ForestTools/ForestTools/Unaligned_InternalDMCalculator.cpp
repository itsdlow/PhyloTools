/********************************************************************************************************
DeAngelo Wilson
January 18 2020

								Unaligned_InternalDistanceMeasureCalculator
********************************************************************************************************/

#include "Unaligned_InternalDMCalculator.h"

#include "FileObjectManager.h"

namespace distanceMeasure
{
	void Unaligned_InternalDMCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)
	{

		//IF:: Alignment needed (MrBayes + P-Value) must align for each seqeunce_set!
		//on sequnece set --> refill FileObjectManager with new Unaligned_file		
			//CalculatorAligner::create_Unaligned_sequence_set_file();
		fileObjectManager.RefillFileObjectsBuffer(sequence_set_names, CalculatorFastaFormatter::create_sequence_set_fasta_file(fileObjectManager, sequence_set_names, sequence_set));//Unaligned_sequence_set_path);

		//pass it forward
		InternalDistanceMeasureCalculator::calculate_and_output_matrix(fileObjectManager, sequence_set_names, sequence_set, batch_id);
	}

}