/******************************************************************************
DeAngelo Wilson
April 14 2020

			FileObjectManager - Determine Sequence Names(strategy)
******************************************************************************/


#ifndef _SequenceNamesIDStrategy
#define _SequenceNamesIDStrategy

#include "SequenceNamesStrategy.h"

namespace distanceMeasure
{
	//Sequence Names INPUT STRATEGY*******
	class SequenceNamesIDStrategy: public SequenceNamesStrategy
	{
	public:
		SequenceNamesIDStrategy() = default;
		virtual ~SequenceNamesIDStrategy() = default;
		SequenceNamesIDStrategy(const SequenceNamesIDStrategy&) = delete;
		SequenceNamesIDStrategy& operator=(const SequenceNamesIDStrategy&) = delete;

		virtual void FillSequenceNamesVector(std::vector<std::string>& sequenceNames, std::vector<std::string>& currentSequenceNames, int& sequenceCount, FileObject*& pFileObjectBuffer) override;
		std::string GetSpeciesIdentifier(FileObjectManager * const pFOM, const std::string& line, const int index) const override;

		static std::string GetAccessionNumber(const std::string& line);
		
		
	private:

	};
}


#endif // !_SequenceNamesIDStrategy