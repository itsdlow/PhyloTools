/**************************************************************************************************************************
DeAngelo Wilson
March 24 2020

					SequenceListsGenerator
**************************************************************************************************************************/

#include "SequenceListsGenerator.h"

#include "SystemParameters.h"
#include "DistanceMeasureCalculator.h"


std::string distanceMeasure::SequenceListsGenerator::GenerateSequenceListFile(const std::vector<std::string>& names)
{
	//for each original file object -- write to line
	return WriteSequenceListsFile(CreateSequenceListString(names));
}

//given master set of sequence names -- generates all subsets -- writes to sequenceList file
std::string distanceMeasure::SequenceListsGenerator::GenerateBatchSequenceListsFile(const std::vector<std::string>& names)
{
	std::string file_string;
		//LARGE - TREES
	RecursivelyGenerateSequenceListSubset(names, file_string, 0, SystemParameters::GetSubsetSizeLarge(), SystemParameters::GetSubsetCountLarge(), SystemParameters::GetSubsetCountRatioLarge());
		//SMALL - TREES
	RecursivelyGenerateSequenceListSubset(names, file_string, 0, SystemParameters::GetSubsetSizeSmall(), SystemParameters::GetSubsetCountSmall(), SystemParameters::GetSubsetCountRatioSmall());
		//QUARTET - TREES
	const int totalQuartets = DistanceMeasureCalculator::GetQuartetCombinations( static_cast<int>(names.size()) );
	RecursivelyGenerateSequenceListSubset(names, file_string, 0, SystemParameters::GetMinSequenceListSize(), totalQuartets, 1);


	return WriteSequenceListsFile(file_string);
}

void distanceMeasure::SequenceListsGenerator::RecursivelyGenerateSequenceListSubset(const std::vector<std::string>& names, std::string& file_string, int index, int size, int maxCount, int subsetCountRatio)
{
	//reference vars for recursive func
	bool done_flag = false;
	int count = 0;
	//while subset_count not reached (128) -- pick subset size (12) indexes and create SequenceList
	std::vector<std::string> subset_names;
	std::string subset_size_sequence_lists;
	printf("New Subset:\n");
	while(count != maxCount)
	{
		printf("\tRecursively GeneratingSequenceListSubsetHelper\n");

		//create subset of names array (count++ fileString written) + write sequence_set to fileString
		//append to string fileString subsets of XX size  (IN SYSTEMS PARAMETERS)
		RecursivelyGenerateSequenceListSubsetHelper(names, subset_size_sequence_lists, done_flag, subset_names, 0, size, count, maxCount, subsetCountRatio);
	}
	file_string.append(subset_size_sequence_lists);
}

void distanceMeasure::SequenceListsGenerator::RecursivelyGenerateSequenceListSubsetHelper(const std::vector<std::string>& names, std::string& file_string, bool& done, std::vector<std::string>& subset_names, int index, int size, int& count, int maxCount, int subsetCountRatio)
{
	//subset finished...
	if(static_cast<int>(subset_names.size()) == size)
	{
		//RANDOMNESS -- NOTE:: wastes time
		//if((rand() % (max_count - count)) + 1 == 1)
		if (rand() % subsetCountRatio == 0)
		{
			const std::string temp = CreateSequenceListString(subset_names);
			//check if duplicate combination
			if(file_string.find(temp))
			{
				//write subset names...
				//std::string temp = CreateSequenceListString(subset_names);
				file_string.append(temp);
				count++;
			}
	
		}
		

		return;
	}
	if (count == maxCount)
	{
		//done done...
		//set flag...
		done = true;
		return;
	}

	for (int i = index; i < static_cast<int>(names.size()); i++)
	{
		//check -- promotes early backtracking
		if(!done)
		{
			subset_names.push_back(names.at(i));
 			RecursivelyGenerateSequenceListSubsetHelper(names, file_string, done, subset_names, i + 1, size, count, maxCount, subsetCountRatio);
			subset_names.pop_back();
		}

	}
	//clear all 
	//return std::string();
}

std::string distanceMeasure::SequenceListsGenerator::CreateSequenceListString(const std::vector<std::string>& names)
{
	std::string str(std::to_string(names.size()));
	for (auto it = names.begin(); it != names.end(); it++)
	{
		std::string name = *it;
		DistanceMeasureCalculator::swap_space_with_underscores(name);
		str.append(" " + name);
	}
	str.append("\n");
	
	return str;
}

//write given string to file
std::string distanceMeasure::SequenceListsGenerator::WriteSequenceListsFile(const std::string& str)
{
	std::string filename = SystemParameters::GetSequenceListsFileString();
	
	FILE* pSequenceLists = fopen(filename.c_str(), "w");
	if (pSequenceLists != nullptr)
	{
		size_t numBytesWritten = fwrite(str.c_str(), str.length(), 1, pSequenceLists);
		fclose(pSequenceLists);
		//reset string for next batch
		//this->results.clear();
	}
	return filename;
}