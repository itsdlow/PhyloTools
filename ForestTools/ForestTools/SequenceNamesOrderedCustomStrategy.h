/******************************************************************************
DeAngelo Wilson
April 14 2020

			FileObjectManager - Determine Sequence Names(strategy)
******************************************************************************/


#ifndef _SequenceNamesOrderedCustomStrategy
#define _SequenceNamesOrderedCustomStrategy

#include "SequenceNamesStrategy.h"

namespace distanceMeasure
{
	//Sequence Names INPUT STRATEGY*******
	class SequenceNamesOrderedCustomStrategy : public SequenceNamesStrategy
	{
	public:
		SequenceNamesOrderedCustomStrategy() = default;
		virtual ~SequenceNamesOrderedCustomStrategy() = default;
		SequenceNamesOrderedCustomStrategy(const SequenceNamesOrderedCustomStrategy&) = delete;
		SequenceNamesOrderedCustomStrategy& operator=(const SequenceNamesOrderedCustomStrategy&) = delete;

		virtual void FillSequenceNamesVector(std::vector<std::string>& sequenceNames, std::vector<std::string>& currentSequenceNames, int& sequenceCount, FileObject*& pFileObjectBuffer) override;
		std::string GetSpeciesIdentifier(FileObjectManager * const pFOM, const std::string& line, const int index) const override;

	private:

	};
}


#endif // !_SequenceNamesOrderedCustomStrategy