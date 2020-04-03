/**************************************************************************************************************************
DeAngelo Wilson
January 18 2020

					SequenceListsGenerator
						-- DistanceMEasureCalculators that require reformatting of FileObjects --> new_Fasta_file
								(i.e. aligned sequences -- each new sequence set)
**************************************************************************************************************************/

#ifndef _SequenceListsGenerator
#define _SequenceListsGenerator

#include <string>
#include <vector>


namespace distanceMeasure
{
	class FileObjectManager;

	class SequenceListsGenerator
	{
	public:
		SequenceListsGenerator() = default;
		~SequenceListsGenerator() = default;
		SequenceListsGenerator(const SequenceListsGenerator& ca) = delete;
		SequenceListsGenerator& operator=(const SequenceListsGenerator& ca) = delete;

		static std::string GenerateBatchSequenceListsFile(const std::vector<std::string>& names);

		static std::string GenerateSequenceListFile(const std::vector<std::string>& names);

	private:
		static void RecursivelyGenerateSequenceListSubsetHelper(const std::vector<std::string>& names, std::string& file_string, bool& done, std::vector<std::string>& subset_names, int index, int size, int& count, int maxCount, int subsetCountRatio);
		static void RecursivelyGenerateSequenceListSubset(const std::vector<std::string>& names, std::string& file_string, int index, int size, int maxCount, int subsetCountRatio);

		
		static std::string CreateSequenceListString(const std::vector<std::string>& names);

		//returns sequenceLists text file path
		static std::string WriteSequenceListsFile(const std::string& str);
	};
}

#endif


