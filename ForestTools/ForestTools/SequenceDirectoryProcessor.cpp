/******************************************************************************
DeAngelo Wilson
January 3 2020*

						SequenceDirectoryProcessor (main)
******************************************************************************/


#include <windows.h>

#include "FileObject.h"

#include "SequenceDirectoryProcessor.h"

namespace distanceMeasure
{


	void SequenceDirectoryProcessor::CreateFileObjects(FileObject* pCurrentFileObject, const std::string& dir, const int sequenceCount)
	{
		//windows.h get path_directory files impl
		WIN32_FIND_DATAA data2;
		HANDLE hFind;

		std::string dirPath = std::string(dir);
		//get all files in dir
		dirPath.append("\\*");
		hFind = FindFirstFileA(dirPath.c_str(), &data2);//ignore first entry -- reference to self dir (".")

		printf("Reading FASTA files from directory '%s'\ncreating FileObjects...\n", dirPath.c_str());
	    if (hFind != INVALID_HANDLE_VALUE) 
	    {
			FindNextFileA( hFind, &data2 );//ignore second entry -- reference to parent dir ("..")

			int count = 0;
			//PREVENT ERROR --> Only go fileCount number files************
			while( FindNextFileA( hFind, &data2) && count < sequenceCount )
			{
				printf("%s\n", data2.cFileName);			
				std::string fileNameWithExtension = std::string(data2.cFileName);
				
				//remove file (fasta) extension
				size_t extIndex = fileNameWithExtension.find_last_of(".");
				std::string fileName = fileNameWithExtension.substr(0, extIndex);

				//create string -- full path of FileObject, file
				std::string filePath = std::string(dir);
				filePath.append("\\");
				filePath.append(fileNameWithExtension);
				//place new fileObject into buffer
				FileObject* tmp = new(pCurrentFileObject++) FileObject(filePath, fileName);
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

}