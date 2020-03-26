/******************************************************************************
DeAngelo Wilson
January 18 2020

						IntneralDistanceMEasureCalculator (derived-base)
******************************************************************************/


#include "InternalDistanceMeasureCalculator.h"

#include "FileObjectManager.h"
#include "SystemParameters.h"

namespace distanceMeasure
{
	void InternalDistanceMeasureCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)
	{
		//if alignment needed
		//ALIGN -- refill_FileObjectManager... done by derived calcs

		//calculate LargeTree giving sequence_names_list
		this->CalculateLargeTreeDistanceMeasures(fileObjectManager, sequence_set_names);
		this->CalculateAllQuartetsDistanceMeasures(fileObjectManager, sequence_set_names);

		////create file for current sequence_set
		this->write_batch_results( batch_id, sequence_set_names.size() );

		this->create_tree(sequence_set_names, batch_id);
	}

	void distanceMeasure::InternalDistanceMeasureCalculator::create_tree(const std::vector<std::string>& sequence_set_names, const int batch_id)
	{
		//get batch_ID_matrix FILE (quartets + LargeList)
			//feed to fastme
			//********************************************
		const int sequence_set_count = static_cast<int>(sequence_set_names.size());
		//Get current_sequence-set_matrix files
		char largelist_matrix_file_path[100];
		this->GetLargeListMatrixFileName(largelist_matrix_file_path, 100, batch_id, sequence_set_count);
		char quartet_matrices_file_path[100];
		this->GetQuartetsMatrixFileName(quartet_matrices_file_path, 100, batch_id, sequence_set_count);

		//get path for new_tree files
		char large_tree_file_path[150];
		this->GetLargeListTreeFileName(large_tree_file_path, 150, batch_id, sequence_set_count);
		char quartet_trees_file_path[150];
		this->GetQuartetsTreeFileName(quartet_trees_file_path, 150, batch_id, sequence_set_count);

		//sequence_set_size choose 4
		const int quartetCount = DistanceMeasureCalculator::GetQuartetCombinations( sequence_set_count );

		char fastme_command[200];
		char fastme_quartets_command[200];
		this->GetFastMECommand(fastme_command, 200, largelist_matrix_file_path, 1, large_tree_file_path);
		this->GetFastMECommand(fastme_quartets_command, 200, quartet_matrices_file_path, quartetCount, quartet_trees_file_path);

		system(fastme_command);
		system(fastme_quartets_command);

		//TODO:: currents sequence set progress debug statement
		printf("1st trees created\n");
	}

	
	//calculate LargeTree (w/o quartets) Distance Matrix -- phylib format
	void InternalDistanceMeasureCalculator::CalculateLargeTreeDistanceMeasures(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names)
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
				float normalizedDistance = this->calculate_normalized_distance(*fileObjectManager.GetSequenceSetFileObject(sequence_set_names.at(i)), *fileObjectManager.GetSequenceSetFileObject(sequence_set_names.at(j)));

				//append to distance matrix for quartet calcs
				this->lamdaMatrix.push_back(normalizedDistance);

				//write lcs to results
				this->results.append(" ");
				this->results.append(std::to_string(normalizedDistance));
			}

			this->results.append("\n");
			printf("\t%zu %s calculations performed -- %zu calculations remaining...\r", fileCount, this->GetCalculatorName().c_str(), (fileCount * fileCount) - (fileCount + (i * fileCount)));
		}
	}

	//4 POINT CONDITION CHECK --> FIND ALL QUARTETS "TREE (T)" INDUCES
	void InternalDistanceMeasureCalculator::CalculateAllQuartetsDistanceMeasures(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names)
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

						
						//create quartet Distance Matrix -- append to output
						/*
						--0 lamdaM(count, i) lamdaM(count, j) lamdaM(count, k) lamdaM(count, l)
						--1 lamdaM(count, i)...
						--2
						--3
						*/
						//pass FOM in for Pvalue Calc--> does not use lambda matrix
						this->write_quartet_matrix(fileObjectManager, indexV, sequence_set_names, fileCount);
						count++;
						printf("%d/%d %s Quartet Matrices calculated...\r", count, totalQuartets, this->GetCalculatorName().c_str());
					}
				}
			}
		}

	}

	//Internal calc - quartet matrix creastion --> uses values calculated from LargeListTree... (cannot be used by Aligned_Internal calc --> override)
	void InternalDistanceMeasureCalculator::write_quartet_matrix(FileObjectManager& fileObjectManager, const std::vector<int>& speciesSequenceSetIndexes, const std::vector<std::string>& sequence_set_names, const int fileCount)
	{
		//const FileObject* const pFileObjects = reinterpret_cast<const FileObject*>(this->getFileObjectManager().getFileObjectsAddr());
		//const int fileCount = this->getFileObjectManager().get_file_count();

		const int quartetSize = 4;
		//line 0 of matrix for PHYLIP format (num of taxon)
		this->quartetResults.append(std::to_string(quartetSize).append("\n"));
		for (int row = 0; row < quartetSize; row++)
		{
			//taxon name of current fileObject
			//this->quartetResults.append(pFileObjects[indexV.at(row)].GetFileName());
			//this->quartetResults.append(sequence_set_names.at(indexV.at(row)));
			std::string name = sequence_set_names.at(speciesSequenceSetIndexes.at(row));
			//TODO:: refine so dont need to always swap -- change FOM to store one-word-sequence_names
			DistanceMeasureCalculator::swap_space_with_underscores(name);
			this->quartetResults.append(name);
			
			//append all normalized distances between species - "name" and other quartet species
			for (auto it = speciesSequenceSetIndexes.begin(); it != speciesSequenceSetIndexes.end(); it++)
			{
				this->quartetResults.append(" ");
				this->quartetResults.append(std::to_string(this->lamdaMatrix.at(DistanceMeasureCalculator::getArrayIndex(speciesSequenceSetIndexes.at(row), *it, fileCount))));
		
			}
			this->quartetResults.append("\n");
		}
		this->quartetResults.append("\n");
	}

	//Open new batch file && write results buffer to output FILEs
	void InternalDistanceMeasureCalculator::write_batch_results(const int batch_number, const size_t sequence_count)
	{
		char largelist_matrix_file_path[100];
		this->GetLargeListMatrixFileName(largelist_matrix_file_path, 100, batch_number, sequence_count);
		char quartet_matrices_file_path[100];
		this->GetQuartetsMatrixFileName(quartet_matrices_file_path, 100, batch_number, sequence_count);

		
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
	
	void distanceMeasure::InternalDistanceMeasureCalculator::GetFastMECommand(char* buffer, const size_t buffer_size, char* input, int count, char* output) const
	{
		//"extra_tools\\fastme-2.1.5\\binaries\\fastme.exe -i %s -D %d -o %s"
		sprintf_s(buffer, buffer_size, SystemParameters::GetFastmeCommandString().c_str(), input, count, output);
	}

}