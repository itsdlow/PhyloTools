/******************************************************************************
DeAngelo Wilson
January 3 2020

						NullDistanceMeasureCalculator (strategy -- derived)
******************************************************************************/
#ifndef _NullDistanceMeasureCalculator
#define _NullDistanceMeasureCalculator

#include "DistanceMeasureCalculator.h"

namespace distanceMeasure
{

	class NullDistanceMeasureCalculator : public DistanceMeasureCalculator
	{
	public:
		//BIG 4
		NullDistanceMeasureCalculator() = delete;
		NullDistanceMeasureCalculator(RunFlags* flags, const std::string& name);

		NullDistanceMeasureCalculator(const NullDistanceMeasureCalculator&) = delete;
		NullDistanceMeasureCalculator& operator=(const NullDistanceMeasureCalculator&) = delete;
		virtual ~NullDistanceMeasureCalculator() override = default;

		float calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const override { /* do nothing */ return 0.0f; }
		float normalize(int differenceCount, int sequencesize) const override { /* do nothing */ return  0.0f; }

		void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id) override;
		void write_quartet_matrix(FileObjectManager& fileObjectManager, const std::vector<int>& speciesSequenceSetIndexes, const std::vector<std::string>& sequence_set_names, const int fileCount, const int quartet_count) override;

	private:

	};


}

#endif // !_NullDistanceMeasureCalculator
