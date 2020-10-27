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

#include "SequenceNamesStrategy.h"

//#include <cstring>

	//TODO
	/*************************************************************
		Allow choice between 1 Fasta File (current set up)
							&&
		Directory of Fasta Files (sequence directory processor) 
				
					--> Create FileObject_Array
	**************************************************************/
	//Guards::
		// 1) If data needs to be aligned (MrBayes/P-Value) --> Sequences need same length***  

namespace distanceMeasure
{ 
	FileObjectManager::FileObjectManager(SequenceNamesStrategy* strategy, const std::string& path):
	fileCount(0),
	//pFileObjectsBuffer(new FileObject[sequenceCount]),
	pFileObjectsBuffer(nullptr),
	pSequenceSetFileObjectBuffer(nullptr),
	sp(new SequenceFileProcessor()),
	pNameStrategy(strategy),
	sequenceSetCount(0),
	filePath(path),
	sequencesPath(path)
	{
		// NOTE :: dir processing if given a set of sequence sets.... (protein analysis)
		//this->sp = new SequenceFileProcessor();
	}

	void distanceMeasure::FileObjectManager::InitializeFileObjects()
	{
		/*
		 * 			this->fileCount = count;
			this->sequenceSetCount = this->fileCount;
			this->pFileObjectsBuffer = new FileObject[this->fileCount];
			this->pSequenceSetFileObjectBuffer = new FileObject[this->fileCount];
		}
		return this->sequenceNames;
		 */
		//get all sequence names
		//this->currentSequenceNames = this->FillSequenceNamesVector(sequence_names_path);
		this->pNameStrategy->FillSequenceNamesVector(this->sequenceNames, this->currentSequenceNames, this->fileCount, this->pFileObjectsBuffer);
		this->pSequenceSetFileObjectBuffer = new FileObject[this->fileCount];
		//used in createFileObjects
		this->sequenceSetCount = this->fileCount;

		printf("Filling FileObjectBuffer with %d sequences\n", this->fileCount);
		//creates Sequence_File_Objects and fills array
		this->sp->CreateFileObjects(this, this->pFileObjectsBuffer);
		
		//"sequenceNames" can be set in "FillSequenceNameVector" || "CreateFileObjects" -- must be initialized after...
			//NOTE:: Needed by SequenceListGenerator...
		this->currentSequenceNames = this->sequenceNames;
	}

	
	//NOTE:: FOR UNALIGNED DMC's --> sequence set duplicated (shouldnt be needed... but is)
	//used to create sub-fileObject_sets (sequence Set file objects -- for batch run)
	void FileObjectManager::RefillFileObjectsBuffer(const std::vector<std::string>& sequence_set_names, const std::string& new_sequences_path)
	{
		this->sequencesPath = new_sequences_path;
		this->sequenceSetCount = static_cast<int>(sequence_set_names.size());
		this->currentSequenceNames = sequence_set_names;
		printf("Creating %d FileObjects for this Sequence Set\n", this->sequenceSetCount);
		this->sp->CreateFileObjects(this, this->pSequenceSetFileObjectBuffer);
	}

	//const std::vector<std::string> FileObjectManager::FillSequenceNamesVector(const std::string& sequence_names_path)
	//{
	//	//open names file
	//	std::ifstream sequenceNamesInput(sequence_names_path);

	//	if (!sequenceNamesInput.is_open())
	//	{
	//		printf("File at path: %s - could not be opened\nSequence names could not be retrieved\n", sequence_names_path.c_str());
	//		//int res;
	//		//printf("If you would like to try the default Sequence Name retrieval type: '0'\n");

	//		//
	//		////std::cin >> res;
	//		////use default streagy...
	//		//if (!res)
	//		//{
	//		//	printf("default name retrieval inactive... WIP\n");
	//		//}
	//		exit(0);
	//	}
	//	//fill sequenceNames vector with organisms namesd
	//	else
	//	{
	//		int count = 0;
	//		//read file
	//		std::string line;
	//		while (std::getline(sequenceNamesInput, line))
	//		{
	//			//add name ("line") to vector
	//			this->sequenceNames.push_back(line);
	//			count++;
	//		}
	//		//SET FILE COUNT BASED OFF sequence names count
	//		this->fileCount = count;
	//		this->sequenceSetCount = this->fileCount;
	//		this->pFileObjectsBuffer = new FileObject[this->fileCount];
	//		this->pSequenceSetFileObjectBuffer = new FileObject[this->fileCount];
	//	}
	//	return this->sequenceNames;
	//}

	std::string distanceMeasure::FileObjectManager::GetSpeciesIdentifier(const std::string& line, const int index)
	{
		return this->pNameStrategy->GetSpeciesIdentifier(this, line, index);
	}

	std::string distanceMeasure::FileObjectManager::GetSequenceNameAtIndex(const int i) const
	{
		return this->sequenceNames.at(i);
	}

	void distanceMeasure::FileObjectManager::TryAddingSequenceIdentifier(const std::string& id)
	{
		//if all of the sequences have not been added to vector
		if(static_cast<int>(this->sequenceNames.size()) < this->fileCount)
		{
			this->sequenceNames.push_back(id);
		}
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

		printf("Sequence (Organism) name not found in Fasta File Sequences... giving it whole line as name\n");
		return line;
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
			if(this->pFileObjectsBuffer[i].GetSequenceName() == name)
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
			if (this->pSequenceSetFileObjectBuffer[i].GetSequenceName() == name)
			{
				return &this->pSequenceSetFileObjectBuffer[i];
			}
		}
		printf("FileObject not found for '%s'\n", name.c_str());
		exit(0);
		return nullptr;
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
	int distanceMeasure::FileObjectManager::GetCurrentSetSequenceLength() const
	{
		return this->pSequenceSetFileObjectBuffer[0].GetSequenceSize();
	}

	FileObjectManager::~FileObjectManager()
	{
		delete[] this->pFileObjectsBuffer;
		delete[] this->pSequenceSetFileObjectBuffer;
		delete this->sp;
		delete this->pNameStrategy;
	}
}