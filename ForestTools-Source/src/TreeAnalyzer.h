/**************************************************************************************************************************
DeAngelo Wilson
February 26 2020

					TreeAnalyzer
						-- Perform analysis of trees for BatchDistanceCalculators
							--Compute Compatibilities for permutations of calculator_methods
								--outputs files to ../ForestFiles/Analysis/..
**************************************************************************************************************************/

#ifndef _TreeAnalyzer
#define _TreeAnalyzer

#include <string>
#include <vector>

namespace distanceMeasure
{
	//forward declarations
	class DistanceMeasureCalculator;
}

namespace phylo
{


	class TreeAnalyzer
	{
	public:
		TreeAnalyzer() = delete;
		TreeAnalyzer(distanceMeasure::DistanceMeasureCalculator* const* pCalculators, const int calculator_count);

		virtual ~TreeAnalyzer() = default;
		TreeAnalyzer(const TreeAnalyzer& bca) = delete;
		TreeAnalyzer& operator=(const TreeAnalyzer& bca) = delete;

		//void analyze_trees(const std::vector<std::string>& tree_paths);

		//called by BatchCalculators
		//void WriteLargeTreeAnalysisTables(const int tree_count);
		//void WriteBatchAnalysisTables(const int tree_count);

		//void WriteQuartetsAnalysisTables();

		//void SetCalculatorsArray(DistanceMeasureCalculator* const* calcs) { this->pCalculators = calcs; };
		//void CalculateLargeTreeAnalysisMeasures(const int index_i, const int index_j, const std::string& tree_path_i, const std::string& tree_path_j, const int tree_count);
		//calcualtes Large tree <-> large tree  +  quartets <-> large tree/quartets
		//void CalculateQuartetsAnalysisMeasures(const std::string& large_tree_path_i, const std::string& large_tree_path_j, const std::string& quartet_trees_path_i, const std::string& quartet_trees_path_j, const int leaf_count);

		virtual void CalculateAnalysisMeasures(const int calculator_index_i, const int calculator_index_j, const int sequence_set_size, const int batch_id) = 0;
		virtual void InitializeCompatibilityVectors(const int table_dimension) = 0;
		virtual void WriteAnalysisTables(const int ss_size_count, const int ss_size) = 0;

	protected:
		//ptr to array of pointers, which hold constant calculators
			//CANNOT change calculator elements
		distanceMeasure::DistanceMeasureCalculator* const* pCalculators;
		const int calculator_count;
		
		std::string results;

		const int standard_label_size = 6;

		std::string GetStandardizedCalculatorLabels() const;
		std::string StandardizeCalculatorLabel(const int index) const;
		void WriteAnalysisTable(const std::vector<float>& table_vector, const std::string& table_description, const int ss_size_count);

	private:

	};
}

#endif


