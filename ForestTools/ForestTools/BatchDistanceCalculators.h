/******************************************************************************
DeAngelo Wilson
January 3 2020

						BatchDistanceCalculators (strategy -- derived)
******************************************************************************/
#ifndef _BatchDistanceCalculators
#define _BatchDistanceCalculators

#include "DistanceMeasureCalculator.h"

#include "SystemParameters.h"

namespace distanceMeasure
{
	class BatchDistanceCalculators : public DistanceMeasureCalculator
	{
	public:
		//BIG 4
		BatchDistanceCalculators();
		BatchDistanceCalculators(const BatchDistanceCalculators&) = delete;
		BatchDistanceCalculators& operator=(const BatchDistanceCalculators&) = delete;
		virtual ~BatchDistanceCalculators();

		void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id) override;

		std::string GetCalculatorName() const override;

		//opens log file -- for derived calc(s)
		void InitializeSequenceSetTimingsLog() override;
		//write total calc time to log FILE --> closes file
		void LogTotalCalculationTime() override;

	private:
		static const int calculator_count = 3;
		//MrBayesDistanceCalculator mrbayesCalculator;
		//PValueDistanceCalculator pvalueCalculator;
		//LcsDistanceCalculator lcsCalculator;
		
		//NOTE:: GET FROM SYSTEM PARAMETERS?
		DistanceMeasureCalculator* calculators[calculator_count]{nullptr};

		static DistanceMeasureCalculator* GetCalculator(int i);
	};
}



#endif // !_BatchDistanceCalculators
