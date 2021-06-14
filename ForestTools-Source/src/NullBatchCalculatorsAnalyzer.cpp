/**************************************************************************************************************************
DeAngelo Wilson
February 26 2020

					BatchCalculatorsAnalyzer
**************************************************************************************************************************/

#include "NullBatchCalculatorsAnalyzer.h"

//#include "TreeAnalyzer.h"


namespace distanceMeasure
{
	NullBatchCalculatorsAnalyzer::NullBatchCalculatorsAnalyzer(const int calculator_count, phylo::TreeAnalyzer* pAnalyzer) :
		BatchCalculatorsAnalyzer(calculator_count, pAnalyzer)
	{
	}

	void NullBatchCalculatorsAnalyzer::batch_analyze_sequence_set(const std::vector<std::string>& sequence_set_names, const int batch_id, bool quartets_generated)
	{
		//Do nothing -- no analysis
	}

	void NullBatchCalculatorsAnalyzer::WriteAnalysisTables() const
	{
		//Do nothing...
		//this->poStrategyAnalyzer->WriteAnalysisTables(this->ss_size_count, this->previous_ss_size);
	}

}
