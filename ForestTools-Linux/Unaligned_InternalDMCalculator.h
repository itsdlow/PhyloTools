/********************************************************************************************************
DeAngelo Wilson
January 18 2020

								Unaligned_InternalDistanceMeasureCalculator
					"has-a" calculatoraligner -- aligns sequence set
********************************************************************************************************/

#ifndef _Unaligned_InternalDMCalculator
#define _Unaligned_InternalDMCalculator

//#include "InternalDistanceMeasureCalculator.h"
#include "DistanceMeasureCalculator.h"

#include "InternalCalculatorTools.h"
#include "CalculatorFastaFormatter.h"

namespace distanceMeasure
{
	class Unaligned_InternalDMCalculator : public DistanceMeasureCalculator, private CalculatorFastaFormatter, private InternalCalculatorTools
	{
	public:
		//BIG 4
		Unaligned_InternalDMCalculator() = delete;
		Unaligned_InternalDMCalculator(RunFlags* flags, const std::string& name);
		
		Unaligned_InternalDMCalculator(const Unaligned_InternalDMCalculator&) = delete;
		Unaligned_InternalDMCalculator& operator=(const Unaligned_InternalDMCalculator&) = delete;
		virtual ~Unaligned_InternalDMCalculator() = default;

		float calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const override = 0;
		float normalize(int differenceCount, int sequencesize) const override = 0;

		//align --> forward to InternalDistanceMEasureCalc
		void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id) override;

		void write_quartet_matrix(FileObjectManager& fileObjectManager, const std::vector<int>& speciesSequenceSetIndexes, const std::vector<std::string>& sequence_set_names, const int fileCount, const int quartet_count) override;

	};
}

#endif


