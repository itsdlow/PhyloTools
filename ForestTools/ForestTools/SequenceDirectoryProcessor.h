/******************************************************************************
DeAngelo Wilson
January 3 2020*

						SequenceDirectoryProcessor
******************************************************************************/

#ifndef _SequenceDirectoryProcessor
#define _SequenceDirectoryProcessor

#include "SequenceProcessor.h"

namespace distanceMeasure
{

	class SequenceDirectoryProcessor: public SequenceProcessor
	{
	public:

		//BIG 4
		SequenceDirectoryProcessor() = default;
		SequenceDirectoryProcessor(const SequenceDirectoryProcessor&) = delete;
		SequenceDirectoryProcessor& operator=(const SequenceDirectoryProcessor&) = delete;
		~SequenceDirectoryProcessor() = default;

		//helper functions
		void CreateFileObjects(FileObject* pCurrentFileObject, const std::string& dir, const int sequenceCount) override;

		//accessor functions


	private:


	};
}

#endif // !_SequenceDirectoryProcessor

