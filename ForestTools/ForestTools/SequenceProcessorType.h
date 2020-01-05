/******************************************************************************
DeAngelo Wilson
January 3 2020

						SeqeunceProcessor Type (enum)
******************************************************************************/

#ifndef _SequenceProcessorType
#define _SequenceProcessorType

namespace distanceMeasure
{
	//enum to determine processing strategy -- 
		//File Processing -- all sequences within the 1, given file
		//Directory Processing -- all sequences ins seperate files within the same, given directory
	enum class SequenceProcessorType {FileProcessor, DirectoryProcessor};

}

#endif // !_SequenceProcessorType
