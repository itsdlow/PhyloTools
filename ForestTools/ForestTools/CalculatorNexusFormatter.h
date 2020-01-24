/********************************************************************************************************
DeAngelo Wilson
January 18 2020

					CalculatorNexusFormatter
						-- converts an aligned .afa sequence file into NEXUS (mrBayes) formatting
********************************************************************************************************/

#ifndef _CalculatorNexusFormatter
#define _CalculatorNexusFormatter

#include <string>

namespace distanceMeasure
{
	class CalculatorNexusFormatter
	{
	public:
		CalculatorNexusFormatter() = default;
		~CalculatorNexusFormatter() = default;
		CalculatorNexusFormatter(const CalculatorNexusFormatter& ca) = delete;
		CalculatorNexusFormatter& operator=(const CalculatorNexusFormatter& ca) = delete;

		const std::string create_sequence_set_nexus_file();
	private:
		std::string path;
	};
}

#endif


