/********************************************************************************************************
DeAngelo Wilson
January 18 2020

								Aligned_InternalDistanceMeasureCalculator
********************************************************************************************************/

#include "Aligned_InternalDMCalculator.h"

#include "FileObjectManager.h"

namespace distanceMeasure
{
	void Aligned_InternalDMCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id)
	{
		//if alignment needed
		//ALIGN -- refill_FileObjectManager... done by derived calcs
			//align seqeunces...
		//		//create fasta file on sequence_set
		//	//CalculatorAligner::CreateAligned
		//		//where to store aligned seqeunces.... (need orignal FileObjects for lifetime...)
		
			//NOT IMPLEMENTED
				//IF:: Alignment needed (MrBayes + P-Value) must align for each seqeunce_set!!***
		//create new :aligned_sequence_set
			//on sequnece set --> refill FileObjectManager with new file		
		//CalculatorAligner::create_aligned_sequence_set_file();
		fileObjectManager.RefillFileObjectsBuffer(sequence_set_names, CalculatorAligner::create_sequence_set_aligned_file(fileObjectManager, sequence_set_names));//aligned_sequence_set_path);
		
		//pass it forward
		InternalDistanceMeasureCalculator::calculate_and_output_matrix(fileObjectManager, sequence_set_names, batch_id);
	}
}