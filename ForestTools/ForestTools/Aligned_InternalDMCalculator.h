/********************************************************************************************************
DeAngelo Wilson
January 18 2020

								Aligned_InternalDistanceMeasureCalculator
					"has-a" calculatoraligner -- aligns sequence set 
********************************************************************************************************/

#ifndef _Aligned_InternalDMCalculator
#define _Aligned_InternalDMCalculator

#include "InternalDistanceMeasureCalculator.h"
#include "CalculatorAligner.h"

namespace distanceMeasure
{
	class Aligned_InternalDMCalculator: public InternalDistanceMeasureCalculator, private CalculatorAligner
	{
	public:
		//BIG 4
		Aligned_InternalDMCalculator() = default;
		Aligned_InternalDMCalculator(const Aligned_InternalDMCalculator&) = delete;
		Aligned_InternalDMCalculator& operator=(const Aligned_InternalDMCalculator&) = delete;
		virtual ~Aligned_InternalDMCalculator() = default;

		float calculate_normalized_distance(const FileObject & file1, const FileObject & file2) const override = 0;
		float normalize(int differenceCount, int sequencesize) const override = 0;

		//align --> forward to InternalDistanceMEasureCalc
		void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id) override final;
	};
}

#endif


