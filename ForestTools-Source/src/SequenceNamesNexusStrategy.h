/******************************************************************************
DeAngelo Wilson
April 5 2021

			FileObjectManager - Determine Sequence Names(strategy)
******************************************************************************/


#ifndef _SequenceNamesNexusStrategy
#define _SequenceNamesNexusStrategy

#include "SequenceNamesStrategy.h"

namespace distanceMeasure
{
	//Sequence Names INPUT STRATEGY*******
	class SequenceNamesNexusStrategy : public SequenceNamesStrategy
	{
	public:
		SequenceNamesNexusStrategy(std::string path);
		virtual ~SequenceNamesNexusStrategy() = default;
		SequenceNamesNexusStrategy(const SequenceNamesNexusStrategy&) = delete;
		SequenceNamesNexusStrategy& operator=(const SequenceNamesNexusStrategy&) = delete;

		virtual void FillSequenceNamesVector(std::vector<std::string>& sequenceNames, std::vector<std::string>& currentSequenceNames, int& sequenceCount, FileObject*& pFileObjectBuffer) override;
		std::string GetSpeciesIdentifier(FileObjectManager* const pFOM, const std::string& line, const int index) const override;

	private:
		std::string path;

	};
}


#endif // !_SequenceNamesNexusStrategy