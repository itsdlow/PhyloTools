/******************************************************************************
DeAngelo Wilson
April 14 2020

			FileObjectManager - Determine Sequence Names(strategy)
******************************************************************************/


#ifndef _SequenceNamesDescriptionStrategy
#define _SequenceNamesDescriptionStrategy

#include "SequenceNamesStrategy.h"

namespace distanceMeasure
{
	//Sequence Names INPUT STRATEGY*******
	class SequenceNamesDescriptionStrategy : public SequenceNamesStrategy
	{
	public:
		SequenceNamesDescriptionStrategy() = default;
		virtual ~SequenceNamesDescriptionStrategy() = default;
		SequenceNamesDescriptionStrategy(const SequenceNamesDescriptionStrategy&) = delete;
		SequenceNamesDescriptionStrategy& operator=(const SequenceNamesDescriptionStrategy&) = delete;

		virtual void FillSequenceNamesVector(std::vector<std::string>& sequenceNames, std::vector<std::string>& currentSequenceNames, int& sequenceCount, FileObject*& pFileObjectBuffer) override;
		std::string GetSpeciesIdentifier(FileObjectManager * const pFOM, const std::string& line, const int index) const override;

	private:
		//duplicate code... in DistanceMatrixObject
		void clean_fasta_description_line(std::string& description_string) const;

	};
}


#endif // !_SequenceNamesDescriptionStrategy