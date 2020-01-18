/******************************************************************************
DeAngelo Wilson
January 3 2020*

						SequenceDirectoryProcessor (main)
******************************************************************************/


#include "SequenceDirectoryProcessor.h"

#include "FileObject.h"
#include "FileObjectManager.h"

#include <windows.h>
#include <fstream>


namespace distanceMeasure
{

	//WINDOWS SPECIFIC --> LEGACY -- RETIRED...

	
	void SequenceDirectoryProcessor::CreateFileObjects(const FileObjectManager* pFOM, FileObject* const pFileObjectsBuffer)
	{
		//windows.h get path_directory files impl
		WIN32_FIND_DATAA data2;
		HANDLE hFind;

		FileObject* pCurrentFileObject = pFileObjectsBuffer;

		const int sequenceCount = pFOM->get_file_count();
		std::string dirPath = pFOM->get_original_sequences_path();
		//get all files in dir
		dirPath.append("\\*");
		hFind = FindFirstFileA(dirPath.c_str(), &data2);//ignore first entry -- reference to self dir (".")

		printf("Reading FASTA files from directory '%s'\ncreating FileObjects...\n", dirPath.c_str());
	    if (hFind != INVALID_HANDLE_VALUE) 
	    {
			FindNextFileA( hFind, &data2 );//ignore second entry -- reference to parent dir ("..")

			int count = 0;
			//While -- more files in directory + fileObjects created < sequenceCount
			while( FindNextFileA( hFind, &data2) && count < sequenceCount )
			{
				printf("%s\n", data2.cFileName);			
				std::string fileNameWithExtension = std::string(data2.cFileName);
				
				//remove file (fasta) extension
				size_t extIndex = fileNameWithExtension.find_last_of(".");
				std::string fileName = fileNameWithExtension.substr(0, extIndex);

				//create string -- full path of FileObject, file
				std::string filePath = pFOM->get_original_sequences_path();
				filePath.append("\\");
				filePath.append(fileNameWithExtension);


				//TODO

				//place new fileObject into buffer
				FileObject* tmp = new(pCurrentFileObject++) FileObject(this->GetFileSequence(filePath), fileName);
				count++;
			}
	        FindClose( hFind );
	    }
		else
		{
			printf("SDP:: directory path not found -- invalid handle");
		}
		//***UNDER 1 second to create FILE OBJECTS < ~.008s***
	}


	const std::string SequenceDirectoryProcessor::GetFileSequence(const std::string& filepath)
	{
		//open file
		std::ifstream fastaInput(filepath);

		if (!fastaInput.is_open())
		{
			printf("File at path: %s - could not be opened\nFile Objects not to be created\n", filepath.c_str());
		}
		else
		{
			//TODO
			//read file and return sequence string
			//		READ SEQUENCE until --> EOF || '>' new 'annotation' (organism) line
		}
		return std::string();
	}
}