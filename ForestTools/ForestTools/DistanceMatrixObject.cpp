/******************************************************************************
DeAngelo Wilson
January 3 2020*

						DistanceMatrixObject
******************************************************************************/

#include "DistanceMatrixObject.h"

//#include "FileObject.h"
#include "DistanceMeasureCalculator.h"
#include <fstream>

//debugging timer
//#include <time.h>

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
	}

	DistanceMatrixObject::DistanceMatrixObject(std::string sequence_names_dir, std::string sequences_dir, DistanceMeasureCalculator* dmc):
	//pResults(nullptr),
	//pQuartetResults(nullptr),
	fileObjectManager(sequence_names_dir, sequences_dir),
	//results(std::to_string(sequenceCount).append("\n")),
	distanceMeasureFunc(dmc)
	{
		//printf("dmo constructed\n");
	}

	//legacy:: name --> batch_calculation.. (computes matrix --> tree) on all sets of 'sequenceLists' file
	void DistanceMatrixObject::batch_matrix_calculation(const std::string& sequences_list_dir)
	{

		//open file
		std::ifstream fastaInput(sequences_list_dir);

		if (!fastaInput.is_open())
		{
			printf("File at path: %s - could not be opened\nBatch matrix creation can not be executed\n", sequences_list_dir.c_str());
		}
		else
		{
			//read file and create matrixes
			std::string line;
			int batch_count = 0;
			//while more matrix/tree - sets
			while(std::getline(fastaInput, line))
			{
				//return vector of sequence names
				const std::vector<std::string> sequence_set_names = ProcessSequenceSet(line);

				//function delegated to specific DMO calculator (strategy matrix calculator)
					//CALCULATES DISTANCE MATRIX (if necessary) --> tree out files for current sequence set
				this->distanceMeasureFunc->calculate_and_output_matrix(this->fileObjectManager, sequence_set_names, batch_count);// *** output tree files ***
					//note::multithreaded -- thread pool on function/functor^
						//output associated with thread_id
						
					//mrbayes does not need create_tree... call in derived calcs
				//this->distanceMeasureFunc->create_tree(sequence_set_names, batch_count);
				
				batch_count++;
			}
			
		}
		
	}

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
	const std::string DistanceMatrixObject::swap_underscores(const std::string& str) const
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


