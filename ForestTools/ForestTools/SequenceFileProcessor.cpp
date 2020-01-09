/******************************************************************************
DeAngelo Wilson
January 3 2020*

						SequenceDirectoryProcessor (main)
******************************************************************************/


#include "SequenceFileProcessor.h"

#include "FileObject.h"
#include "FileObjectManager.h"

#include <fstream>

namespace distanceMeasure
{
	//TODO

	//given a single Fasta File as input -- containing "fileCount" many sequences
	void SequenceFileProcessor::CreateFileObjects(const FileObjectManager* pFOM, FileObject* const pFileObjectsBuffer)
	{
		//open file
		//read line 1 --> get file name
		//read line 2 pass sequence (line) to FileObject()

		//open file
		std::ifstream fastaInput(pFOM->GetPathToSequences());

		if (!fastaInput.is_open())
		{
			printf("File at path: %s - could not be opened\nFile Objects not to be created\n", pFOM->GetPathToSequences().c_str());
		}
		else
		{
			const int sequenceCount = pFOM->get_file_count();

			int count = 0;
			//read file
			std::string line;
			FileObject* pCurrentFileObject = pFileObjectsBuffer;
			
			std::getline(fastaInput, line);
			do
			{
				count++;
			} while (SequencesProcessingStatus::MORE_SEQUENCES == create_file_object(fastaInput, line, pFOM, pCurrentFileObject++) && count < sequenceCount);
			

			printf("finished processing file\n");
			fastaInput.close();
		}
	}

	SequenceFileProcessor::SequencesProcessingStatus distanceMeasure::SequenceFileProcessor::create_file_object(std::ifstream& fasta_input, std::string& annotation_line, const FileObjectManager* pFOM, FileObject* const pFileObject) const
	{
		SequencesProcessingStatus more_sequences_status = SequencesProcessingStatus::MORE_SEQUENCES;
		std::string line(annotation_line);
		std::string speciesSequence;
		speciesSequence.reserve(10000);

		//get species name from File_Object_Manager
		const std::string speciesName(pFOM->CheckForSequenceName(line));
		
		//put together species_sequence --- 1 - ??? lines long
		while (std::getline(fasta_input, line) && line.front() != '>')
		{
			speciesSequence.append(line);	
		}
		
		if(fasta_input.eof())
		{
			more_sequences_status = SequencesProcessingStatus::NO_MORE_SEQUENCES;
		}
		else
		{
			//pass-back next_organism name
			annotation_line = line;
		}

		printf("SpeciesName::%s-\n", speciesName.c_str());
		//create file object with file name and sequence string
		FileObject* tmp = new(pFileObject) FileObject(speciesSequence, speciesName);//...not really const function... FileObjectManager:: unsafe pointer 
		printf("created FileObject\n");

		return more_sequences_status;
	}

	/*
	 *

	 //get filename (species name) MUST BE 2 WORDS TO BE SET PROPERLY
					//1st whitespace -- 3rd
				//start of species name
				size_t species_start_index = line.find_first_of(" ", 0u) + 1u;
				//printf("start:: %d\n", species_start_index);

				//middle of species name (space index)
				size_t species_mid_index = line.find_first_of(" ", species_start_index);
				//printf("mid:: %d\n", species_mid_index);

				//index of last letter of species name
				size_t species_end_index = line.find_first_of(" ", species_mid_index + 1u) - 1u;
				//printf("end:: %d\n", species_end_index);



				std::string speciesName = line.substr(species_start_index, species_mid_index - species_start_index) + "_" + line.substr(species_mid_index + 1u, species_end_index - species_mid_index);

				//BUG: cheat implementation -- replace using sequence_names_list
				//HARD CODE RETRIEVAL OF 3rd word (seperated by space) FOR..
				//Eulemur_macaco_macaco
				//Ovis_orientalis_ophion

				//if sepecies is either ^^ --> retrieve 3rd word_name
	if (speciesName.substr(0u, 3u) == "Ovi" || speciesName.substr(0u, 3u) == "Eul")
	{
		size_t third_word_start = species_end_index + 2u;
		//get 3rd spaced word and append
		size_t species_end_end_index = line.find_first_of(" ", third_word_start) - 1u;
		speciesName += ("_" + line.substr(third_word_start, species_end_end_index - (species_end_index + 1u)));
	}

	 				printf("SpeciesName::%s-\n", speciesName.c_str());

	 * 
	 */

}