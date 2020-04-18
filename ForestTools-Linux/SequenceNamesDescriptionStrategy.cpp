/******************************************************************************
DeAngelo Wilson
April 16 2020

			FileObjectManager - Determine Sequence Names(strategy)
******************************************************************************/

#include "SequenceNamesDescriptionStrategy.h"

#include "FileObjectManager.h"
#include "FileObject.h"


namespace distanceMeasure
{
	void distanceMeasure::SequenceNamesDescriptionStrategy::FillSequenceNamesVector(std::vector<std::string>& sequenceNames, std::vector<std::string>& currentSequenceNames, int& sequenceCount, FileObject*& pFileObjectBuffer)
	{
		sequenceCount = this->GetSequenceCount();
		pFileObjectBuffer = new FileObject[this->GetSequenceCount()];

		//sequenceNames filled whilst CreateFileObjects
	}

	std::string distanceMeasure::SequenceNamesDescriptionStrategy::GetSpeciesIdentifier(FileObjectManager * const pFOM, const std::string& line, const int index) const
	{
		std::string description = line;
		this->clean_fasta_description_line(description);
		//removee commas
		//FastME --> TAXA Name must be less than 64 chars
		description = description.substr(0, 60);
		//if not in SequenceNames add it
		pFOM->TryAddingSequenceIdentifier(description);
		
		return description;
	}

	//replaces bad chars (_ || ,) with " " (space)
	void distanceMeasure::SequenceNamesDescriptionStrategy::clean_fasta_description_line(std::string& description_string) const
	{
		//remove '>' char from FASTA description
		description_string.erase(0, 1);

		//replace all spaces w/ "__underscores__"
		for (auto i = 0u; i < description_string.size(); i++)
		{
			//remove underscores and commas
			if (description_string.at(i) == '_' || description_string.at(i) == ',')
			{
				description_string.replace(i, 1u, 1u, ' ');
			}
		}
	}
}
