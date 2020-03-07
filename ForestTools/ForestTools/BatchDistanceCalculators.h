/******************************************************************************
DeAngelo Wilson
January 3 2020

						BatchDistanceCalculators (strategy -- derived)
******************************************************************************/
#ifndef _BatchDistanceCalculators
#define _BatchDistanceCalculators

#include "DistanceMeasureCalculator.h"
#include "BatchCalculatorsAnalyzer.h"

namespace distanceMeasure
{
	class BatchDistanceCalculators : public DistanceMeasureCalculator, private BatchCalculatorsAnalyzer
	{
	private:
		//static const int calculator_count = 2;

	public:
		//BIG 4
		BatchDistanceCalculators(const int calculator_count);
		BatchDistanceCalculators(const BatchDistanceCalculators&) = delete;
		BatchDistanceCalculators& operator=(const BatchDistanceCalculators&) = delete;
		virtual ~BatchDistanceCalculators();

		void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id) override;
		//move to analysis tool
		//void batch_analyze_sequence_set(const std::vector<std::string>& sequence_set_names, const int batch_id);// override;


		
		std::string GetCalculatorName() const override;

		//opens log file -- for derived calc(s)
		void InitializeSequenceSetTimingsLog() override;
		//write total calc time to log FILE --> closes file
		void LogTotalCalculationTime() override;

	private:
		const int calculator_count;

		////array of calculators to perform batch analysis on
		DistanceMeasureCalculator** calculators;

		static DistanceMeasureCalculator* GetCalculator(int i);
	};
}



#endif // !_BatchDistanceCalculators
