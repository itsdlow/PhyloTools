/******************************************************************************
DeAngelo Wilson
January 3 2020*

						SequenceDirectoryProcessor (main)
******************************************************************************/


#include "NexusFormatProcessor.h"

#include "FileObject.h"
#include "FileObjectManager.h"

#include <fstream>
#include <cstring>

#include "SystemParameters.h"

/*ASSUMPIONS:
 *
 * File given in NEXUS format
 * ==> Set of whole sequences -- NOT sequence sets for each species...
 *
 * ==> Sequences data begins after the "MATRIX" keyword...
 * ==> Sequences are 1 line, including the "TaxLabel" seperated by whitespace
*/
namespace distanceMeasure
{

	void distanceMeasure::NexusFormatProcessor::InitializationComplete(FileObjectManager* pFOM)
	{
		pFOM->SetSequenceProcessingToDefault();
	}
	
	//given a single Fasta File as "fastaInput" -- containing "fileCount" many sequences --> writes to FileObjectsBuffer
	void NexusFormatProcessor::CreateFileObjects(FileObjectManager* pFOM, FileObject* const pFileObjectsBuffer)
	{
		//uses FOM
			//fileCount + CheckForName() + sequences_path
		//return value
		//int maxSequenceLength = 0;

		//open file
		//read line 1 --> get file name
		//read line 2 pass sequence (line) to FileObject()

		//open file
		std::string clean_path = SystemParameters::Trim(pFOM->get_sequence_set_path());
		std::ifstream nexusInput(clean_path);

		if (!nexusInput.is_open())
		{
			printf("File at path: %s - could not be opened\nFile Objects not to be created\n", pFOM->get_sequence_set_path().c_str());
			exit(0);
		}
		else
		{
			const int sequenceCount = pFOM->get_sequence_set_count();
			int count = 0;
			//read file
			std::string line;
			//clear FileObjectBuffer
			//memset(pFileObjectsBuffer, 0, sizeof(FileObject) * pFOM->get_file_count());
			FileObject* pCurrentFileObject = pFileObjectsBuffer;

			
			std::string target = "MATRIX";
			//Find the MATRIX keyword
			while(std::getline(nexusInput, line))
			{
				if (line.find(target) != std::string::npos) 
				{
					break;
				}
			}

			for (int i = 0; i < sequenceCount; i++)
			{
				create_file_object(nexusInput, line, pFOM, pCurrentFileObject++, i);
			}
			
			printf("finished processing file\n");
			nexusInput.close();
		}
	}

	NexusFormatProcessor::SequencesProcessingStatus distanceMeasure::NexusFormatProcessor::create_file_object(std::ifstream& fasta_input, std::string& annotation_line, FileObjectManager* pFOM, FileObject* const pFileObject, const int index) const
	{
		SequencesProcessingStatus more_sequences_status = SequencesProcessingStatus::MORE_SEQUENCES;
		std::string line;
		//get sequence + ID line
		while(std::getline(fasta_input, line) && line.empty())
		{
		}
		if (fasta_input.eof())
		{
			more_sequences_status = SequencesProcessingStatus::NO_MORE_SEQUENCES;
		}
		else
		{
			const std::string speciesIdentifier(pFOM->GetSpeciesIdentifier(line, index));

			
			
			std::string::iterator sequenceStart;
			//starting at end of speciesID --> find first non-whitespace --> start of sequence
			for(auto it = line.begin() + speciesIdentifier.size(); it != line.end(); ++it)
			{
				if (!isspace(*it))
				{
					sequenceStart = it;
					break;
				}
			}
			std::string speciesSequence = std::string(sequenceStart, line.end());

			std::string speciesFastaDescription = std::string(">");
			speciesFastaDescription.append(speciesIdentifier);

			printf("SpeciesName::%s\n", speciesIdentifier.c_str());

		#pragma warning( push )
		#undef new

			//create file object with file name and sequence string
			FileObject* tmp = new(pFileObject) FileObject(speciesFastaDescription, speciesSequence, speciesIdentifier);//...not really const function... FileObjectManager:: unsafe pointer 

		#pragma warning( pop )
			printf("created FileObject\n");

		}
		

		return more_sequences_status;
	}


}