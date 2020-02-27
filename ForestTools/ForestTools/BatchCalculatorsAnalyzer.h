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


namespace distanceMeasure
{
	//forward declarations
	class DistanceMeasureCalculator;
	
	class BatchCalculatorsAnalyzer
	{
	public:
		BatchCalculatorsAnalyzer() = delete;
		BatchCalculatorsAnalyzer(const int calculator_count);

		~BatchCalculatorsAnalyzer() = default;
		BatchCalculatorsAnalyzer(const BatchCalculatorsAnalyzer& bca) = delete;
		BatchCalculatorsAnalyzer& operator=(const BatchCalculatorsAnalyzer& bca) = delete;

		void batch_analyze_sequence_set(const std::vector<std::string>& sequence_set_names, const int batch_id);

		//called by BatchCalculators
		void WriteAnalysisTables();
		void SetCalculatorsArray(DistanceMeasureCalculator* const* calcs) { this->pCalculators = calcs; };
		
	private:
		const int calculator_count;
		int previous_ss_size = 0;
		int ss_size_count = 0;

		//ptr to array of pointers, which hold constant calculators
			//CANNOT change calculator elements
		DistanceMeasureCalculator* const* pCalculators;

		std::string results;

		//2D array 
		std::vector<float> compatibilityTable_LtoL;
		std::vector<float> compatibilityTable_IQtoQ;
		std::vector<float> compatibilityTable_QtoQ;
		std::vector<float> compatibilityTable_IQtoIQ;

		//private helpers
		void CalculateAnalysisMeasures(const int calculator_index_i, const int calculator_index_j, const int sequence_set_size, const int batch_id);
		void InitializeCompatibilityVectors(const int sequence_count);
		void WriteAnalysisTable(const std::vector<float>& table_vector, const std::string& table_description);

		void GetAnalysisTableFilePath(char* buffer, const size_t buffer_size, const int sequence_count) const;

		//duplicate code...
		//static int getArrayIndex(int row, int col, int rowCount) { return (row * rowCount) + col; }

		
	};
}

#endif


