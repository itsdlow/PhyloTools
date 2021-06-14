/******************************************************************************
DeAngelo Wilson
April 14 2020

			FileObjectManager - Determine Sequence Names(strategy)
******************************************************************************/


#ifndef _SequenceNamesUnorderedFastaStrategy
#define _SequenceNamesUnorderedFastaStrategy

#include "SequenceNamesStrategy.h"

namespace distanceMeasure
{
	//Sequence Names INPUT STRATEGY*******
	class SequenceNamesUnorderedFastaStrategy : public SequenceNamesStrategy
	{
	public:
		SequenceNamesUnorderedFastaStrategy() = default;
		virtual ~SequenceNamesUnorderedFastaStrategy()= default;
		SequenceNamesUnorderedFastaStrategy(const SequenceNamesUnorderedFastaStrategy&) = delete;
		SequenceNamesUnorderedFastaStrategy& operator=(const SequenceNamesUnorderedFastaStrategy&) = delete;

		virtual void FillSequenceNamesVector(std::vector<std::string>& sequenceNames, std::vector<std::string>& currentSequenceNames, int& sequenceCount, FileObject*& pFileObjectBuffer) override;
		std::string GetSpeciesIdentifier(FileObjectManager * const pFOM,const std::string& line, const int index) const override;

	private:

	};
}


#endif // !_SequenceNamesUnorderedFastaStrategy