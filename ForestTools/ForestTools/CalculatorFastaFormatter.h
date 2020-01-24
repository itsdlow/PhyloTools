/**************************************************************************************************************************
DeAngelo Wilson
January 18 2020

					CalculatorFastaFormatter
						-- DistanceMEasureCalculators that require reformatting of FileObjects --> new_Fasta_file
								(i.e. aligned sequences -- each new sequence set)
**************************************************************************************************************************/

#ifndef _CalculatorFastaFormatter
#define _CalculatorFastaFormatter

#include <string>

namespace distanceMeasure
{
	class CalculatorFastaFormatter
	{
	public:
		CalculatorFastaFormatter() = default;
		~CalculatorFastaFormatter() = default;
		CalculatorFastaFormatter(const CalculatorFastaFormatter& ca) = delete;
		CalculatorFastaFormatter& operator=(const CalculatorFastaFormatter& ca) = delete;

		const std::string create_sequence_set_fasta_file();
	private:
		std::string path;
	};
}

#endif


