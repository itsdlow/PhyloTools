/******************************************************************************
DeAngelo Wilson
April 16 2020

			FileObjectManager - Determine Sequence Names(strategy)
******************************************************************************/

#include "SequenceNamesIDStrategy.h"

#include "FileObjectManager.h"
#include "FileObject.h"

namespace distanceMeasure
{
	void distanceMeasure::SequenceNamesIDStrategy::FillSequenceNamesVector(std::vector<std::string>& sequenceNames, std::vector<std::string>& currentSequenceNames, int& sequenceCount, FileObject*& pFileObjectBuffer)
	{
		sequenceCount = this->GetSequenceCount();
		pFileObjectBuffer = new FileObject[this->GetSequenceCount()];

		//sequenceNames filled whilst CreateFileObjects
	}

	std::string distanceMeasure::SequenceNamesIDStrategy::GetSpeciesIdentifier(FileObjectManager* const pFOM, const std::string& line, const int index) const
	{
		std::string id = SequenceNamesIDStrategy::GetAccessionNumber(line);
		id = SequenceNamesStrategy::swap_underscores_with_space(id);
		//if not in SequenceNames add it
		pFOM->TryAddingSequenceIdentifier(id);

		return id;
	}

	std::string distanceMeasure::SequenceNamesIDStrategy::GetAccessionNumber(const std::string& line)
	{
		std::string id;
		//exclude starting '>' character on FASTA description line
			//append chars until a space " " is reached
		for (auto it = line.begin() + 1; *it != ' '; ++it)
		{
			id.push_back(*it);
		}
		
		return id;
	}
}
