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

		void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id) override = 0;

		//internal calc specific
		float calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const override final;
		float normalize(int differenceCount, int sequencesize) const override final;
		void write_quartet_matrix(FileObjectManager& fileObjectManager, const std::vector<int>& speciesSequenceSetIndexes, const std::vector<std::string>& sequence_set_names, const int fileCount) override final;

		std::string GetCalculatorName() const override = 0;
	private:

	};
}


#endif // !_ExternalDistanceMeasureCalculator
