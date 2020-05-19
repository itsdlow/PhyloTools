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


namespace phylo
{
	//forward declarations
	//class DistanceMeasureCalculator;

	class TreeAnalyzer
	{
	public:
		TreeAnalyzer() = default;
		//TreeAnalyzer(const int calculator_count);

		~TreeAnalyzer() = default;
		TreeAnalyzer(const TreeAnalyzer& bca) = delete;
		TreeAnalyzer& operator=(const TreeAnalyzer& bca) = delete;

		void analyze_trees(const std::vector<std::string>& tree_paths);

		//called by BatchCalculators
		void WriteLargeTreeAnalysisTables(const int tree_count);
		void WriteBatchAnalysisTables(const int tree_count);

		//void WriteQuartetsAnalysisTables();

		//void SetCalculatorsArray(DistanceMeasureCalculator* const* calcs) { this->pCalculators = calcs; };
		void CalculateLargeTreeAnalysisMeasures(const int index_i, const int index_j, const std::string& tree_path_i, const std::string& tree_path_j, const int tree_count);
		//calcualtes Large tree <-> large tree  +  quartets <-> large tree/quartets
		//void CalculateQuartetsAnalysisMeasures(const std::string& large_tree_path_i, const std::string& large_tree_path_j, const std::string& quartet_trees_path_i, const std::string& quartet_trees_path_j, const int leaf_count);

		
	private:
		//const int tree_count;
		//int previous_ss_size = 0;
		//int ss_size_count = 0;

		//make standard label size --> size of max tree_label... (max==20?)
		const int standard_label_size = 6;
		//ptr to array of pointers, which hold constant calculators
			//CANNOT change calculator elements
		//DistanceMeasureCalculator* const* pCalculators;

		std::string results;

		//2D array
		std::vector<float> compatibilityTable_LtoL;
		std::vector<float> compatibilityTable_IQtoQ;
		std::vector<float> compatibilityTable_QtoQ;
		std::vector<float> compatibilityTable_IQtoIQ;

		//new analysis measure vectors
		std::vector<float> compatibilityTable_LtoL_;
		std::vector<float> compatibilityTable_IQtoQ_;
		std::vector<float> compatibilityTable_QtoQ_;
		std::vector<float> compatibilityTable_IQtoIQ_;

		//private helpers
		//void CalculateAnalysisMeasures(const int calculator_index_i, const int calculator_index_j, const int sequence_set_size, const int batch_id);
		void InitializeCompatibilityVectors(const int table_dimension);
		void WriteAnalysisTable(const std::vector<float>& table_vector, const std::string& table_description, const int table_dimension, const int ss_size_count = 1);

		void GetAnalysisTableFilePath(char* buffer, const size_t buffer_size, const int tree_count) const;
		void GetBatchAnalysisTableFilePath(char* buffer, const size_t buffer_size, const int sequence_count) const;

		std::string GetStandardizedCalculatorLabels() const;
		std::string StandardizeCalculatorLabel(const int index) const;
		//duplicate code...
		//static int getArrayIndex(int row, int col, int rowCount) { return (row * rowCount) + col; }


	};
}

#endif


