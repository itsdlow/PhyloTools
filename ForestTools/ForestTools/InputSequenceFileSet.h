/******************************************************************************
DeAngelo Wilson
April 5 2021

			InputSequenceFileSet 
******************************************************************************/
#ifndef _InputSequenceFileSet
#define _InputSequenceFileSet


#include "SequenceProcessorType.h"

namespace distanceMeasure
{
	class SequenceNamesStrategy;
}

struct InputSequenceFileSet
{
	InputSequenceFileSet()
		: formatting(distanceMeasure::InputSequencesFormatType::FASTA),
		namesStrategy(nullptr),
		sequenceListBatchFlag(100)//default case --> single, full tree sequence list
	{

	}

	std::string path;
	distanceMeasure::InputSequencesFormatType formatting;
	distanceMeasure::SequenceNamesStrategy* namesStrategy;
	int sequenceListBatchFlag;
	std::string sequenceListPath;
	std::string compareTreePath;

};


#endif
