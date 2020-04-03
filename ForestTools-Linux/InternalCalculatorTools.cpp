/******************************************************************************
DeAngelo Wilson
January 18 2020

						IntneralDistanceMEasureCalculator (derived-base)
******************************************************************************/


#include "InternalCalculatorTools.h"

#include "SystemParameters.h"
#include "DistanceMeasureCalculator.h"
#include "FileObjectManager.h"


void distanceMeasure::InternalCalculatorTools::CalculateDistanceMeasuresAndTrees(DistanceMeasureCalculator* dmc, FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)
{
	//pass it forward
	//InternalDistanceMeasureCalculator::calculate_and_output_matrix(fileObjectManager, sequence_set_names, sequence_set, batch_id);
	//calculate LargeTree giving sequence_names_list
	this->CalculateLargeTreeDistanceMeasures(dmc, fileObjectManager, sequence_set_names);
	InternalCalculatorTools::CalculateAllQuartetsDistanceMeasures(dmc, fileObjectManager, sequence_set_names);

	////create file for current sequence_set
	this->write_batch_results(dmc, batch_id, sequence_set_names.size());
	InternalCalculatorTools::create_tree(dmc, sequence_set_names, batch_id);
}


void distanceMeasure::InternalCalculatorTools::create_tree(DistanceMeasureCalculator* dmc, const std::vector<std::string>& sequence_set_names, const int batch_id)
{
	//get batch_ID_matrix FILE (quartets + LargeList)
		//feed to fastme
		//********************************************
	const int sequence_set_count = static_cast<int>(sequence_set_names.size());
	//Get current_sequence-set_matrix files
	char largelist_matrix_file_path[100];
	dmc->GetLargeListMatrixFileName(largelist_matrix_file_path, 100, batch_id, sequence_set_count);
	char quartet_matrices_file_path[100];
	dmc->GetQuartetsMatrixFileName(quartet_matrices_file_path, 100, batch_id, sequence_set_count);

	//get path for new_tree files
	char large_tree_file_path[150];
	dmc->GetLargeListTreeFileName(large_tree_file_path, 150, batch_id, sequence_set_count);
	char quartet_trees_file_path[150];
	dmc->GetQuartetsTreeFileName(quartet_trees_file_path, 150, batch_id, sequence_set_count);

	//sequence_set_size choose 4
	const int quartetCount = DistanceMeasureCalculator::GetQuartetCombinations(sequence_set_count);

	char fastme_command[200];
	char fastme_quartets_command[200];
	dmc->GetFastMECommand(fastme_command, 200, largelist_matrix_file_path, 1, large_tree_file_path);
	dmc->GetFastMECommand(fastme_quartets_command, 200, quartet_matrices_file_path, quartetCount, quartet_trees_file_path);

	system(fastme_command);
	system(fastme_quartets_command);

	//TODO:: currents sequence set progress debug statement
	printf("1st trees created\n");
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
		printf("\t%zu %s calculations performed -- %zu calculations remaining...\n", fileCount, dmc->GetCalculatorName().c_str(), (fileCount * fileCount) - (fileCount + (i * fileCount)));
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
	int totalQuartets = DistanceMeasureCalculator::GetQuartetCombinations(fileCount);
	int count = 0;
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
					//std::vector<int> indexV;
					//indexV.push_back(i);
					//indexV.push_back(j);
					//indexV.push_back(k);
					//indexV.push_back(l);

					
					//pass FOM in for Pvalue Calc--> does not use lambda matrix
					dmc->write_quartet_matrix(fileObjectManager, indexV, sequence_set_names, fileCount);
					count++;
					//NOTE:: CHANGE TO '\r' when other debug print statements removed
					printf("%d/%d %s Quartet Matrices calculated...\n", count, totalQuartets, dmc->GetCalculatorName().c_str());
				}
			}
		}
	}

}

//Open new batch file && write results buffer to output FILEs
void distanceMeasure::InternalCalculatorTools::write_batch_results(DistanceMeasureCalculator* dmc, const int batch_number, const size_t sequence_count)
{
	char largelist_matrix_file_path[100];
	dmc->GetLargeListMatrixFileName(largelist_matrix_file_path, 100, batch_number, sequence_count);
	char quartet_matrices_file_path[100];
	dmc->GetQuartetsMatrixFileName(quartet_matrices_file_path, 100, batch_number, sequence_count);


	//WINDOWS DEPENDENCE -- "_s" functions
	fopen_s(&this->pResults, largelist_matrix_file_path, "w");
	fopen_s(&this->pQuartetResults, quartet_matrices_file_path, "w");
	//this->pResults = fopen(largetree_filename, "w");
	//this->pQuartetResults = fopen(quartettrees_filename, "w");
	if (this->pResults != nullptr)
	{
		size_t numBytesWritten = fwrite(this->results.c_str(), this->results.length(), 1, this->pResults);
		printf("%s written...\n", largelist_matrix_file_path);
		fclose(this->pResults);
		//reset string for next batch
		this->results.clear();
	}
	if (this->pQuartetResults != nullptr)
	{
		size_t numBytesWritten2 = fwrite(this->quartetResults.c_str(), this->quartetResults.length(), 1, this->pQuartetResults);
		printf("%s written...\n", quartet_matrices_file_path);
		fclose(this->pQuartetResults);
		//reset string for next batch
		this->quartetResults.clear();
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

