/******************************************************************************
DeAngelo Wilson
April 14 2020

			FileObjectManager - Determine Sequence Names(strategy)
******************************************************************************/


#ifndef _SequenceNamesStrategy
#define _SequenceNamesStrategy

#include <string>
#include <vector>

//allow multiple ways
/*
 * (1) -- Default -- use whole taxa name line
 * (2) Use accenssion number
 * (3) supply SequenceNames file
 * (4) supply ordered alternative names
 */

namespace distanceMeasure
{
	class FileObject;
	class FileObjectManager;

	//Sequence Names INPUT STRATEGY*******
	class SequenceNamesStrategy
	{
	public:
	    SequenceNamesStrategy() = default;
	    virtual ~SequenceNamesStrategy() = default;
	    SequenceNamesStrategy(const SequenceNamesStrategy&) = delete;
	    SequenceNamesStrategy& operator=(const SequenceNamesStrategy&) = delete;

	    virtual void FillSequenceNamesVector(std::vector<std::string>& sequenceNames, std::vector<std::string>& currentSequenceNames, int& sequenceCount, FileObject*& pFileObjectBuffer) = 0;
		
		//use index for ordered sequence names file
		//use line for else
		virtual std::string GetSpeciesIdentifier(FileObjectManager * const pFOM, const std::string& line, const int index) const = 0;
		
		void SetSequenceNamesPath(const std::string& str) { this->sequenceNamesPath = str; };
		void SetSequenceCount(int count) { this->sequenceCount = count; };

		int GetSequenceCount() const { return this->sequenceCount; };
		std::string GetSequenceNamesPath() const { return this->sequenceNamesPath; };

		//given line with 0 - N underscores -- swap underscore with spaces
		static std::string swap_underscores_with_space(const std::string& str)
		{
			std::string res = str;
			for (auto it = res.begin(); it != res.end(); it++)
			{
				//swap underscore for space
				if (*it == '_')
				{
					*it = ' ';
				}
			}

			return res;
		}
	private:
		int sequenceCount;
	    std::string sequenceNamesPath;
	};
}


#endif // !_SequenceNamesStrategy