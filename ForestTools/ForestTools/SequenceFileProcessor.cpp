/******************************************************************************
DeAngelo Wilson
January 3 2020*

						SequenceDirectoryProcessor (main)
******************************************************************************/


#include "SequenceFileProcessor.h"

#include "FileObject.h"

#include <fstream>

namespace distanceMeasure
{

	//given a single Fasta File as input -- containing "fileCount" many sequences
	void SequenceFileProcessor::CreateFileObjects(FileObject* pFileObjectsBuffer, const std::string& path, const int sequenceCount)
	{
		//open file
		//read line 1 --> get file name
		//read line 2 pass sequence (line) to FileObject()

		//open file
		std::ifstream fastaInput(path);

		if (!fastaInput.is_open())
		{
			printf("File at path: %s - could not be opened\nFile Object not created\n", path.c_str());
		}
		else
		{
			//start = 4 end = 22/23
			// Allivibrio fischeri 

			int count = 0;
			//read file
			std::string line;
			FileObject* pCurrentFileObject = pFileObjectsBuffer;
			while (std::getline(fastaInput, line) && count < sequenceCount)
			{
				//get filename (species name) MUST BE 2 WORDS TO BE SET PROPERLY
					//1st whitespace -- 3rd
				//start of species name
				int species_start_index = line.find_first_of(" ", 0) + 1;
				//printf("start:: %d\n", species_start_index);

				//middle of species name (space index)
				int species_mid_index = line.find_first_of(" ", species_start_index);
				//printf("mid:: %d\n", species_mid_index);

				//index of last letter of species name
				int species_end_index = line.find_first_of(" ", species_mid_index + 1) - 1;
				//printf("end:: %d\n", species_end_index);



				std::string speciesName = line.substr(species_start_index, species_mid_index - species_start_index) + "_" + line.substr(species_mid_index + 1, species_end_index - species_mid_index);

				/*************************************************/
				//HARD CODE RETRIEVAL OF 3rd word (seperated by space) FOR..
				//Eulemur_macaco_macaco
				//Ovis_orientalis_ophion

				//if sepecies is either ^^ --> retrieve 3rd word_name
				if (speciesName.substr(0, 3) == "Ovi" || speciesName.substr(0, 3) == "Eul")
				{
					int third_word_start = species_end_index + 2;
					//get 3rd spaced word and append
					int species_end_end_index = line.find_first_of(" ", third_word_start) - 1;
					speciesName += ("_" + line.substr(third_word_start, species_end_end_index - (species_end_index + 1)));
				}
				/*************************************************/

				printf("SpeciesName::%s-\n", speciesName.c_str());

				//get sequence (line 2)
				std::getline(fastaInput, line);
				//create file object with file name and sequence string
				FileObject* tmp = new(pCurrentFileObject++) FileObject(line, speciesName);
				printf("created FileObject\n");


				//this->pFileObjectsBuffer[count] = new FileObject(line, speciesName);
				count++;

			}
			printf("finsihed processing file\n");
			fastaInput.close();
		}
	}

}