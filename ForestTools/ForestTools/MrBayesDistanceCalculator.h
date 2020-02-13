/******************************************************************************
DeAngelo Wilson
January 3 2020

						MrBayesDistanceCalculator (strategy -- derived)
******************************************************************************/
#ifndef _MrBayesDistanceCalculator
#define _MrBayesDistanceCalculator

#include "ExternalDistanceMeasureCalculator.h"
#include "CalculatorNexusFormatter.h"

namespace distanceMeasure
{
	class MrBayesDistanceCalculator : public ExternalDistanceMeasureCalculator, private CalculatorNexusFormatter
	{
	public:
		//BIG 4
		MrBayesDistanceCalculator() = default;
		MrBayesDistanceCalculator(const MrBayesDistanceCalculator&) = delete;
		MrBayesDistanceCalculator& operator=(const MrBayesDistanceCalculator&) = delete;
		~MrBayesDistanceCalculator() = default;
 
		void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id)  override;

		
	private:
		//batch running command creation func
		std::string create_mrbayes_default_command_block_file(const std::string& relative_nxs_path);

	};

}

#endif // !_MrBayesDistanceCalculator
