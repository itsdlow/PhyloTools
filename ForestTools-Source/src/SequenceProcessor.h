/******************************************************************************
DeAngelo Wilson
January 3 2020

						SequenceProcessor (strategy -- base)
******************************************************************************/

#ifndef _SequenceProcessor
#define _SequenceProcessor


#include <string>


namespace distanceMeasure
{
	//forward declarations
	class FileObject;
	class FileObjectManager;

	class SequenceProcessor
	{
	public:

		enum class SequencesInputFormat { FASTA, NEXUS };

		
		//BIG 4
		SequenceProcessor() = default;
		SequenceProcessor(const SequenceProcessor&) = delete;
		SequenceProcessor& operator=(const SequenceProcessor&) = delete;
		virtual ~SequenceProcessor() = default;

		//helper functions
		//virtual void CreateFileObjects(FileObject* pCurrentFileObject, const std::string& dir, const int sequenceCount) = 0;
		virtual void CreateFileObjects(FileObjectManager* pFOM, FileObject* const pCurrentFileObject) = 0;
		virtual void InitializationComplete(FileObjectManager* pFOM) {}

	private:


	};
}

#endif // !_SequenceProcessor
