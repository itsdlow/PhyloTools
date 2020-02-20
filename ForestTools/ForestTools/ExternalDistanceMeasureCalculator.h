/***********************************************************************************************
DeAngelo Wilson
January 18 2020

			ExternalDistanceMeasureCalculator (strategy -- derived-base)
					--DistanceMatrix created by 3rd party software (MrBayes,...
***********************************************************************************************/
#ifndef _ExternalDistanceMeasureCalculator
#define _ExternalDistanceMeasureCalculator

#include "DistanceMeasureCalculator.h"

#include <string>
#include <vector>

namespace distanceMeasure
{
	class ExternalDistanceMeasureCalculator : public DistanceMeasureCalculator
	{
	public:
		//BIG 4
		ExternalDistanceMeasureCalculator() = default;
		ExternalDistanceMeasureCalculator(const ExternalDistanceMeasureCalculator&) = delete;
		ExternalDistanceMeasureCalculator& operator=(const ExternalDistanceMeasureCalculator&) = delete;
		virtual ~ExternalDistanceMeasureCalculator() = default;

		void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id) override = 0;
		void create_tree(const std::vector<std::string>& sequence_set_names, const int batch_id) override = 0;

		//External calc specific funcs
		
		//internal calc specific funcs
					//fill results buffer with distance Measures + LAMDAMATRIX for allquartetsMethod
		//void CalculateLargeTreeDistanceMeasures(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names);
		//void CalculateAllQuartetsDistanceMeasures(const std::vector<std::string>& sequence_set_names);

		//void write_quartet_matrix(int i, int j, int k, int l, const std::vector<std::string>& sequence_set_names, const int fileCount);

		//write results buffer to output FILE, closes FILE
		//void write_batch_results(const int batch_number, const size_t sequence_count);
		//
		std::string GetCalculatorName() const override = 0;
	private:

	};
}


#endif // !_ExternalDistanceMeasureCalculator
