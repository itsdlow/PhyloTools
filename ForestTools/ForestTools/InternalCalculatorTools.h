/******************************************************************************
DeAngelo Wilson
January 18 2020

			InternalCalculatorTools (strategy -- derived-base)
					--DistanceMatrix created by hand
******************************************************************************/
#ifndef _InternalCalculatorTools
#define _InternalCalculatorTools

#include <string>
#include <vector>

namespace distanceMeasure
{
	class FileObject;
	class FileObjectManager;
	class DistanceMeasureCalculator;
	
	class InternalCalculatorTools
	{
	public:
		//BIG 4
		InternalCalculatorTools() = default;
		InternalCalculatorTools(const InternalCalculatorTools&) = delete;
		InternalCalculatorTools& operator=(const InternalCalculatorTools&) = delete;
		~InternalCalculatorTools() = default;

		//virtual float calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const = 0;
		//virtual float normalize(int differenceCount, int sequencesize) const = 0;

		//driver funcs
		void CalculateDistanceMeasuresAndTrees(DistanceMeasureCalculator* dmc, FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id);
		
		//void operator()();
		static void create_large_tree(DistanceMeasureCalculator* dmc, const std::vector<std::string>& sequence_set_names, const int batch_id);
		static void create_quartet_trees(DistanceMeasureCalculator* dmc, const std::vector<std::string>& sequence_set_names, const int batch_id);


		//internal calc specific funcs
					//fill results buffer with distance Measures + LAMDAMATRIX for allquartetsMethod
		void CalculateLargeTreeDistanceMeasures(DistanceMeasureCalculator* dmc, FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names);
		static void CalculateAllQuartetsDistanceMeasures(DistanceMeasureCalculator* dmc, FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names);

		//virtual void write_quartet_matrix(FileObjectManager& fileObjectManager, const std::vector<int>& speciesSequenceSetIndexes, const std::vector<std::string>& sequence_set_names, const int fileCount);

		//write results buffer to output FILE, closes FILE
		//void write_batch_results(DistanceMeasureCalculator* dmc, const int batch_number, const size_t sequence_count);
		void write_large_tree_results(DistanceMeasureCalculator* dmc, const int batch_number, const size_t sequence_count);
		void write_quartets_results(DistanceMeasureCalculator* dmc, const int batch_number, const size_t sequence_count);

		void AppendToQuartetResults(const std::string& res);

		float GetLamdaMatrixDistanceAt(int pos) const;
		
	private:
		//PUBLIC INTERNAL CALCULATOR MEMBERS
		FILE* pResults = nullptr;
		FILE* pQuartetResults = nullptr;

		std::string results;
		std::string quartetResults;

		//2D array (distanceMATRIX) of floats
		std::vector<float> lamdaMatrix;

		//void GetQuartetsFastMECommand(char* quartets_buffer, const size_t buffer_size, char* input, int count, char* output) const;



	};
}


#endif // !_InternalCalculatorTools
