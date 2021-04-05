/******************************************************************************
DeAngelo Wilson
January 3 2020

						NexusFormatProcessor (strategy -- Derived)
******************************************************************************/

#ifndef _NexusFormatProcessor
#define _NexusFormatProcessor

#include "SequenceProcessor.h"

namespace distanceMeasure
{

	class NexusFormatProcessor : public SequenceProcessor
	{
	public:


		//BIG 4
		NexusFormatProcessor() = default;
		NexusFormatProcessor(const NexusFormatProcessor&) = delete;
		NexusFormatProcessor& operator=(const NexusFormatProcessor&) = delete;
		~NexusFormatProcessor() override = default;


		//helper functions
		//void CreateFileObjects(FileObject* pCurrentFileObject, const std::string& dir, const int sequenceCount) override;
		void CreateFileObjects(FileObjectManager* pFOM, FileObject* const pFileObjectsBuffer) override;

		virtual void InitializationComplete(FileObjectManager* pFOM) override;

	private:
		enum class SequencesProcessingStatus { NO_MORE_SEQUENCES, MORE_SEQUENCES, PROCESSING_ERROR };

		SequencesProcessingStatus create_file_object(std::ifstream& fasta_input, std::string& annotation_line, FileObjectManager* pFOM, FileObject* const pFileObject, const int index) const;


	};
}

#endif // !_NexusFormatProcessor

