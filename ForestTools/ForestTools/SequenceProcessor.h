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


	class SequenceProcessor
	{
	public:

		//BIG 4
		SequenceProcessor() = default;
		SequenceProcessor(const SequenceProcessor&) = delete;
		SequenceProcessor& operator=(const SequenceProcessor&) = delete;
		~SequenceProcessor() = default;

		//helper functions
		virtual void CreateFileObjects(FileObject* pCurrentFileObject, const std::string& dir) = 0;

	private:


	};
}

#endif // !_SequenceProcessor
