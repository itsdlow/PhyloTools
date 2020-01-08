/******************************************************************************
DeAngelo Wilson
January 3 2020

						FileObjectManager 
******************************************************************************/

//#include <windows.h>

#include "FileObjectManager.h"

#include "FileObject.h"
#include "SequenceDirectoryProcessor.h"
#include "SequenceFileProcessor.h"


#include <fstream>
#include <iostream>

namespace distanceMeasure
{ 
	//TODO
	/*************************************************************
		Allow choice between 1 Fasta File (current set up)
							&&
		Directory of Fasta Files (sequence directory processor) 
				
					--> Create FileObject_Array
	**************************************************************/
	//Guards::
		// 1) If data needs to be aligned (MrBayes/P-Value) --> Sequences need same length***  



	FileObjectManager::FileObjectManager(int sequenceCount, std::string sequence_names_path, std::string path, SequenceProcessorType path_type):
	fileCount(sequenceCount),
	pFileObjectsBuffer(new FileObject[sequenceCount]),
	sp(nullptr),
	sequencesPath(path)
	{
		
		switch (path_type)
		{
			case SequenceProcessorType::FileProcessor:
				this->sp = new SequenceFileProcessor();
				break;
			case SequenceProcessorType::DirectoryProcessor:
				this->sp = new SequenceDirectoryProcessor();
				break;
		}
		//get all sequence names
		this->FillSequenceNamesVector(sequence_names_path);

		printf("Filling FileObjectBuffer with %d sequences\n", this->fileCount);
		//creates Sequence_File_Objects and fills array
		//this->sp->CreateFileObjects(this->pFileObjectsBuffer, path, this->fileCount);
		this->sp->CreateFileObjects(this, this->pFileObjectsBuffer);


		//this->fillFileObjectsBuffer(path);
	}

	void FileObjectManager::FillSequenceNamesVector(const std::string& sequence_names_path)
	{
		//open names file
		std::ifstream sequenceNamesInput(sequence_names_path);

		if (!sequenceNamesInput.is_open())
		{
			printf("File at path: %s - could not be opened\nSequence names could not be retrieved\n", sequence_names_path.c_str());
			int res;
			printf("If you would like to try the default Sequence Name retrieval type: '0'\n");
			std::cin >> res;
			//use default streagy...
			if (!res)
			{
				printf("default name retrieval inactive... WIP\n");
			}
		}
		//fill sequenceNames vector with file names
		else
		{
			int count = 0;
			//read file
			std::string line;
			FileObject* pCurrentFileObject = this->pFileObjectsBuffer;
			while (std::getline(sequenceNamesInput, line) && count < this->fileCount)
			{
				//replace "space" with "_"
					// Get the first occurrence
				size_t pos = line.find(" ");

				// Repeat till end is reached
				while (pos != std::string::npos)
				{
					// replace "pos" with "_"
					line.replace(pos, 1u, "_");

					// Get the next occurrence from the current position
					pos = line.find(" ", pos + 1u);
				}

				//add name ("line") to vector
				this->sequenceNames.push_back(line);
			}
		}
	}


	const std::string distanceMeasure::FileObjectManager::CheckForSequenceName(const std::string& line) const
	{
		//check if any given sequence names are within "line" -- return one_word_name

		return std::string();
	}


	////given a single Fasta File as input -- containing "fileCount" many sequences
	//void distanceMeasure::FileObjectManager::fillFileObjectsBuffer(std::string path)
	//{
	//	//open file
	//	//read line 1 --> get file name
	//	//read line 2 pass sequence (line) to FileObject()

	//	//open file
	//	std::ifstream sequenceNamesInput(path);

	//	if (!sequenceNamesInput.is_open())
	//	{
	//		printf("File at path: %s - could not be opened\nFile Object not created\n", path.c_str());
	//	}
	//	else
	//	{
	//		//start = 4 end = 22/23
	//		// Allivibrio fischeri 

	//		int count = 0;
	//		//read file
	//		std::string line;
	//		FileObject* pCurrentFileObject = this->pFileObjectsBuffer;
	//		while (std::getline(sequenceNamesInput, line) && count < this->fileCount)
	//		{
	//			//get filename (species name) MUST BE 2 WORDS TO BE SET PROPERLY
	//				//1st whitespace -- 3rd
	//			//start of species name
	//			int species_start_index = line.find_first_of(" ", 0) + 1;
	//			//printf("start:: %d\n", species_start_index);

	//			//middle of species name (space index)
	//			int species_mid_index = line.find_first_of(" ", species_start_index);
	//			//printf("mid:: %d\n", species_mid_index);

	//			//index of last letter of species name
	//			int species_end_index = line.find_first_of(" ", species_mid_index + 1) - 1;
	//			//printf("end:: %d\n", species_end_index);

	//		

	//			std::string speciesName = line.substr(species_start_index, species_mid_index-species_start_index) + "_" + line.substr(species_mid_index + 1, species_end_index - species_mid_index);
	//			
	//			/*************************************************/				
	//			//HARD CODE RETRIEVAL OF 3rd word (seperated by space) FOR..
	//			//Eulemur_macaco_macaco
 //       		//Ovis_orientalis_ophion

 //       		//if sepecies is either ^^ --> retrieve 3rd word_name
	//			if(speciesName.substr(0, 3) == "Ovi" || speciesName.substr(0,3) == "Eul")
	//			{
	//				int third_word_start = species_end_index + 2;
	//				//get 3rd spaced word and append
	//				int species_end_end_index = line.find_first_of(" ", third_word_start) - 1;
	//				speciesName+= ("_" + line.substr(third_word_start, species_end_end_index - (species_end_index + 1)));
	//			}
	//			/*************************************************/

	//			printf("SpeciesName::%s-\n",speciesName.c_str());
	//			
	//			//get sequence (line 2)
	//			std::getline(sequenceNamesInput, line);
	//			//create file object with file name and sequence string
	//			FileObject* tmp = new(pCurrentFileObject++) FileObject(line, speciesName);
	//			printf("created FileObject\n");


	//			//this->pFileObjectsBuffer[count] = new FileObject(line, speciesName);
	//			count++;

	//		}
	//		printf("finsihed processing file\n");
	//		sequenceNamesInput.close();
	//	}

	//}

	const FileObject* FileObjectManager::getFileObjectsAddr() const
	{
		return this->pFileObjectsBuffer;
	}

	const int FileObjectManager::get_file_count() const
	{
		return this->fileCount;
	}
	const std::string distanceMeasure::FileObjectManager::GetPathToSequences() const
	{
		return this->sequencesPath;
	}

	FileObjectManager::~FileObjectManager()
	{
		delete[] this->pFileObjectsBuffer;
	}
}