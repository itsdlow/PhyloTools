/******************************************************************************
DeAngelo Wilson
January 3 2020

						SequenceFileProcessor (strategy -- Derived)
******************************************************************************/

#ifndef _SequenceFileProcessor
#define _SequenceFileProcessor

#include "SequenceProcessor.h"

namespace distanceMeasure
{

	class SequenceFileProcessor : public SequenceProcessor
	{
	public:


		//BIG 4
		SequenceFileProcessor() = default;
		SequenceFileProcessor(const SequenceFileProcessor&) = delete;
		SequenceFileProcessor& operator=(const SequenceFileProcessor&) = delete;
		~SequenceFileProcessor() = default;


		//helper functions
		//void CreateFileObjects(FileObject* pCurrentFileObject, const std::string& dir, const int sequenceCount) override;
		int CreateFileObjects(const FileObjectManager* pFOM, FileObject* const pFileObjectsBuffer) override;

		enum class SequencesProcessingStatus{NO_MORE_SEQUENCES, MORE_SEQUENCES, ERROR};
		SequencesProcessingStatus create_file_object(std::ifstream& fasta_input, std::string& annotation_line, const FileObjectManager* pFOM, FileObject* const pFileObject, int& maxSequence_ref) const;
	private:


	};
}

#endif // !_SequenceFileProcessor

