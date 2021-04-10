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
#include "RunFlags.h"
#include <ctime>

namespace distanceMeasure
{
	//forward declarations
	class FileObjectManager;
	class FileObject;
	
	class DistanceMeasureCalculator
	{
	public:
		//BIG 4
		//DistanceMeasureCalculator() = delete;
		//DistanceMeasureCalculator();
		DistanceMeasureCalculator(RunFlags* flags, const std::string& name);
		
		DistanceMeasureCalculator(const DistanceMeasureCalculator&) = delete;
		DistanceMeasureCalculator& operator=(const DistanceMeasureCalculator&) = delete;
		virtual ~DistanceMeasureCalculator() = default;

		//main calculator driver func -- output matrix as text-file
			//calculate_sequence_set_trees()
		virtual void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id) = 0;
		//called by Get_Matrix/Tree_FileNames + BatchCalculatorsAnalyzer
		//virtual std::string GetCalculatorName() const = 0;
		std::string GetCalculatorName() const { return this->name; };


		//internal calc specific
		virtual float calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const = 0;
		virtual float normalize(int differenceCount, int sequencesize) const = 0;
		virtual void write_quartet_matrix(FileObjectManager& fileObjectManager, const std::vector<int>& speciesSequenceSetIndexes, const std::vector<std::string>& sequence_set_names, const int fileCount, const int quartet_count) = 0;

		
		//CALLED BY DistanceMatrixCalculator
		//opens log file -- for derived calc
		virtual void InitializeSequenceSetTimingsLog(const int total_sequence_count);
		//write total calc time to log FILE --> closes file
		virtual void LogTotalCalculationTime();

		
		//PRIVATE???
		double GetCalculationTime() const { return this->calculationTime; };
		double GetTotalCalculationTime() const { return this->totalCalculationTime; };


		
		
		void GetLargeListMatrixFileName(char* buffer, const int batch_number, const size_t sequence_count) const;
		virtual void GetLargeListTreeFileName(char* buffer, const int batch_number, const size_t sequence_count) const;
		void GetClusteredMatrixFileName(char* buffer, const int batch_number, const size_t sequence_count) const;
		void GetClusteredTreeFileName(char* buffer, const int batch_number, const size_t sequence_count) const;

		
		void GetQuartetsMatrixFileName(char* buffer, const int batch_number, const size_t sequence_count) const;
		void GetQuartetsTreeFileName(char* buffer, const int batch_number, const size_t sequence_count) const;
		void GetFastMECommand(char* buffer, char* input, int count, char* output) const;

		void GetClosenessLimitLogFileName(char* buffer, const int batch_number, const size_t sequence_count) const;

			
		//convert 2d-array index into 1d index
		static int getArrayIndex(int row, int col, int rowCount) { return (row * rowCount) + col; }
		static int GetQuartetCombinations(int n);
		static int GetCombinations(int n, int r);
		static void swap_space_with_underscores(std::string& description_string);

		const RunFlags* GetCalculatorFlags() const { return this->pFlags; };
		bool GenerateQuartetsFlag() const { return this->pFlags->generate_quartets; };
		float GetClosenessFactor() const { return this->pFlags->closeness_factor; };
		
	protected:
		FILE* pTimingsLogFile = nullptr;

		//Calculator Run Flags:
		RunFlags* pFlags;
		
		//debug timing things
		//clock_t startTime = 0;
		double startTime = 0;

		double calculationTime = 0;
		double totalCalculationTime = 0;

		std::string name;
		
		//helpers
		static std::vector<std::string> CreateSubsequenceSet(const std::vector<std::string>& sequence_set_names, const std::vector<int>& subSequenceSetIndexes);
		static std::string CreateSubsequenceSetString(const std::vector<std::string>& subsequence_set_names);

		//debug - timings helpers
		virtual void StartCalculationTimer();
		virtual void StopCalculationTimer(int batchID, const std::string& sequenceSet);


		//CALLED BY StopCalculationTimer()
		void LogSequenceSetTiming(int batchID, double calculationTime, const std::string& sequenceSet) const;
	private:
		

	};
}


#endif // !_DistanceMeasureCalculator
