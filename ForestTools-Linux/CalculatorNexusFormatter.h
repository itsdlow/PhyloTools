/********************************************************************************************************
DeAngelo Wilson
January 18 2020

					CalculatorNexusFormatter
						-- converts an aligned .afa sequence file into NEXUS (mrBayes) formatting
********************************************************************************************************/

#ifndef _CalculatorNexusFormatter
#define _CalculatorNexusFormatter

#include <string>
#include "CalculatorAligner.h"

namespace distanceMeasure
{
	class CalculatorNexusFormatter: private CalculatorAligner
	{
	public:
		CalculatorNexusFormatter() = default;
		~CalculatorNexusFormatter() = default;
		CalculatorNexusFormatter(const CalculatorNexusFormatter& ca) = delete;
		CalculatorNexusFormatter& operator=(const CalculatorNexusFormatter& ca) = delete;

		std::string create_sequence_set_nexus_file(AlignedDistanceMeasureCalculator* dmc, FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int total_sequence_count, const int hash_id) const;
	private:
		void swap_space_with_underscores(std::string& description_string) const;
		void clean_sequence_description(std::string& description_string) const;

	};
}

#endif


