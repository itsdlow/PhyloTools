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
#include <vector>


namespace distanceMeasure
{
	class FileObjectManager;

	class CalculatorFastaFormatter
	{
	public:
		CalculatorFastaFormatter() = default;
		~CalculatorFastaFormatter() = default;
		CalculatorFastaFormatter(const CalculatorFastaFormatter& ca) = delete;
		CalculatorFastaFormatter& operator=(const CalculatorFastaFormatter& ca) = delete;

		const std::string create_sequence_set_fasta_file(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int hash_id) const;
		const std::string create_sequence_set_fasta_file_for_aligner(bool alignment_check_flag, FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int hash_id) const;
	private:
	};
}

#endif


