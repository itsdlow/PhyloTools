/******************************************************************************
DeAngelo Wilson
January 18 2020

						IntneralDistanceMEasureCalculator (derived-base)
******************************************************************************/


#include "InternalDistanceMeasureCalculator.h"

#include "FileObjectManager.h"

namespace distanceMeasure
{
	void InternalDistanceMeasureCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id)
	{
		//if alignment needed
		//ALIGN -- refill_FileObjectManager... done by derived calcs
		//calculate LargeTree giving sequence_names_list
		this->CalculateLargeTreeDistanceMeasures(fileObjectManager, sequence_set_names);
		this->CalculateAllQuartetsDistanceMeasures(sequence_set_names);

		////create file for current sequence_set
		this->write_batch_results( batch_id, sequence_set_names.size() );
	}

	//calculate LargeTree (w/o quartets) Distance Matrix
	void InternalDistanceMeasureCalculator::CalculateLargeTreeDistanceMeasures(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names)
	{
		const size_t fileCount = sequence_set_names.size();

		for (auto i = 0u; i < fileCount; i++)
		{
			//this->results.append(pCurrentFileObject->GetFileName());
			this->results.append(sequence_set_names.at(i));


			for (auto j = 0u; j < fileCount; j++)
			{
				//Pvalue, LCS,...
				//NOT IMPLEMENTED:: Guard against not found FileObject (nullptr)
				//pass current (i) fileobject + next (j) fileobject to Distance_Calculator
				float normalizedDistance = this->calculate_normalized_distance(*fileObjectManager.GetSequenceSetFileObject(sequence_set_names.at(i)), *fileObjectManager.GetSequenceSetFileObject(sequence_set_names.at(j)));

				//append to distance matrix for quartet calcs
				this->lamdaMatrix.push_back(normalizedDistance);

				//write lcs to results
				this->results.append(" ");
				this->results.append(std::to_string(normalizedDistance));
			}

			this->results.append("\n");
			printf("\t%zu LCS calculations performed -- %zu calculations remaining...\n", fileCount, (fileCount * fileCount) - (fileCount + (i * fileCount)));
		}
	}
	//4 POINT CONDITION CHECK --> FIND ALL QUARTETS "TREE (T)" INDUCES
	void InternalDistanceMeasureCalculator::CalculateAllQuartetsDistanceMeasures(const std::vector<std::string>& sequence_set_names)
	{
		const int fileCount = static_cast<int>(sequence_set_names.size());

		if (fileCount == 4)
		{
			//return distanceMatrix -- tree
		}
		if (fileCount < 4)
		{
			//ERROR
		}

		for (int i = 0; i < fileCount; i++)
		{
			//while "atleast" 3 OTHER fileObjects exist
			for (int j = i + 1; j < fileCount; j++)
			{
				for (int k = j + 1; k < fileCount; k++)
				{
					for (int l = k + 1; l < fileCount; l++)
					{

						/*
						float sum1 = this->lamdaMatrix.at(this->getArrayIndex(i, j, fileCount)) + this->lamdaMatrix.at(this->getArrayIndex(l, k, fileCount));
						float sum2 = this->lamdaMatrix.at(this->getArrayIndex(i, k, fileCount)) + this->lamdaMatrix.at(this->getArrayIndex(j, l, fileCount));
						float sum3 = this->lamdaMatrix.at(this->getArrayIndex(i, l, fileCount)) + this->lamdaMatrix.at(this->getArrayIndex(j, k, fileCount));
						//printf("(%d, %d + %d, %d) -- %f,  %f, %f\n", i,j,l,k,sum1, sum2, sum3 );
						float minPairwiseSum;
						if(this->fourPointConditionCheck(sum1, sum2, sum3, minPairwiseSum) == -1)
						{
							//4point condition does not hold
							//Quartets method FAILED --> distance Matrix Invalid
							printf("lamdaMatrix invalid (non-additive) -- 4 point condition does not hold\n");
							//break;
							//return;
						}
						*/


						//create quartet Distance Matrix -- append to output
						/*
						--0 lamdaM(count, i) lamdaM(count, j) lamdaM(count, k) lamdaM(count, l)
						--1 lamdaM(count, i)...
						--2
						--3
						*/

						//using four point method--
						//create newick based off results of FPM (--> tells pairings (siblings)) [no branch lengths???]


						this->write_quartet_matrix(i, j, k, l, sequence_set_names, fileCount);
						//printf("wrote quartet matrix\n");

					}
				}
			}
		}

	}



	void InternalDistanceMeasureCalculator::write_quartet_matrix(int i, int j, int k, int l, const std::vector<std::string>& sequence_set_names, const int fileCount)
	{
		//const FileObject* const pFileObjects = reinterpret_cast<const FileObject*>(this->getFileObjectManager().getFileObjectsAddr());
		//const int fileCount = this->getFileObjectManager().get_file_count();

		//vector of quartet indicies
		std::vector<int> indexV{ i,j,k,l };//removed for MAC build
		//std::vector<int> indexV;
		//indexV.push_back(i);
		//indexV.push_back(j);
		//indexV.push_back(k);
		//indexV.push_back(l);


		const int quartetSize = 4;
		//line 0 of matrix for PHYLIP format (num of taxon)
		this->quartetResults.append(std::to_string(quartetSize).append("\n"));
		for (int row = 0; row < quartetSize; row++)
		{
			//taxon name of current fileObject
			//this->quartetResults.append(pFileObjects[indexV.at(row)].GetFileName());
			this->quartetResults.append(sequence_set_names.at(indexV.at(row)));

			//distance measures
			this->quartetResults.append(" ");
			this->quartetResults.append(std::to_string(this->lamdaMatrix.at(this->getArrayIndex(indexV.at(row), i, fileCount))));
			this->quartetResults.append(" ");
			this->quartetResults.append(std::to_string(this->lamdaMatrix.at(this->getArrayIndex(indexV.at(row), j, fileCount))));
			this->quartetResults.append(" ");
			this->quartetResults.append(std::to_string(this->lamdaMatrix.at(this->getArrayIndex(indexV.at(row), k, fileCount))));
			this->quartetResults.append(" ");
			this->quartetResults.append(std::to_string(this->lamdaMatrix.at(this->getArrayIndex(indexV.at(row), l, fileCount))).append("\n"));
		}
		this->quartetResults.append("\n");
	}

	//Open new batch file && write results buffer to output FILEs
	void InternalDistanceMeasureCalculator::write_batch_results(const int batch_number, const size_t sequence_count)
	{
		char largelist_filename[50];
		char quartets_filename[50];

		//WINDOWS DEPENDENCE --  '\\'
		//open output files
		////TODO include Distance calculator tag (id string)
		/////TODO -- create singleton for filename format strings...  ******
		//sprintf(largetree_filename, "ForestFiles\\LargeListMatrix_%d.txt", batch_number);
		//sprintf(quartettrees_filename, "ForestFiles\\QuartetMatrixes_%d.txt", batch_number);
		sprintf_s(largelist_filename, "ForestFiles\\LargeListMatrix_%zu_%d.txt", sequence_count, batch_number);
		sprintf_s(quartets_filename, "ForestFiles\\QuartetMatrixes_%zu_%d.txt", sequence_count, batch_number);

		//WINDOWS DEPENDENCE -- "_s" functions
		fopen_s(&this->pResults, largelist_filename, "w");
		fopen_s(&this->pQuartetResults, quartets_filename, "w");
		//this->pResults = fopen(largetree_filename, "w");
		//this->pQuartetResults = fopen(quartettrees_filename, "w");



		if (this->pResults != nullptr)
		{
			size_t numBytesWritten = fwrite(this->results.c_str(), this->results.length(), 1, this->pResults);
			printf("%s written...\n", largelist_filename);
			fclose(this->pResults);
			//reset string for next batch
			this->results.clear();
		}
		if (this->pQuartetResults != nullptr)
		{
			size_t numBytesWritten2 = fwrite(this->quartetResults.c_str(), this->quartetResults.length(), 1, this->pQuartetResults);
			printf("%s written...\n", quartets_filename);
			fclose(this->pQuartetResults);
			//reset string for next batch
			this->quartetResults.clear();
		}

	}

}