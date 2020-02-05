/********************************************************************************************************
DeAngelo Wilson
January 18 2020

					CalculatorAligner
						-- DistanceMEasureCalculators that require aligned
							Seqeunces "has-a" aligner -- called before matrix_calculation

							//accesses muscle 3rd party Software (https://www.drive5.com/muscle/)
		Edgar, R.C. (2004) MUSCLE: multiple sequence alignment with high accuracy and high throughput
  Nucleic Acids Res. 32(5):1792-1797
********************************************************************************************************/

#ifndef _CalculatorAligner
#define _CalculatorAligner

#include <string>
#include "CalculatorFastaFormatter.h"

namespace distanceMeasure
{
	class FileObjectManager;
	
	class CalculatorAligner : private CalculatorFastaFormatter
	{
	public:
		CalculatorAligner() = default;
		~CalculatorAligner() = default;
		CalculatorAligner(const CalculatorAligner& ca) = delete;
		CalculatorAligner& operator=(const CalculatorAligner& ca) = delete;

		const std::string create_sequence_set_aligned_file(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names);
	private:
	};
} 

#endif


