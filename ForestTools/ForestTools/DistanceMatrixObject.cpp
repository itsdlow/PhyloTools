/******************************************************************************
DeAngelo Wilson
January 3 2020*

						DistanceMatrixObject
								BATCH_DISTANCE_CALCULATOR_OBJECT
									given construction params
										--> produces Trees for distanceMeasureCalculator + analyzes tree if BATCH
******************************************************************************/

#include "DistanceMatrixObject.h"

//#include "FileObject.h"
#include "DistanceMeasureCalculator.h"
//#include "PhyloAnalysis.h"

#include <fstream>
//debugging timer
//#include <time.h>
#include <ctime>

//needed for Sequence list file generation 
#include "ForestPlug.h"
#include "SystemParameters.h"

namespace distanceMeasure
{
	//DistanceMatrixObject::DistanceMatrixObject()
	//{
	//	//do nothing
	//}


	DistanceMatrixObject::~DistanceMatrixObject()
	{
		//this->writeResults();
		delete this->distanceMeasureFunc;
		//delete this->poRunFlags;
	}

	//DistanceMatrixObject::DistanceMatrixObject(SequenceNamesStrategy* names_strategy, InputSequencesFormatType format, std::string sequences_dir, DistanceMeasureCalculator* dmc):
	////pResults(nullptr),
	////pQuartetResults(nullptr),
	////pTimesLogFile(nullptr),
	//distanceMeasureFunc(dmc),
	////poRunFlags(dmc->GetCalculatorFlags()),
	//fileObjectManager(names_strategy, format, sequences_dir)//TODO:: tell FOM format of "sequences_dir"... (FASTA || NEXUS)
	////results(std::to_string(sequenceCount).append("\n")),
	//{
	//	//printf("dmo constructed\n");
	//}
	
	distanceMeasure::DistanceMatrixObject::DistanceMatrixObject(DistanceMeasureCalculator* dmc):
		distanceMeasureFunc(dmc),
		fileObjectManager()
	{
	}

	
	void distanceMeasure::DistanceMatrixObject::run(std::vector<InputSequenceFileSet> batch_files)
	{		
		for (InputSequenceFileSet inputFileSet: batch_files)
		{
			this->fileObjectManager.Initialize(inputFileSet.namesStrategy, inputFileSet.formatting, inputFileSet.path);
		
			this->fileObjectManager.InitializeFileObjects();

			//Create sequence list file (if needed)
			ForestPlug::SetSequenceListsFile(inputFileSet.sequenceListBatchFlag, *this, inputFileSet.sequenceListPath);
			
			//driver function of DMO
			this->batch_matrix_calculation(inputFileSet.sequenceListPath);

			//clean FOM resources...
			this->fileObjectManager.Terminate();

			//update current batch number
			SystemParameters::IncrementCurrentFileSetBatchNumber();
			//clear temp files...
			ForestPlug::ClearTempFiles();
		}

	}
	
	//legacy:: name --> batch_calculation.. (computes matrix --> tree) on all sets of 'sequenceLists' file
	void DistanceMatrixObject::batch_matrix_calculation(const std::string& sequences_list_dir)
	{

		//open file
		std::ifstream fastaInput(sequences_list_dir);

		if (!fastaInput.is_open())
		{
			printf("SequenceList File at path: %s - could not be opened\nBatch matrix creation can not be executed\n", sequences_list_dir.c_str());
		}
		else
		{
			//read file and create matrixes
			std::string line;
			int batch_count = 0;
			
			//opens log file(s)
			this->distanceMeasureFunc->InitializeSequenceSetTimingsLog(this->fileObjectManager.get_file_count());
			//while more matrix/tree - sets
			while(std::getline(fastaInput, line))
			{				
				//return vector of sequence names
				const std::vector<std::string> sequence_set_names = ProcessSequenceSet(line);
				
				//function delegated to specific DMO calculator (strategy matrix calculator)
					//CALCULATES DISTANCE MATRIX (if necessary) --> tree out files for current sequence set
				this->distanceMeasureFunc->calculate_and_output_matrix(this->fileObjectManager, sequence_set_names, line, batch_count++);// *** output tree files ***
					//note::multithreaded -- thread pool on function/functor^
						//output associated with thread_id

				//batch_count++;
			}
			//Alert distanceMeasureFunc --> done processes sequence Sets
				//Log (write) total calcaulation time + close file
			this->distanceMeasureFunc->LogTotalCalculationTime();
		}
		
	}

	//void distanceMeasure::DistanceMatrixObject::batch_analyze_trees()
	//{
	//	//
	//	
	//}

	//REMOVEs UNDERSCORES " _ " from sequence_set -- appends all names to vector - returns
	const std::vector<std::string> distanceMeasure::DistanceMatrixObject::ProcessSequenceSet(const std::string& sequence_set) const
	{
		//get number of sequences
		std::vector<std::string> sequence_names;
		
		std::size_t start;
		const int sequence_count = std::stoi(sequence_set, &start);
		//skip space after sequence_count
		start++;
		sequence_names.reserve(sequence_count);
		
		int count = 0;
		std::size_t next;
		//add sequences to vector - return
		while(std::string::npos != (next = sequence_set.find_first_of(' ', start)) )
		{
			sequence_names.push_back(swap_underscores(sequence_set.substr(start, next-start)) );
			//sequence_names.push_back( sequence_set.substr(start, next - start) );
			start = next + 1;
			count++;
		}
		//add last sequence_name
		sequence_names.push_back(swap_underscores(sequence_set.substr(start)) );
		//sequence_names.push_back(sequence_set.substr(start));
		count++;

		
		if(count != sequence_count)
		{
			printf("Sequence Listing format incorrect....\n\n\n\n");
		}
		
		return sequence_names;
	}
	//given line with 0 - N underscores -- swap underscore with spaces
	std::string DistanceMatrixObject::swap_underscores(const std::string& str) const
	{
		std::string res = str;
		for(auto it = res.begin(); it != res.end(); it++)
		{
			//swap underscore for space
			if(*it == '_')
			{
				*it = ' ';
			}
		}
		
		return res;
	}

	//accessor methods
	DistanceMeasureCalculator* DistanceMatrixObject::getDistanceMeasureFunc() const
	{
		return this->distanceMeasureFunc;
	}

	const FileObjectManager& DistanceMatrixObject::getFileObjectManager() const
	{
		return this->fileObjectManager;
	}

}


