/******************************************************************************
DeAngelo Wilson
January 3 2020

						CompareTreeCalculator (strategy -- derived)
******************************************************************************/
#ifndef _CompareTreeCalculator
#define _CompareTreeCalculator

#include "NullDistanceMeasureCalculator.h"

namespace distanceMeasure
{
	//a null CompareTree calculator --> can only be 1 per analysis, and can change based on current "InputSequenceFileSet"
	class CompareTreeCalculator : public NullDistanceMeasureCalculator
	{
	public:
		//BIG 4
		CompareTreeCalculator() = delete;
		CompareTreeCalculator(RunFlags* flags, const std::string& name, const std::string& tree_filename);

		CompareTreeCalculator(const CompareTreeCalculator&) = delete;
		CompareTreeCalculator& operator=(const CompareTreeCalculator&) = delete;
		virtual ~CompareTreeCalculator() override = default;

		//called by LargeTreeAnalyzer to generate analysis tables
		virtual void GetLargeListTreeFileName(char* buffer, const int batch_number, const size_t sequence_count) const;

		
	private:
		std::string filename;
	};


}

#endif // !_CompareTreeCalculator
