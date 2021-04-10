/******************************************************************************
DeAngelo Wilson
January 18 2020

						IntneralDistanceMEasureCalculator (derived-base)
******************************************************************************/


#include "InternalCalculatorTools.h"

#include "SystemParameters.h"
#include "DistanceMeasureCalculator.h"
#include "FileObjectManager.h"
//#include <set>
//used for FLT_MAX/MIN
#include <cfloat>

void distanceMeasure::InternalCalculatorTools::CalculateDistanceMeasuresAndTrees(DistanceMeasureCalculator* dmc, FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)
{
	//if calculating -- Large Tree + Quartet Trees
		//open quartets file for writing (allows for appending chunks of quartetResults)
			//fopen_s(&this->pQuartetResults, quartet_matrices_file_path, "w");
		//calculateLarge + calulcateAllQuartets
		//write

	//calculate LargeTree giving sequence_names_list
	this->CalculateLargeTreeDistanceMeasures(dmc, fileObjectManager, sequence_set_names);

	/***************************************************************************************************************************
	//TODO::ADD CLUSTERING (should only be enabled when dealing with all InternalCalculators (or non-batchCalculator->Internal)
		//check this->lamda
			//look for any pairwise distances that are very close (0.0 - 0.???)
		//NOT IMPLEMENTED:: strict -- loose -- none -------> clustering
		//determine "closeness-limit" based on min-max pairwise distance (for each species (species1 -- speciesX) )

		//CLUSTERING FILTER -- creates an additional tree with like-sequences clustered into 1
	****************************************************************************************************************************/
	//Get closeness-limit for each sequence -- check if species "too" close
				//------> remove "other" species from matrix???

	//iterate through lambda matrix
		//get min and max pairwise distance for -- 1 species matrix line
			//calc "closeness-limit" (closeness_factor * max_pairwise_distance_diff)
		//reiterate through same line of lamda -- check against closeness_limit -- add index of (species, species_to_remove) pair
			//---> add to std::set --> supply compare function...
			//NOTE:: add pair --> no duplicates allowed i.e. --> (species_0 <--> species_3) + (species_3 <-> species_0)
	//if strict || loose clustering
	if(dmc->GetClosenessFactor() > 0)
	{
		this->CalculateClusteredTreeDistanceMeasures(dmc, sequence_set_names, batch_id);

		this->write_clustered_tree_results(dmc, batch_id, sequence_set_names.size());
		InternalCalculatorTools::create_clustered_tree(dmc, sequence_set_names, batch_id);
	}

	

		
	//NOTE: MUST - change sequence_set_names to reflect removed sequences
	if(dmc->GenerateQuartetsFlag())
	{
		//calculate quartets tree matrix
		InternalCalculatorTools::CalculateAllQuartetsDistanceMeasures(dmc, fileObjectManager, sequence_set_names);
		
		//write quartet matrices and run FASTME
		this->write_quartets_results(dmc, batch_id, sequence_set_names.size());
		InternalCalculatorTools::create_quartet_trees(dmc, sequence_set_names, batch_id);
	}
	//Write large tree matrix and run FASTME
	this->write_large_tree_results(dmc, batch_id, sequence_set_names.size());
	InternalCalculatorTools::create_large_tree(dmc, sequence_set_names, batch_id);
}


void distanceMeasure::InternalCalculatorTools::create_large_tree(DistanceMeasureCalculator* dmc, const std::vector<std::string>& sequence_set_names, const int batch_id)
{
	//get batch_ID_matrix FILE (quartets + LargeList)
		//feed to fastme
		//********************************************
	const int sequence_set_count = static_cast<int>(sequence_set_names.size());
	//Get current_sequence-set_matrix files
	char largelist_matrix_file_path[100];
	dmc->GetLargeListMatrixFileName(largelist_matrix_file_path, batch_id, sequence_set_count);

	//get path for new_tree files
	char large_tree_file_path[150];
	dmc->GetLargeListTreeFileName(large_tree_file_path, batch_id, sequence_set_count);

	char fastme_command[200];
	dmc->GetFastMECommand(fastme_command, largelist_matrix_file_path, 1, large_tree_file_path);

	//TODO:: store return value of system call --> check if command was sucessful
		//==> IF NOT -> try re-execute once?
	system(fastme_command);

	printf("FastME command executed -- Large Tree Generated\n");
}
void distanceMeasure::InternalCalculatorTools::create_quartet_trees(DistanceMeasureCalculator* dmc, const std::vector<std::string>& sequence_set_names, const int batch_id)
{
	//get batch_ID_matrix FILE (quartets + LargeList)
		//feed to fastme
		//********************************************
	const int sequence_set_count = static_cast<int>(sequence_set_names.size());
	//Get current_sequence-set_matrix files
	char quartet_matrices_file_path[100];
	dmc->GetQuartetsMatrixFileName(quartet_matrices_file_path, batch_id, sequence_set_count);

	char quartet_trees_file_path[150];
	dmc->GetQuartetsTreeFileName(quartet_trees_file_path, batch_id, sequence_set_count);

	//sequence_set_size choose 4
	const int quartetCount = DistanceMeasureCalculator::GetQuartetCombinations(sequence_set_count);

	char fastme_quartets_command[200];
	dmc->GetFastMECommand(fastme_quartets_command, quartet_matrices_file_path, quartetCount, quartet_trees_file_path);

	system(fastme_quartets_command);

	printf("FastME command executed -- Quartet Trees Generated\n");
}
void distanceMeasure::InternalCalculatorTools::create_clustered_tree(DistanceMeasureCalculator* dmc, const std::vector<std::string>& sequence_set_names, const int batch_id)
{
	//get batch_ID_matrix FILE (quartets + LargeList)
		//feed to fastme
		//********************************************
	const int sequence_set_count = static_cast<int>(sequence_set_names.size());
	//Get current_sequence-set_matrix files
	char clustered_matrix_file_path[200];
	dmc->GetClusteredMatrixFileName(clustered_matrix_file_path, batch_id, sequence_set_count);

	//get path for new_tree files
	char clustered_tree_file_path[200];
	dmc->GetClusteredTreeFileName(clustered_tree_file_path, batch_id, sequence_set_count);

	char fastme_command[200];
	dmc->GetFastMECommand(fastme_command, clustered_matrix_file_path, 1, clustered_tree_file_path);

	system(fastme_command);

	printf("FastME command executed -- Clustered Tree Generated\n");
}

//calculate LargeTree (w/o quartets) Distance Matrix -- phylib format
void distanceMeasure::InternalCalculatorTools::CalculateLargeTreeDistanceMeasures(DistanceMeasureCalculator* dmc, FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names)
{
	const size_t fileCount = sequence_set_names.size();

	this->results.append(std::to_string(fileCount).append("\n"));

	for (auto i = 0u; i < fileCount; i++)
	{
		//this->results.append(pCurrentFileObject->GetFileName());
		std::string name = sequence_set_names.at(i);
		//NOTE:: refine so dont need to always swap -- change FOM to store one-word-sequence_names... need both forms (searching FOM --> spaces -- creating output --> one-word)
		DistanceMeasureCalculator::swap_space_with_underscores(name);
		this->results.append(name);

		for (auto j = 0u; j < fileCount; j++)
		{
			//Pvalue, LCS,...
			//NOT IMPLEMENTED:: Guard against not found FileObject (nullptr) -> is possible?
			//pass current (i) fileobject + next (j) fileobject to Distance_Calculator
			float normalizedDistance = dmc->calculate_normalized_distance(*fileObjectManager.GetSequenceSetFileObject(sequence_set_names.at(i)), *fileObjectManager.GetSequenceSetFileObject(sequence_set_names.at(j)));

			//append to distance matrix for quartet calcs
			this->lamdaMatrix.push_back(normalizedDistance);

			//write lcs to results
			this->results.append(" ");
			this->results.append(std::to_string(normalizedDistance));
		}

		this->results.append("\n");
		//NOTE:: CHANGE TO '\r' when other debug print statements removed
		printf("\t%zu %s matrix distances calculated -- %d/%zu row calculations performed...\n", fileCount, dmc->GetCalculatorName().c_str(), static_cast<int>(i) + 1, fileCount);

		//TODO:: (IF NCDCalculator --> NORMALIZE ENTIRE MATRIX BASED ON Min-max values...)
	}
}

//4 POINT CONDITION CHECK --> FIND ALL QUARTETS "TREE (T)" INDUCES
void distanceMeasure::InternalCalculatorTools::CalculateAllQuartetsDistanceMeasures(DistanceMeasureCalculator* dmc, FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names)
{
	const int fileCount = static_cast<int>(sequence_set_names.size());

	if (fileCount == 4)
	{
		//return distanceMatrix -- tree
		printf("CalculateAllQuartetsDistanceMeasures:: Tree is quartet\n");
		return;
	}
	if (fileCount < 4)
	{
		//ERROR
		printf("Sequence set contains less leaves than minimum viable tree (quartet): %d\n", fileCount);
		exit(0);
	}
	const int totalQuartets = DistanceMeasureCalculator::GetQuartetCombinations(fileCount);
	int count = 0;

	const float fraction = .25f;
	//25% of total quartets rounded up
	const int multiple = static_cast<int>(static_cast<float>(totalQuartets) * fraction);
	
	for (int i = 0; i < fileCount; i++)
	{
		//while "atleast" 3 OTHER fileObjects exist
		for (int j = i + 1; j < fileCount; j++)
		{
			for (int k = j + 1; k < fileCount; k++)
			{
				for (int l = k + 1; l < fileCount; l++)
				{
					//vector of quartet indicies
					std::vector<int> indexV{ i,j,k,l };//removed for MAC build

					//TODO:: if totalQuartets > XX --> if count /
					//if currently on a quartile number index of quartet (i.e. 25%, 50%, 75%)
					if(count % multiple == 1)
					{
						//TODO
						//flush quartets string
						printf("FLUSHING 'quartetResult' STRING MEMORY -- writing to file");
					}
					
					//pass FOM in for Pvalue Calc--> does not use lambda matrix
					dmc->write_quartet_matrix(fileObjectManager, indexV, sequence_set_names, fileCount, count++);//appended to quartetsResult
					//count++;
					//NOTE:: CHANGE TO '\r' when other debug print statements removed (mr mrbayes/alignment output interfers)
					printf("%d/%d %s Quartet Matrices calculated...\n", count, totalQuartets, dmc->GetCalculatorName().c_str());
				}
			}
		}
	}

}
void distanceMeasure::InternalCalculatorTools::CalculateClusteredTreeDistanceMeasures(DistanceMeasureCalculator* dmc, const std::vector<std::string>& sequence_set_names, const int batch_id)
{
	//write Large tree matrix -- MINUS "too close" sequences
	this->WriteClusteredMatrixResults(this->GetClusteredRemovableIndexes(dmc, sequence_set_names, batch_id), sequence_set_names);
}

std::set<int> distanceMeasure::InternalCalculatorTools::GetClusteredRemovableIndexes(DistanceMeasureCalculator* dmc, const std::vector<std::string>& sequence_set_names, const int batch_id)
{
	//set of indexes to cluster
//removes .second index of CLuster Pair
	std::set<ClusterPair> index_pair_set;
	//std::set<ClusterPair, ClusterPairCompare> index_pair_set;

	const int fileCount = static_cast<int>(sequence_set_names.size());
	const float closeness_factor = dmc->GetClosenessFactor();
	for (int i = 0; i < fileCount; i++)
	{
		float min = static_cast<float>(FLT_MAX);
		float max = static_cast<float>(FLT_MIN);
		//go through species matrix line -- pairwise distances
			//find max and min pairwise distance
		for (int j = 0; j < fileCount; j++)
		{
			const float pairwise_distance = this->GetLamdaMatrixDistanceAt(DistanceMeasureCalculator::getArrayIndex(i, j, fileCount));
			//check if new min
			if (pairwise_distance > 0.0f && pairwise_distance < min)
			{
				min = pairwise_distance;
			}
			//check if new max
			if (pairwise_distance > max)
			{
				max = pairwise_distance;
			}
		}
		//calculate closeness limit
		const float closeness_limit = closeness_factor * (max - min);
		this->WriteSequenceClosenessLimitLog(sequence_set_names.at(i), closeness_limit);
		for (int j = 0; j < fileCount; j++)
		{
			const float pairwise_distance = this->GetLamdaMatrixDistanceAt(DistanceMeasureCalculator::getArrayIndex(i, j, fileCount));

			//check for indexes of "too close" sequences
			if (pairwise_distance > 0.0f && pairwise_distance < closeness_limit)
			{
				//HACK:: place index pair in same ordering no matter what -- smaller index == .first larger index .second
						//always remove the larger index
					//NOTE::USER DEFINED COMPARATOR (ClusterPairCompare) INVALID...
						//---> supposed to ignore ordering of pair for equality...
				int small_index, large_index;
				if(i < j)
				{
					small_index = i;
					large_index = j;
				}
				else
				{
					small_index = j;
					large_index = i;
				}
				//add to set
				index_pair_set.emplace(small_index, large_index);
			}
		}
	}
	//write closeness limit log file results
	this->WriteClosenessLimitLog(dmc, batch_id, sequence_set_names.size());
	//set -> log search...
	std::set<int> remove_indexes;
	for (auto it = index_pair_set.begin(); it != index_pair_set.end(); it++)
	{
		remove_indexes.insert((*it).second);
	}
	return remove_indexes;
}

void distanceMeasure::InternalCalculatorTools::WriteClusteredMatrixResults(std::set<int>&& remove_indexes, const std::vector<std::string>& sequence_set_names)
{
	const int fileCount = static_cast<int>(sequence_set_names.size());

	//write matrix
	const int cluster_count = fileCount - static_cast<int>(remove_indexes.size());
	this->clusteredResults.append(std::to_string(cluster_count) + '\n');

	//to remove -- itereate through lamda matrix -- writing to results_clustered -- (NO COPYING results)
		//if i == remove_index -- || j == remove_index --> do not write
	for (int i = 0; i < fileCount; i++)
	{
		//write matrix line/entry if... curent index row (i)-- not removed
		if (remove_indexes.count(i) == 0)
		{
			std::string name = sequence_set_names.at(i);
			//NOTE:: refine so dont need to always swap -- change FOM to store one-word-sequence_names... need both forms (searching FOM --> spaces -- creating output --> one-word)
			DistanceMeasureCalculator::swap_space_with_underscores(name);
			//write name of matrix table line
			this->clusteredResults.append(name);

			for (int j = 0; j < fileCount; j++)
			{

				//if current index column (j) not removed
				if (remove_indexes.count(j) == 0)
				{
					const float pairwise_distance = this->GetLamdaMatrixDistanceAt(DistanceMeasureCalculator::getArrayIndex(i, j, fileCount));

					//write lcs to results
					this->clusteredResults.append(" ");
					this->clusteredResults.append(std::to_string(pairwise_distance));
				}
			}
			this->clusteredResults.append("\n");
		}
	}
}

void distanceMeasure::InternalCalculatorTools::WriteSequenceClosenessLimitLog(const std::string& sequence_name, const float closeness_limit)
{	
	this->closeness_limit_log.append(sequence_name + ": " + std::to_string(closeness_limit) + "\n");
}

void distanceMeasure::InternalCalculatorTools::WriteClosenessLimitLog(DistanceMeasureCalculator* dmc, const int batch_number, const size_t sequence_count)
{
	//NOTE:: NEED GUARD (ERROR MESSAGE) FOR Path too large
	char closeness_limit_file_path[200];
	dmc->GetClosenessLimitLogFileName(closeness_limit_file_path, batch_number, sequence_count);

	FILE* pLog = fopen(closeness_limit_file_path, "w");
	//this->pResults = fopen(largetree_filename, "w");
	//this->pQuartetResults = fopen(quartettrees_filename, "w");
	if (pLog != nullptr)
	{
		size_t numBytesWritten = fwrite(this->closeness_limit_log.c_str(), this->closeness_limit_log.length(), 1, pLog);
		printf("%s written...\n", closeness_limit_file_path);
		fclose(pLog);
		//reset string for next batch
		this->closeness_limit_log.clear();
	}
}


void distanceMeasure::InternalCalculatorTools::write_large_tree_results(DistanceMeasureCalculator* dmc, const int batch_number, const size_t sequence_count)
{
	//NOTE:: NEED GUARD (ERROR MESSAGE) FOR Path too large
	char largelist_matrix_file_path[200];
	dmc->GetLargeListMatrixFileName(largelist_matrix_file_path, batch_number, sequence_count);

	this->pResults = fopen(largelist_matrix_file_path, "w");
	//this->pResults = fopen(largetree_filename, "w");
	//this->pQuartetResults = fopen(quartettrees_filename, "w");
	if (this->pResults != nullptr)
	{
		size_t numBytesWritten = fwrite(this->results.c_str(), this->results.length(), 1, this->pResults);
		printf("%s written...\n", largelist_matrix_file_path);
		fclose(this->pResults);
		//reset string for next batch
		this->results.clear();
		this->lamdaMatrix.clear();//NOTE:: problematic? was working before...

	}
}
void distanceMeasure::InternalCalculatorTools::write_quartets_results(DistanceMeasureCalculator* dmc, const int batch_number, const size_t sequence_count)
{
	char quartet_matrices_file_path[200];
	dmc->GetQuartetsMatrixFileName(quartet_matrices_file_path, batch_number, sequence_count);

	this->pQuartetResults = fopen(quartet_matrices_file_path, "a");
	//this->pResults = fopen(largetree_filename, "w");
	//this->pQuartetResults = fopen(quartettrees_filename, "w");

	if (this->pQuartetResults != nullptr)
	{
		size_t numBytesWritten2 = fwrite(this->quartetResults.c_str(), this->quartetResults.length(), 1, this->pQuartetResults);
		printf("%s written...\n", quartet_matrices_file_path);
		fclose(this->pQuartetResults);
		//reset string for next batch
		this->quartetResults.clear();
	}
}
void distanceMeasure::InternalCalculatorTools::write_clustered_tree_results(DistanceMeasureCalculator* dmc, const int batch_number, const size_t sequence_count)
{
	char clustered_matrix_file_path[200];
	//dmc->GetLargeListMatrixFileName(largelist_matrix_file_path, 200, batch_number, sequence_count);
	//std::string clustered_matrix_file_path(largelist_matrix_file_path);
	dmc->GetClusteredMatrixFileName(clustered_matrix_file_path, batch_number, sequence_count);

	this->pClusteredResults = fopen(clustered_matrix_file_path, "w");
	//this->pResults = fopen(largetree_filename, "w");
	//this->pQuartetResults = fopen(quartettrees_filename, "w");
	if (this->pClusteredResults != nullptr)
	{
		size_t numBytesWritten = fwrite(this->clusteredResults.c_str(), this->clusteredResults.length(), 1, this->pClusteredResults);
		printf("%s written...\n", clustered_matrix_file_path);
		fclose(this->pClusteredResults);
		//reset string for next batch
		this->clusteredResults.clear();
	}
}


float distanceMeasure::InternalCalculatorTools::GetLamdaMatrixDistanceAt(int pos) const
{
	return this->lamdaMatrix.at(pos);
}
void distanceMeasure::InternalCalculatorTools::AppendToQuartetResults(const std::string& res)
{
	this->quartetResults.append(res);
}

