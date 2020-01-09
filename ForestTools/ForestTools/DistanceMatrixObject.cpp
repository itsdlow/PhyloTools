/******************************************************************************
DeAngelo Wilson
January 3 2020*

						DistanceMatrixObject
******************************************************************************/

#include "DistanceMatrixObject.h"
#include "FileObject.h"
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
		this->writeResults();
		
		delete this->distanceMeasureFunc;
	}

	DistanceMatrixObject::DistanceMatrixObject(std::string sequence_names_dir, std::string sequences_dir, int sequenceCount, DistanceMeasureCalculator* dmc):
	fileObjectManager(sequenceCount, sequence_names_dir, sequences_dir),
	results(std::to_string(sequenceCount).append("\n")),
	distanceMeasureFunc(dmc)
	{
		//open output files ***************************************************************moved to func
		fopen_s(&this->pResults, "output.txt", "w");
		fopen_s(&this->pQuartetResults, "quartets.txt", "w");
		//this->pResults = fopen("output.txt", "w");
		//this->pQuartetResults = fopen("quartets.txt", "w");


		size_t un_sequenceCount = sequenceCount;
		size_t matrix_res = un_sequenceCount * un_sequenceCount;
		size_t results_res = matrix_res * 4u;
		//RESERVE "results (string)" SPACE BASED ON NUMBER OF SEQUENCES AND "MAX_LINE_SIZE"
			//reserve space for sequenceCount (4-digit) floats and sequenceCount lines
		results.reserve(results_res);
		//reserve room for sequenceCount choose 4 distance Matrixes
		//quartetResults.reserve(sequenceCount);
		lamdaMatrix.reserve(matrix_res);
		printf("dmo constructed\n");
	}

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
			//TODO
			//read file and create matrixes
			std::string line;
			
			//while more matrix/tree - sets
			while(std::getline(fastaInput, line))
			{
				int sequence_count = line.front();
				//find first sequence_name
				for(auto i = 1; i < line.size(); i++)
				{
					if (!isspace(line.at(i)))
					{
						//store sequence
					}
				}
			}
			
		}
		
	}


//TODO: BATCH Measure calcs -- must create new file and ensure that only needed fileobjects are included in each matrix
	//calculate LargeTree (w/o quartets) Distance Matrix
	void DistanceMatrixObject::CalculateLargeTreeDistanceMeasures()
	{
		const FileObject* const pFileObjects = this->getFileObjectManager().getFileObjectsAddr();
		const FileObject* pCurrentFileObject = pFileObjects;
		const int fileCount = this->getFileObjectManager().get_file_count();

		for (int i = 0; i < fileCount; i++)
		{
			this->results.append(pCurrentFileObject->GetFileName());
			
			for (int j = 0; j < fileCount; j++)
			{
				//takes 2 file Objects and computes distance measure (default is Longest common subsequece measure)
				//int lcs = this->distanceMeasureFunc(*pCurrentFileObject, pFileObjects[j]);
				//float normalizedDistance = this->normalize(lcs, maxSequenceLength(pCurrentFileObject->sequencesize, pFileObjects[j].sequencesize));
				
				//generic / pvalue
				float normalizedDistance = this->distanceMeasureFunc->operator()(*pCurrentFileObject, pFileObjects[j]);

				//float normalizedDistance =diffCount / pCurrentFileObject->sequencesize;// this->normalize(diffCount, pCurrentFileObject->sequencesize);

				//append to distance matrix for quartet calcs
				this->lamdaMatrix.push_back(normalizedDistance);

				//write lcs to results
				this->results.append(" ");
				this->results.append(std::to_string(normalizedDistance));
			}

			this->results.append("\n");
			printf("\t%d LCS calculations performed -- %d calculations remaining...\n", fileCount, (fileCount*fileCount) - (fileCount + (i * fileCount)));
			pCurrentFileObject++;
		}
	}
	//4 POINT CONDITION CHECK --> FIND ALL QUARTETS "TREE (T)" INDUCES
	void DistanceMatrixObject::CalculateAllQuartetsDistanceMeasures()
	{
		const FileObject* const pFileObjects = reinterpret_cast<const FileObject*>(this->getFileObjectManager().getFileObjectsAddr());
		const FileObject* pCurrentFileObject = pFileObjects;
		const int fileCount = this->getFileObjectManager().get_file_count();
		
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


						this->writeQuartetMatrix(i,j,k,l);
						//printf("wrote quartet matrix\n");

					}
				}
			}
		}

	}

	
	//write results buffer to output FILE, closes FILE
	void DistanceMatrixObject::writeResults()
	{
		size_t numBytesWritten = fwrite(this->results.c_str(), this->results.length(), 1, this->pResults);
		printf("output.txt written...\n");
		fclose(this->pResults);

		size_t numBytesWritten2 = fwrite(this->quartetResults.c_str(), this->quartetResults.length(), 1, this->pQuartetResults);
		printf("quartets.txt written...\n");
		fclose(this->pQuartetResults);
	}
	void DistanceMatrixObject::writeQuartetMatrix(int i, int j, int k, int l)
	{
		const FileObject* const pFileObjects = reinterpret_cast<const FileObject*>(this->getFileObjectManager().getFileObjectsAddr());
		const int fileCount = this->getFileObjectManager().get_file_count();

		//vector of quartet indicies
		std::vector<int> indexV {i,j,k,l};//removed for MAC build
		//std::vector<int> indexV;
		//indexV.push_back(i);
		//indexV.push_back(j);
		//indexV.push_back(k);
		//indexV.push_back(l);


		int quartetSize = 4;
		//line 0 of matrix for PHYLIP format (num of taxon)
		this->quartetResults.append(std::to_string(quartetSize).append("\n"));
		for (int row = 0; row < quartetSize; row++)
		{
			//taxon name of current fileObject
			this->quartetResults.append(pFileObjects[indexV.at(row)].GetFileName());
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

	//function for testing to statically set lamda vector
	void DistanceMatrixObject::setCalculateDistanceMeasureTEST(float* array)
	{
		const int fileCount = this->getFileObjectManager().get_file_count();
		for (int i = 0; i < fileCount; ++i)
		{
			for (int j = 0; j < fileCount; ++j)
			{
				//printf("Index: %d = %f\n",this->getArrayIndex(i,j,fileCount),array[this->getArrayIndex(i,j,fileCount)] );
				this->lamdaMatrix.push_back(array[this->getArrayIndex(i,j,fileCount)]);
			}
		}
		//delete dynamic array
		delete[] array;
	}

	
	/*
	const float DistanceMatrixObject::normalize(int lcs, long maxSequenceSize) const
	{

		//cheaty + 1 to set LCS to correct value???**************LCS CALC OFF BY 1********
		
		float normalizedDistance = float(maxSequenceSize - lcs) / maxSequenceSize;

		printf("%f -- %d -- %d\n", normalizedDistance, lcs, maxSequenceSize);
		return normalizedDistance;
	}

	const float DistanceMatrixObject::normalize(int differenceCount, int alignedSequenceSize) const
	{		
		float normalizedDistance = differenceCount / alignedSequenceSize;

		printf("%f -- %d -- %d\n", normalizedDistance, lcs, maxSequenceSize);
		return normalizedDistance;
	}
	*/
	int DistanceMatrixObject::fourPointConditionCheck(float sum1, float sum2, float sum3, float& minPairwiseSum) const
	{
		//if sum1 is min
		if(sum1 < sum2 && sum1 < sum3)
		{
			if(sum2 == sum3)
			{
				minPairwiseSum = sum1;
				return 0;
			}
		}
		//else if sum2 is min
		else if(sum2 < sum1 && sum2 < sum3)
		{
			if(sum1 == sum3)
			{
				minPairwiseSum = sum2;
				return 0;
			}
		}
		//else sum3 is min
		else 
		{
			if(sum1 == sum2)
			{
				minPairwiseSum = sum3;
				return 0;
			}
		}
		//Four Point condition does not hold!!!
		return -1;
	}

	inline int DistanceMatrixObject::maxSequenceLength(int sequencesize1, int sequencesize2) const
	{
		return sequencesize1 > sequencesize2 ? sequencesize1 : sequencesize2;
	}

	inline const int DistanceMatrixObject::getArrayIndex(int row, int col, int rowCount) const
	{
			return (row*rowCount) + col;
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

	FILE* DistanceMatrixObject::getFILEPtr() const
	{
		return this->pResults;
	}


}


