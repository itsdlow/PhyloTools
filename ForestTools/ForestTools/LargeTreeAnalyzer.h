/**************************************************************************************************************************
DeAngelo Wilson
February 26 2020

					LargeTreeAnalyzer
						-- Perform analysis of trees for BatchDistanceCalculators
							--Compute Compatibilities for permutations of calculator_methods
								--outputs files to ../ForestFiles/Analysis/..
**************************************************************************************************************************/

#ifndef _LargeTreeAnalyzer
#define _LargeTreeAnalyzer

#include "TreeAnalyzer.h"

#include <vector>


namespace phylo
{
	//forward declarations
	//class DistanceMeasureCalculator;

	class LargeTreeAnalyzer : public TreeAnalyzer
	{
	public:
		LargeTreeAnalyzer() = delete;
		LargeTreeAnalyzer(distanceMeasure::DistanceMeasureCalculator* const* pCalculators, const int calculator_count);

		virtual ~LargeTreeAnalyzer() = default;
		LargeTreeAnalyzer(const LargeTreeAnalyzer& bca) = delete;
		LargeTreeAnalyzer& operator=(const LargeTreeAnalyzer& bca) = delete;

		virtual void CalculateAnalysisMeasures(const int calculator_index_i, const int calculator_index_j, const int sequence_set_size, const int batch_id) override;
		virtual void InitializeCompatibilityVectors(const int table_dimension) override;
		virtual void WriteAnalysisTables(const int ss_size_count, const int ss_size) override;
	private:

		////2D array
		std::vector<float> compatibilityTable_LtoL;
		//std::vector<float> compatibilityTable_IQtoQ;
		//std::vector<float> compatibilityTable_QtoQ;
		std::vector<float> compatibilityTable_IQtoIQ;

		//new analysis measure vectors
		std::vector<float> compatibilityTable_LtoL_;
		//std::vector<float> compatibilityTable_IQtoQ_;
		//std::vector<float> compatibilityTable_QtoQ_;
		std::vector<float> compatibilityTable_IQtoIQ_;
	};
}

#endif


