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
		void CreateFileObjects(FileObject* pCurrentFileObject, const std::string& dir, const int sequenceCount) override;

	private:


	};
}

#endif // !_SequenceFileProcessor

