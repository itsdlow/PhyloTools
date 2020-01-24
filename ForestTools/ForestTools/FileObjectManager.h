/******************************************************************************
DeAngelo Wilson
January 3 2020*

						FileObjectManager
******************************************************************************/


#ifndef _FileObjectManager
#define _FileObjectManager

	//#include "SequenceProcessorType.h"

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
			FileObjectManager(const std::string& sequence_names_path, const std::string& path);

			//helper functions
			std::string CheckForSequenceName(const std::string& line) const;

			//
			void RefillFileObjectsBuffer(const std::vector<std::string>& sequence_set_names, std::string new_sequences_path);

			
			//accessor functions
			const FileObject* get_file_objects_addr() const;

			const FileObject* get_sequence_set_file_objects_addr() const;
			const int get_file_count() const;
			const int get_sequence_set_count() const;
			const std::string get_original_sequences_path() const;
			const std::string get_sequence_set_path() const;

			const FileObject* const GetFileObject(const std::string& name) const;
			
		private:
			int fileCount;

			FileObject* pFileObjectsBuffer;

			FileObject* pSequenceSetFileObjectBuffer;
			//object that interacts with windows directory || given file -- of sequences
			SequenceProcessor* sp;

			int sequenceSetCount;
			//path to sequence file/directory (original)
			std::string filePath;
			//path to current sequences
			std::string sequencesPath;
			std::vector<std::string> sequenceNames;
			std::vector<std::string> currentSequenceNames;

			
			//private helpers
			const std::vector<std::string> FillSequenceNamesVector(const std::string& sequence_names_path);
			void SetSequencesPath();
			void SetSequenceSetCount();
		};

	}
#endif // !_FileObjectManager
