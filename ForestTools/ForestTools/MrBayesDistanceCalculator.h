/******************************************************************************
DeAngelo Wilson
January 3 2020

						MrBayesDistanceCalculator (strategy -- derived)
******************************************************************************/
#ifndef _MrBayesDistanceCalculator
#define _MrBayesDistanceCalculator

#include "ExternalDistanceMeasureCalculator.h"
#include "CalculatorNexusFormatter.h"

namespace distanceMeasure
{
	class MrBayesDistanceCalculator : public ExternalDistanceMeasureCalculator, private CalculatorNexusFormatter
	{
	public:
		//BIG 4
		MrBayesDistanceCalculator() = default;
		MrBayesDistanceCalculator(const MrBayesDistanceCalculator&) = delete;
		MrBayesDistanceCalculator& operator=(const MrBayesDistanceCalculator&) = delete;
		virtual ~MrBayesDistanceCalculator() = default;
 
		void calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)  override;
		//void create_tree(const std::vector<std::string>& sequence_set_names, const int batch_id) override;

		std::string GetCalculatorName() const override;

	private:
		//batch running command creation func
		std::string create_mrbayes_default_command_block_file(const std::string& relative_nxs_path) const;
		void calculate_quartet_trees(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id);
		void calculate_large_list_tree(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const int batch_id);

		//given mrbayes_output_filename --> extract last gen'ed tree 
		static std::string ExtractMrBayesNewick(const std::string& t_file_name);//use r-value???
		static std::vector<std::string> CreateSubSequenceSet(const std::vector<std::string>& sequence_set_names, int i, int j, int k, int l);
		static std::string GetMrBayesTFileName(const std::string& relative_nxs_path);
		static std::vector<std::string> GetTFileKey(std::ifstream& tFile);
		static std::string GetTreeGenLine(std::ifstream& tFile);
		static std::string ParseTreeGenLine(const std::vector<std::string>& species_names_key, const std::string& tree_gen_line);//use r-value???
		static std::string GetKeySpeciesName(const std::vector<std::string>& species_names_key, size_t i);
		static std::string CleanKeyEntry(const std::string& key_entry);
	};

}

#endif // !_MrBayesDistanceCalculator
