/**************************************************************************************************************************
DeAngelo Wilson
February 26 2020

					BatchCalculatorsAnalyzer
						-- Perform analysis of trees for BatchDistanceCalculators
							--Compute Compatibilities for permutations of calculator_methods
								--outputs files to ../ForestFiles/Analysis/..
**************************************************************************************************************************/

#ifndef _BatchCalculatorsAnalyzer
#define _BatchCalculatorsAnalyzer

#include <string>
#include <vector>

//forward decls
namespace phylo
{
	class TreeAnalyzer;
}

namespace distanceMeasure
{
	//forward declarations
	class DistanceMeasureCalculator;
	
	class BatchCalculatorsAnalyzer
	{
	public:
		BatchCalculatorsAnalyzer() = delete;
		BatchCalculatorsAnalyzer(const int calculator_count, phylo::TreeAnalyzer* pAnalyzer);
		
		virtual ~BatchCalculatorsAnalyzer();
		BatchCalculatorsAnalyzer(const BatchCalculatorsAnalyzer& bca) = delete;
		BatchCalculatorsAnalyzer& operator=(const BatchCalculatorsAnalyzer& bca) = delete;

		virtual void batch_analyze_sequence_set(const std::vector<std::string>& sequence_set_names, const int batch_id, bool quartets_generated);

		//called by BatchCalculators
		virtual void WriteAnalysisTables() const;
		
	private:
		const int calculator_count;
		//ptr to array of pointers, which hold constant calculators
			//CANNOT change calculator elements
		//DistanceMeasureCalculator* const* pCalculators;
		phylo::TreeAnalyzer* poStrategyAnalyzer;

		int previous_ss_size = 0;
		int ss_size_count = 0;

		//const int standard_label_size = 6;

		//private helpers
		//void WriteAnalysisTable(const std::vector<float>& table_vector, const std::string& table_description);
		//
		//std::string GetStandardizedCalculatorLabels() const;
		//std::string StandardizeCalculatorLabel(const int index) const;

		
		/*
		 *  TO MOVE
		 */
		//TREE ANALYZER strategy THINGS
		//std::string results;

		////2D array
		//std::vector<float> compatibilityTable_LtoL;
		//std::vector<float> compatibilityTable_IQtoQ;
		//std::vector<float> compatibilityTable_QtoQ;
		//std::vector<float> compatibilityTable_IQtoIQ;

		////new analysis measure vectors
		//std::vector<float> compatibilityTable_LtoL_;
		//std::vector<float> compatibilityTable_IQtoQ_;
		//std::vector<float> compatibilityTable_QtoQ_;
		//std::vector<float> compatibilityTable_IQtoIQ_;

		////private helpers
		//void CalculateAnalysisMeasures(const int calculator_index_i, const int calculator_index_j, const int sequence_set_size, const int batch_id);
		//void InitializeCompatibilityVectors(const int sequence_count);

	};
}

#endif


