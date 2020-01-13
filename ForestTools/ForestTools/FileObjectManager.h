/******************************************************************************
DeAngelo Wilson
January 3 2020*

						FileObjectManager
******************************************************************************/


#ifndef _FileObjectManager
#define _FileObjectManager

	#include "SequenceProcessorType.h"

	#include <string>
	#include <vector>

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
			FileObjectManager(int fileCount, const std::string& sequence_names_path, const std::string& path);

			//helper functions
			//void fillFileObjectsBuffer(std::string path);

			
			//accessor functions
			const FileObject* getFileObjectsAddr() const;
			const int get_file_count() const;
			const std::string GetPathToSequences() const;

			const FileObject* const GetFileObject(const std::string& name) const;
			
			std::string CheckForSequenceName(const std::string& line) const;
		private:
			const int fileCount;
			FileObject* const pFileObjectsBuffer;

			//object that interacts with windows directory || given file -- of sequences
			SequenceProcessor* sp;

			//path to sequence file/directory
			std::string sequencesPath;
			std::vector<std::string> sequenceNames;

			//private helpers
			void FillSequenceNamesVector(const std::string& sequence_names_path);
		};

	}
#endif // !_FileObjectManager
