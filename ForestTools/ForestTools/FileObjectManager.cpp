/******************************************************************************
DeAngelo Wilson
January 3 2020

						FileObjectManager 
******************************************************************************/

//#include <windows.h>

#include "FileObjectManager.h"

#include "FileObject.h"
//#include "SequenceDirectoryProcessor.h"
#include "SequenceFileProcessor.h"


#include <fstream>
//#include <iostream>

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



	FileObjectManager::FileObjectManager(const std::string& sequence_names_path, const std::string& path):
	fileCount(0),
	//pFileObjectsBuffer(new FileObject[sequenceCount]),
	pFileObjectsBuffer(nullptr),
	pSequenceSetFileObjectBuffer(nullptr),
	sp(nullptr),
	sequenceSetCount(0),
	filePath(path),
	sequencesPath(path)
	{
		//switch (path_type)
		//{
		//	case SequenceProcessorType::FileProcessor:
		//		this->sp = new SequenceFileProcessor();
		//		break;
		//	case SequenceProcessorType::DirectoryProcessor:
		//		this->sp = new SequenceDirectoryProcessor();
		//		break;
		//}
		this->sp = new SequenceFileProcessor();
		//get all sequence names
		this->currentSequenceNames = this->FillSequenceNamesVector(sequence_names_path);

		printf("Filling FileObjectBuffer with %d sequences\n", this->fileCount);
		//creates Sequence_File_Objects and fills array
		//this->sp->CreateFileObjects(this->pFileObjectsBuffer, path, this->fileCount);
		this->maxSequenceSetSequenceLength = this->sp->CreateFileObjects(this, this->pFileObjectsBuffer);
		//WINDOWS DEPENDENCE
		size_t buffer_size = sizeof(FileObject) * this->fileCount;
		memcpy_s(this->pSequenceSetFileObjectBuffer, buffer_size, this->pFileObjectsBuffer, buffer_size);
	}
	
	//used to create sub-fileObject_sets (sequence Set file objects -- for batch run)
	void FileObjectManager::RefillFileObjectsBuffer(const std::vector<std::string>& sequence_set_names, std::string new_sequences_path)
	{
		this->sequencesPath = new_sequences_path;
		this->sequenceSetCount = static_cast<int>(sequence_set_names.size());
		this->currentSequenceNames = sequence_set_names;
		this->maxSequenceSetSequenceLength = this->sp->CreateFileObjects(this, this->pSequenceSetFileObjectBuffer);
	}

	const std::vector<std::string> FileObjectManager::FillSequenceNamesVector(const std::string& sequence_names_path)
	{
		//open names file
		std::ifstream sequenceNamesInput(sequence_names_path);

		if (!sequenceNamesInput.is_open())
		{
			printf("File at path: %s - could not be opened\nSequence names could not be retrieved\n", sequence_names_path.c_str());
			//int res;
			//printf("If you would like to try the default Sequence Name retrieval type: '0'\n");

			//
			////std::cin >> res;
			////use default streagy...
			//if (!res)
			//{
			//	printf("default name retrieval inactive... WIP\n");
			//}
			exit(0);
		}
		//fill sequenceNames vector with organisms namesd
		else
		{
			int count = 0;
			//read file
			std::string line;
			while (std::getline(sequenceNamesInput, line))
			{
				//add name ("line") to vector
				this->sequenceNames.push_back(line);
				count++;
			}
			//SET FILE COUNT BASED OFF sequence names count
			this->fileCount = count;
			this->sequenceSetCount = this->fileCount;
			this->pFileObjectsBuffer = new FileObject[this->fileCount];
			this->pSequenceSetFileObjectBuffer = new FileObject[this->fileCount];
		}
		return this->sequenceNames;
	}

	std::string distanceMeasure::FileObjectManager::CheckForSequenceName(const std::string& line) const
	{
		//check if any given sequence names are within "line" -- return one_word_name
		for(auto it = this->currentSequenceNames.begin(); it != this->currentSequenceNames.end(); it++)
		{
			//if name (it) in current_sequence_line
			if(line.find(*it) != std::string::npos)
			{
				std::string name(*it);

				////replace all spaces w/ "__underscores__"
				//for(auto i = 0u; i < name.size(); i++)
				//{
				//	if( isspace(name.at(i)) )
				//	{
				//		name.replace(i, 1u, 1u, '_');
				//	}
				//}//--> create_one_word_species_name

				return name;
			}
		}

		printf("Sequence (Organism) name not found in Fasta File Sequences\n");
		return std::string();
	}

	/****************************************************
	 *
	 *                   Accessors
	 * 
	 ****************************************************/

	const FileObject* const FileObjectManager::GetFileObject(const std::string name) const
	{
		//find fileobject w. "name" filename
		for(int i = 0; i < this->fileCount; i++)
		{
			if(this->pFileObjectsBuffer[i].GetFileName() == name)
			{
				return &this->pFileObjectsBuffer[i];
			}
		}
		printf("FileObject not found for '%s'\n", name.c_str());
		exit(0);
		return nullptr;
	}

	const FileObject* const distanceMeasure::FileObjectManager::GetSequenceSetFileObject(const std::string name) const
	{
		//find fileobject w. "name" filename
		for (int i = 0; i < this->sequenceSetCount; i++)
		{
			if (this->pSequenceSetFileObjectBuffer[i].GetFileName() == name)
			{
				return &this->pSequenceSetFileObjectBuffer[i];
			}
		}
		printf("FileObject not found for '%s'\n", name.c_str());
		exit(0);
		return nullptr;
	}

	int distanceMeasure::FileObjectManager::GetMax_SS_SequenceLength() const
	{
		return this->maxSequenceSetSequenceLength;
	}
	const FileObject* distanceMeasure::FileObjectManager::get_file_objects_addr() const
	{
		return this->pFileObjectsBuffer;
	}
	const FileObject* distanceMeasure::FileObjectManager::get_sequence_set_file_objects_addr() const
	{
		return this->pSequenceSetFileObjectBuffer;
	}
	const std::string distanceMeasure::FileObjectManager::get_sequence_set_path() const
	{
		return this->sequencesPath;
	}
	const std::string distanceMeasure::FileObjectManager::get_original_sequences_path() const
	{
		return this->filePath;
	}
	const int FileObjectManager::get_file_count() const
	{
		return this->fileCount;
	}
	const int distanceMeasure::FileObjectManager::get_sequence_set_count() const
	{
		return this->sequenceSetCount;
	}

	FileObjectManager::~FileObjectManager()
	{
		delete[] this->pFileObjectsBuffer;
	}
}