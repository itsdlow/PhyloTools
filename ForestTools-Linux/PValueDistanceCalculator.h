/******************************************************************************
DeAngelo Wilson
January 3 2020

						PValueDistanceCalculator (strategy -- derived)
******************************************************************************/
#ifndef _PValueDistanceCalculator
#define _PValueDistanceCalculator

#include "Aligned_InternalDMCalculator.h"

namespace distanceMeasure
{
	class PValueDistanceCalculator: public Aligned_InternalDMCalculator
	{
	public:
		//BIG 4
		PValueDistanceCalculator() = delete;
		PValueDistanceCalculator(RunFlags* flags);
		
		PValueDistanceCalculator(const PValueDistanceCalculator&) = delete;
		PValueDistanceCalculator& operator=(const PValueDistanceCalculator&) = delete;
		virtual ~PValueDistanceCalculator() = default;

		float calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const override;
		float normalize(int differenceCount, int sequencesize) const override;

		void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id) override;

		std::string GetCalculatorName() const override;

	private:


	};

}

#endif // !_PValueDistanceCalculator
