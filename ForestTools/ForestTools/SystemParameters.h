/******************************************************************************
DeAngelo Wilson
January 24 2020

						System Parameters (singleton)
******************************************************************************/


//TODO
//Format strings for files.. reference rogers rb file

#ifndef _SystemParameters
#define _SystemParameters

#include <string>

class SystemParameters
{
public:
	//API interface
	
	//void Terminate();
	//void Initialize();

	//std::string GetFileFormatString() { return SystemParameters::Instance().format_string; };
	static const std::string& GetNexusFileFormatString() { return SystemParameters::Instance().nexus_path_format_string; };
	static const std::string& GetNexusHeaderFormatString() { return SystemParameters::Instance().nexus_header_format_string; };
	static char GetNexusGapChar() { return SystemParameters::Instance().nexus_gap_char; };
	static char GetNexusMissingChar() { return SystemParameters::Instance().nexus_missing_char; };
	static int GetMrBayesNRuns() { return SystemParameters::Instance().mrbayes_nruns; };
	static const std::string& GetNexusDataTypeString() { return SystemParameters::Instance().nexus_data_type_string; };
	static const std::string& GetMrBayesCommandString() { return SystemParameters::Instance().mrbayes_command_string; };
	static const std::string& GetMrBayesBlockString() { return SystemParameters::Instance().mrbayes_block_string; };

	static const std::string& GetFastaFileFormatString() { return SystemParameters::Instance().fasta_path_format_string; };

	static const std::string& GetAlignedFileFormatString() { return SystemParameters::Instance().aligned_path_format_string; };
	static const std::string& GetMuscleCommandString() { return SystemParameters::Instance().muscle_command_string; };

	static const std::string& GetLargeListMatrixFileFormatString() { return SystemParameters::Instance().large_list_matrix_path_format_string; };
	static const std::string& GetQuartetMatricesFileFormatString() { return SystemParameters::Instance().quartet_matrices_path_format_string; };

	static const std::string& GetLargeListTreeFileFormatString() { return SystemParameters::Instance().large_list_tree_path_format_string; };
	static const std::string& GetQuartetTreesFileFormatString() { return SystemParameters::Instance().quartet_trees_path_format_string; };

	static const std::string& GetFastmeCommandString() { return SystemParameters::Instance().fastme_command_string; };

	static const int GetCalculatorsCount() { return SystemParameters::Instance().CALCULATOR_COUNT; };
private:
	SystemParameters() = default;

	static SystemParameters& Instance()
	{
		if(!pInstance)
		{
			pInstance = new SystemParameters();
		}
		return *pInstance;
	}
	static SystemParameters* pInstance;

	//private members

	const int CALCULATOR_COUNT = 3;
	
	//Tree strings
	const std::string large_list_tree_path_format_string = "ForestFiles/Trees/LargeListTree%s_%zu_%d.newick";
	const std::string quartet_trees_path_format_string = "ForestFiles/Trees/QuartetTrees%s_%zu_%d.newick";
	const std::string fastme_command_string = "extra_tools\\fastme-2.1.5\\binaries\\fastme.exe -i %s -D %d -o %s";

	
	//internal (distance matrix)
	const std::string large_list_matrix_path_format_string = "ForestFiles/Matrices/LargeListMatrix%s_%zu_%d.txt";
	const std::string quartet_matrices_path_format_string = "ForestFiles/Matrices/QuartetMatrices%s_%zu_%d.txt";
	//fasta
	const std::string fasta_path_format_string = "ForestFiles/TempFiles/temp_%zu.fasta";
	//aligned
	const std::string aligned_path_format_string = "ForestFiles/TempFiles/temp_%zu.afa";
	//muscle command
	//WINDOWS DEPENDENCE
	const std::string muscle_command_string = "extra_tools\\muscle.exe -in %s -out %s";
	
	//MRBAYES
	const char nexus_gap_char = '-';
	const char nexus_missing_char = '?';
	const int mrbayes_nruns = 2;
	const std::string nexus_data_type_string = "DNA";
	//TODO:: add batch number to nexus_file format string???
	const std::string nexus_path_format_string = "ForestFiles/TempFiles/MrBayes/temp_%zu.nxs";
	const std::string nexus_header_format_string = "#NEXUS\n[comment... data, etc....]\n\n\nBEGIN data;\n\tDIMENSIONS NTAX=%zu NCHAR=%d;\n\tFORMAT DATATYPE = %s GAP = %c MISSING = %c;\n\tMATRIX\n";
	//Windows Dependence
	const std::string mrbayes_command_string = "extra_tools\\MrBayes-3.2.7-WIN\\bin\\mb.3.2.7-win64.exe %s";
	const std::string mrbayes_block_string = "ForestFiles/TempFiles/MrBayes/mrbayes_block.nxs";
};

#endif

