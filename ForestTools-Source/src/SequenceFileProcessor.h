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
		~SequenceFileProcessor() override = default;


		//helper functions
		//void CreateFileObjects(FileObject* pCurrentFileObject, const std::string& dir, const int sequenceCount) override;
		void CreateFileObjects(FileObjectManager* pFOM, FileObject* const pFileObjectsBuffer) override;

	private:
		enum class SequencesProcessingStatus{NO_MORE_SEQUENCES, MORE_SEQUENCES, PROCESSING_ERROR};

		SequencesProcessingStatus create_file_object(std::ifstream& fasta_input, std::string& annotation_line, FileObjectManager* pFOM, FileObject* const pFileObject, const int index) const;


	};
}

#endif // !_SequenceFileProcessor

