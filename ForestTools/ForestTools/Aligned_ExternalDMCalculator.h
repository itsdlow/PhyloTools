/********************************************************************************************************
DeAngelo Wilson
January 18 2020

								Aligned_ExternalDistanceMeasureCalculator
					"has-a" calculatoraligner -- aligns sequence set
********************************************************************************************************/

#ifndef _Aligned_ExternalDMCalculator
#define _Aligned_ExternalDMCalculator

#include "ExternalDistanceMeasureCalculator.h"
#include "CalculatorAligner.h"

namespace distanceMeasure
{
	class Aligned_ExternalDMCalculator : public ExternalDistanceMeasureCalculator, private CalculatorAligner
	{
	public:
		//BIG 4
		Aligned_ExternalDMCalculator() = default;
		Aligned_ExternalDMCalculator(const Aligned_ExternalDMCalculator&) = delete;
		Aligned_ExternalDMCalculator& operator=(const Aligned_ExternalDMCalculator&) = delete;
		~Aligned_ExternalDMCalculator() = default;

		//align --> forward to ExternalDistanceMEasureCalc
		void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id) override final;
	};
}

#endif


