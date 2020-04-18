/******************************************************************************
DeAngelo Wilson
April 16 2020

			FileObjectManager - Determine Sequence Names(strategy)
******************************************************************************/

#include "SequenceNamesOrderedCustomStrategy.h"

#include <fstream>
#include "FileObjectManager.h"
#include "FileObject.h"

namespace distanceMeasure
{
	//CODE DUPLICATION:: should create base class for "InputNamesFileStrategy"
	void distanceMeasure::SequenceNamesOrderedCustomStrategy::FillSequenceNamesVector(std::vector<std::string>& sequenceNames, std::vector<std::string>& currentSequenceNames, int& sequenceCount, FileObject*& pFileObjectBuffer)
	{
		//open names file
		std::ifstream sequenceNamesInput(this->GetSequenceNamesPath());

		if (!sequenceNamesInput.is_open())
		{
			printf("File at path: %s - could not be opened\nSequence names could not be retrieved\n", this->GetSequenceNamesPath().c_str());
			exit(0);
		}
		//fill sequenceNames vector with organisms names
		else
		{
			int count = 0;
			//read file
			std::string line;
			while (std::getline(sequenceNamesInput, line))
			{
				//add name ("line") to vector
				sequenceNames.push_back(line);
				count++;
			}
			//SET FILE COUNT BASED OFF sequence names count
			sequenceCount = count;
			pFileObjectBuffer = new FileObject[count];
		}
		currentSequenceNames = sequenceNames;
	}


	std::string distanceMeasure::SequenceNamesOrderedCustomStrategy::GetSpeciesIdentifier(FileObjectManager * const pFOM, const std::string& line, const int index) const
	{
		//unsafe does not check if name is correctly aligned with proper sequence
		return pFOM->GetSequenceNameAtIndex(index);
	}
}
