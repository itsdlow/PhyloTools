/**************************************************************************************************************************
DeAngelo Wilson
February 26 2020

					NullBatchCalculatorsAnalyzer
						-- Perform analysis of trees for BatchDistanceCalculators
							-- (DERIVED) ==> DOES NOT -Compute Compatibilities for permutations of calculator_methods
								--outputs files to ../ForestFiles/Analysis/..
**************************************************************************************************************************/

#ifndef _NullBatchCalculatorsAnalyzer
#define _NullBatchCalculatorsAnalyzer


#include "BatchCalculatorsAnalyzer.h"


namespace distanceMeasure
{

	class NullBatchCalculatorsAnalyzer: public BatchCalculatorsAnalyzer
	{
	public:
		NullBatchCalculatorsAnalyzer() = delete;
		NullBatchCalculatorsAnalyzer(const int calculator_count, phylo::TreeAnalyzer* pAnalyzer);

		virtual ~NullBatchCalculatorsAnalyzer() = default;
		NullBatchCalculatorsAnalyzer(const NullBatchCalculatorsAnalyzer& bca) = delete;
		NullBatchCalculatorsAnalyzer& operator=(const NullBatchCalculatorsAnalyzer& bca) = delete;

		virtual void batch_analyze_sequence_set(const std::vector<std::string>& sequence_set_names, const int batch_id, bool quartets_generated) override;

		//called by BatchDistanceCalculators
		virtual void WriteAnalysisTables() const override;
	private:


	};
}

#endif


