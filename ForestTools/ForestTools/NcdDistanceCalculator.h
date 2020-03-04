/******************************************************************************
DeAngelo Wilson
January 3 2020

						NcdDistanceCalculator (strategy -- derived)
******************************************************************************/
#ifndef _NcdDistanceCalculator
#define _NcdDistanceCalculator

#include "InternalDistanceMeasureCalculator.h"
#include "CalculatorFastaCompressor.h"

namespace distanceMeasure
{
	class NcdDistanceCalculator : public InternalDistanceMeasureCalculator, private CalculatorFastaCompressor
	{
	public:
		//BIG 4
		NcdDistanceCalculator(const int compressor_flag);
		
		NcdDistanceCalculator() = delete;
		NcdDistanceCalculator(const NcdDistanceCalculator&) = delete;
		NcdDistanceCalculator& operator=(const NcdDistanceCalculator&) = delete;
		virtual ~NcdDistanceCalculator() = default;

		float calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const override;
		float normalize(int differenceCount, int sequencesize) const override;

		void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id) override;

		std::string GetCalculatorName() const override;

	private:
		std::string extension;
		std::string compress_command_format_string;

		//st

		void SetCompressorType(const int compressor_flag);
		static float computeNCDistance(int size_ij, int size_i, int size_j);
	};

}

#endif // !_NcdDistanceCalculator
