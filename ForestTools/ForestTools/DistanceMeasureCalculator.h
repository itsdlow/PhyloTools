/******************************************************************************
DeAngelo Wilson
January 3 2020

						DistanceMeasureCalculator (strategy -- base)
******************************************************************************/
#ifndef _DistanceMeasureCalculator
#define _DistanceMeasureCalculator

//#include "FileObject.h"

#include <string>
#include <vector>

namespace distanceMeasure
{
	//forward declarations
	class FileObjectManager;
	class FileObject;
	
	class DistanceMeasureCalculator
	{
	public:
		//BIG 4
		DistanceMeasureCalculator() = default;
		DistanceMeasureCalculator(const DistanceMeasureCalculator&) = delete;
		DistanceMeasureCalculator& operator=(const DistanceMeasureCalculator&) = delete;
		virtual ~DistanceMeasureCalculator() = default;

		//main calculator driver func -- output matrix as text-file
			//calculate_sequence_set_trees()
		virtual void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id) = 0;
		//internal calc specific
		//virtual void create_tree(const std::vector<std::string>& sequence_set_names, const int batch_id) = 0;
		//virtual void batch_analyze_sequence_set(const std::vector<std::string>& sequence_set_names, const int batch_id);

		
		//CALLED BY DistanceMatrixCalculator
		//opens log file -- for derived calc
		virtual void InitializeSequenceSetTimingsLog();
		//write total calc time to log FILE --> closes file
		virtual void LogTotalCalculationTime();
		
		//PRIVATE???
		double GetCalculationTime() const { return this->calculationTime; };
		double GetTotalCalculationTime() const { return this->totalCalculationTime; };


		virtual void GetLargeListMatrixFileName(char* buffer, const size_t buffer_size, const int batch_number, const size_t sequence_count) final;
		virtual void GetLargeListTreeFileName(char* buffer, const size_t buffer_size, const int batch_number, const size_t sequence_count) final;

		virtual void GetQuartetsMatrixFileName(char* buffer, const size_t buffer_size, const int batch_number, const size_t sequence_count) final;
		virtual void GetQuartetsTreeFileName(char* buffer, const size_t buffer_size, const int batch_number, const size_t sequence_count) final;

		//called by Get_Matrix/Tree_FileNames + BatchCalculatorsAnalyzer
		virtual std::string GetCalculatorName() const = 0;
		
		//convert 2d-array index into 1d index
		static int getArrayIndex(int row, int col, int rowCount) { return (row * rowCount) + col; }
	protected:
		FILE* pTimingsLogFile;

		

		//helpers

		static int GetQuartetCombinations(int n);
		static void swap_space_with_underscores(std::string& description_string);

		//debug - timings helpers
		void StartCalculationTimer();
		void StopCalculationTimer(int batchID, const std::string& sequenceSet);


	private:
		
		//debug timing things
		double startTime;
		double calculationTime;
		double totalCalculationTime = 0;


		//CALLED BY StopCalculationTimer()
		void LogSequenceSetTiming(int batchID, double calculationTime, const std::string& sequenceSet) const;

	};
}


#endif // !_DistanceMeasureCalculator
