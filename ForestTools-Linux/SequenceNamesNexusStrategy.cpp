/******************************************************************************
DeAngelo Wilson
April 5 2021

			FileObjectManager - Determine Sequence Names(strategy)
******************************************************************************/

#include "SequenceNamesNexusStrategy.h"

#include <fstream>


#include "SystemParameters.h"

#include "FileObjectManager.h"
#include "FileObject.h"


namespace distanceMeasure
{
	distanceMeasure::SequenceNamesNexusStrategy::SequenceNamesNexusStrategy(std::string path):
	path(path)
	{
	}
	
	void distanceMeasure::SequenceNamesNexusStrategy::FillSequenceNamesVector(std::vector<std::string>& sequenceNames, std::vector<std::string>& currentSequenceNames, int& sequenceCount, FileObject*& pFileObjectBuffer)
	{
		//open file and read nTax
				//open file
		std::string clean_path = SystemParameters::Trim(this->path);
		std::ifstream nexusInput(clean_path);

		if (!nexusInput.is_open())
		{
			printf("File at path: %s - could not be opened\nFile Objects not to be created\n", this->path.c_str());
			exit(0);
		}

		//read file
		std::string line;

		//TODO:: Extract sequence Count from Nexus header
		std::string target("NTAX=");
		//Find the NTAX keyword
		while (std::getline(nexusInput, line))
		{
			if (line.find(target) != std::string::npos)
			{
				break;
			}
		}

		size_t startIndex = line.find(target) + target.size();
		std::string nTax;
		for (auto it = line.begin() + startIndex; it != line.end(); ++it)
		{
			if (!isspace(*it))
			{
				//add to NTAX string
				nTax.push_back(*it);
			}
			else
			{
				break;
			}
		}
		const int nTaxa = std::stoi(nTax);
		this->SetSequenceCount(nTaxa);


		sequenceCount = this->GetSequenceCount();
		pFileObjectBuffer = new FileObject[this->GetSequenceCount()];
		//sequenceNames filled whilst CreateFileObjects
	}

	std::string distanceMeasure::SequenceNamesNexusStrategy::GetSpeciesIdentifier(FileObjectManager* const pFOM, const std::string& line, const int index) const
	{
		std::string speciesID;
		//read chars until white space
		for (auto it = line.begin(); it != line.end(); ++it)
		{
			if (!isspace(*it))
			{
				//add to NTAX string
				speciesID.push_back(*it);
			}
			else
			{
				break;
			}
		}

		//If current count of sequences is less than expected... + if not in SequenceNames -- add it?
		pFOM->TryAddingSequenceIdentifier(speciesID);

		return speciesID;
	}

}
