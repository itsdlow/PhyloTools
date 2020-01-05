/******************************************************************************
DeAngelo Wilson
January 3 2020*

						FileObjectManager
******************************************************************************/


#ifndef _FileObjectManager
#define _FileObjectManager

	//#include "SequenceProcessor.h"
	#include <string>
	#include "SequenceProcessorType.h"

	namespace distanceMeasure
	{
		//forward declaration
		class SequenceProcessor;
		class FileObject;

		//Creates (in constructor) FileOjects array + inits with sequence driectory
		class FileObjectManager
		{
		public:
			//BIG 4
			FileObjectManager() = delete;
			~FileObjectManager();
			FileObjectManager(const FileObjectManager& tmp) = delete;
			FileObjectManager& operator=(const FileObjectManager& tmp) = delete;

			//custom constructor
			FileObjectManager(int fileCount, std::string path, SequenceProcessorType path_type);

			//helper functions
			//void fillFileObjectsBuffer(std::string path);

			//accessor functions
			const FileObject* getFileObjectsAddr() const;
			const int getFileCount() const;


		private:
			const int fileCount;
			//const????
			FileObject* const pFileObjectsBuffer;

			//buffer to write into output file
			//std::string results;
			//char* results;

			//input and output files

			//object that interacts with windows directory || given file -- of sequences
			SequenceProcessor* sp;
		};

	}
#endif // !_FileObjectManager
