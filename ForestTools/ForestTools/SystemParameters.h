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
	
	static void Terminate();
	static void InitializeSystemDependentCommands();
	static void InitializeCalculatorFactory();
	static void Initialize(int sequence_count, float sequenceListsSizeFractionLarge = .75f, float sequenceListsSizeFractionSmall = .5f, float sequenceListsCountFractionLarge = 0.1f, float sequenceListsCountFractionSmall = 0.1f);

	
	//used by PhyloTools
	static const std::string& GetCleanNewickRegEx() { return SystemParameters::Instance().clean_newick_regex; };
	
	//std::string GetFileFormatString() { return SystemParameters::Instance().format_string; };
	static void GetNexusFileString(char* buffer, const size_t sequence_count);
	const std::string& privGetNexusFileFormatString() const { return nexus_path_format_string; };
	static void GetNexusHeaderString(char* buffer, const size_t sequence_count, const int NCHAR);
	const std::string& privGetNexusHeaderFormatString() const { return nexus_header_format_string; };
	
	char privGetNexusGapChar() const { return nexus_gap_char; };
	char privGetNexusMissingChar() const { return nexus_missing_char; };
	static int GetMrBayesNRuns() { return SystemParameters::Instance().mrbayes_nruns; };
	static int GetMrBayesNChains() { return SystemParameters::Instance().mrbayes_nchains; };
	static int GetMrBayesNGen() { return SystemParameters::Instance().mrbayes_ngen; };

	
	static const std::string& GetNexusDataTypeString() { return SystemParameters::Instance().nexus_data_type_string; };
	static void SetNexusDataTypeString(const char* data_type) { SystemParameters::Instance().nexus_data_type_string = data_type; };

	
	static void GetMrBayesCommand(char* buffer, const char* batch_block_file_path);
	const std::string& privGetMrBayesCommandString() const { return mrbayes_command_string; };
	
	static const std::string& GetMrBayesBlockString() { return SystemParameters::Instance().mrbayes_block_string; };

	static const std::string& GetTempFilesDirectory() { return SystemParameters::Instance().temp_files_dir; };
	static const std::string& GetMrBayesFilesDirectory() { return SystemParameters::Instance().mrbayes_files_dir; };

	
	static void GetFastaFileString(char* buffer, const size_t sequence_count, const int hash_id);
	const std::string& privGetFastaFileFormatString() const { return fasta_path_format_string; };

	
	static void GetAlignedFileString(char* buffer, const int total_sequence_count, const size_t subsequence_count, const int hash_id);
	const std::string& privGetAlignedFileFormatString() const { return aligned_path_format_string; };

	static void GetCompressedFastaFileString(char* buffer, const int file_count);
	const std::string& privGetCompressedFastaFileFormatString() const { return compressed_fasta_path_format_string; };

	
	static void GetAlignmentCommandString(char* buffer, const char* in, const char* out);
	const std::string& privGetMuscleCommandFormatString() const { return muscle_command_string; };

	static const std::string& GetCleanDirectoryCommandString() { return SystemParameters::Instance().clean_dir_format_string; };


	static const std::string& GetClusteredMatrixFileFormatString() { return SystemParameters::Instance().clustered_matrix_path_format_string; };
	static const std::string& GetLargeListMatrixFileFormatString() { return SystemParameters::Instance().large_list_matrix_path_format_string; };
	static const std::string& GetQuartetMatricesFileFormatString() { return SystemParameters::Instance().quartet_matrices_path_format_string; };

	static const std::string& GetLargeListTreeFileFormatString() { return SystemParameters::Instance().large_list_tree_path_format_string; };
	static const std::string& GetQuartetTreesFileFormatString() { return SystemParameters::Instance().quartet_trees_path_format_string; };
	static const std::string& GetClusteredTreeFileFormatString() { return SystemParameters::Instance().clustered_tree_path_format_string; };

	
	static const std::string& GetFastmeCommandString() { return SystemParameters::Instance().fastme_command_string; };

	//DEBUG LOG getters
	static void GetTimingsLogFileString(char* buffer, const char* calculator_name, int total_sequence_count);
	const std::string& privGetTimingsLogFileFormatString() const { return timings_log_path_format_string; };

	static void GetSequenceSetTimingString(char* buffer, int batch_id, double calculation_time, const char* sequence_set_str);
	const std::string& privGetSequenceSetTimingFormatString() const { return sequence_set_timing_format_string; };
	
	static void GetSequenceSetAlignmentTimingString(char* buffer, int batch_id, double calculation_time, const char* sequence_set_str);
	const std::string& privGetSequenceSetAlignmentTimingFormatString() const { return sequence_set_alignment_timing_format_string; };

	static void GetAlignmentTimingsLogFileString(char* buffer, const char* calculator_name, int total_sequence_count);
	const std::string& privGetAlignmentTimingsLogFileFormatString() const { return alignment_timings_log_path_format_string; };	

	static const std::string& GetClosenessLimitLogFileFormatString() { return SystemParameters::Instance().closeness_limit_log_format_string; };

	static void GetAnalysisTableFilePath(char* buffer, const int sequence_count);
	const std::string& privGetAnalysisTableFileFormatString() const { return analysis_table_filepath; };
	
	static const std::string& GetBatchAnalysisTableFileFormatString() { return SystemParameters::Instance().batch_analysis_table_filepath; };

	//buffer sizes
	//static const int GetTreeFilePathSize() { return SystemParameters::Instance().tree_file_path_size; };
	//static const int& GetMatrixFilePathSize() { return SystemParameters::Instance().matrix_file_path_size; };

	//
	static void GetCompressedFilename(char* buffer, const char* extension);
	const std::string& privGetCompressedFilenameFormat() const { return compressed_file_format_string; };
	static void GetCompressionCommand(char* buffer, const char* derived_command, const char* out_file, const char* in_file);

	//static const std::string& GetMFCompressCommandString() { return SystemParameters::Instance().mfcompress_command_string; };
	static const std::string& GetMFC1CommandString() { return SystemParameters::Instance().mfc1_command_string; };
	static const std::string& GetMFC2CommandString() { return SystemParameters::Instance().mfc2_command_string; };
	static const std::string& GetMFC3CommandString() { return SystemParameters::Instance().mfc3_command_string; };

	static const std::string& Get7ZipCommandString() { return SystemParameters::Instance().zip7_command_string; };

	
	//***
	// Calculator Factory things...
	//***
	static int GetCalculatorsCount() { return SystemParameters::Instance().privGetCalculatorTypeCount(); };
	int privGetCalculatorTypeCount();

	static unsigned int GetCalculatorMask(int id);
	static unsigned int GetAllCalculatorsMask();

	
	static int GetMaxSequenceListSize() { return SystemParameters::Instance().max_sequence_list_size; };
	static int GetFractionOfMaxSequenceSize(float fraction) { return static_cast<int>(static_cast<float>(SystemParameters::Instance().max_sequence_list_size) * fraction); };
	static int GetMinSequenceListSize() { return SystemParameters::Instance().min_sequence_list_size;/* quartet size */ };

	static int GetSubsetSizeLarge() { return SystemParameters::Instance().subset_size_large; };
	static int GetSubsetSizeSmall() { return SystemParameters::Instance().subset_size_small; };
	static int GetSubsetCountSmall() { return SystemParameters::Instance().subset_count_small; };
	static int GetSubsetCountLarge() { return SystemParameters::Instance().subset_count_large; };
	static int GetSubsetCountRatioLarge() { return static_cast<int>(SystemParameters::Instance().subset_count_fraction_large * 100); };
	static int GetSubsetCountRatioSmall() { return static_cast<int>(SystemParameters::Instance().subset_count_fraction_small * 100); };
	
	static const std::string& GetSequenceListsFileString() { return SystemParameters::Instance().sequence_lists_filepath; };
	
private:
	SystemParameters();

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
	bool OS_WINDOWS;
	
	//const int CALCULATOR_COUNT = 5;

	
	/***************************************************
	 *				Sequence Generator Parameters
	 **************************************************/
	const int min_sequence_list_size = 4;
	int max_sequence_list_size;
	//subset sizes
	int subset_size_small;// = .5f;
	int subset_size_large;// = .75f;
	//subset sample size
	int subset_count_large;// = 128; --> "                                  " - .07033
	int subset_count_small;// = 1024; --> almost 8% of total combinations...  - .07956
		// + all quartets
	float subset_count_fraction_large;
	float subset_count_fraction_small;
	
	const std::string sequence_lists_filepath = "ForestFiles/SequenceLists.txt";
	
	//TODO:: Allow for user to supply directory path folder
		//create sub folders? i.e. 'Analysis'
			//use getter + setter to update to non-default...
	//const std::string default_analysis_tables_path = "ForestFiles/Analysis/";
	const std::string analysis_table_filepath = "ForestFiles/Analysis/AnalysisTables_%d.txt";
	const std::string batch_analysis_table_filepath = "ForestFiles/Analysis/BatchAnalysisTables_%d.txt";

	
	//DEBUG -- timings log
	const std::string timings_log_path_format_string = "ForestFiles/Logs/TimingsLog%s_%d.txt";
	const std::string sequence_set_timing_format_string = "Calculation Time For Sequence Set[%d]: %f minutes\n\t%s\n";
	const std::string alignment_timings_log_path_format_string = "ForestFiles/Logs/AlignmentTimingsLog%s_d.txt";
	const std::string sequence_set_alignment_timing_format_string = "Alignment Time For Sequence Set[%d]: %f minutes\n\t%s\n";

	const std::string closeness_limit_log_format_string = "ForestFiles/Logs/ClosenessLimitLog%s_%zu_%d.txt";
	
	//Tree strings
	//const int tree_file_path_size = 150;
	const std::string large_list_tree_path_format_string = "ForestFiles/Trees/LargeListTree%s_%zu_%d.newick";
	const std::string quartet_trees_path_format_string = "ForestFiles/Trees/QuartetTrees%s_%zu_%d.newick";
	const std::string clustered_tree_path_format_string = "ForestFiles/Trees/LargeListTree%s_%zu_%d_clustered.newick";
	//const int 
	std::string fastme_command_string;// = "extra_tools\\fastme-2.1.5\\binaries\\fastme.exe -i %s -D %d -o %s";

	
	//internal (distance matrix)
	//const int matrix_file_path_size = 150;
	const std::string large_list_matrix_path_format_string = "ForestFiles/Matrices/LargeListMatrix%s_%zu_%d.txt";
	const std::string quartet_matrices_path_format_string = "ForestFiles/Matrices/QuartetMatrices%s_%zu_%d.txt";
	const std::string clustered_matrix_path_format_string = "ForestFiles/Matrices/ClusteredMatrix%s_%zu_%d_clustered.txt";
	
	// -- allow user to supply directory of tempfiles
	const std::string temp_files_dir = "ForestFiles/TempFiles";
	//fasta
	const std::string fasta_path_format_string = "ForestFiles/TempFiles/temp_%zu_%d.fasta";
	const std::string compressed_fasta_path_format_string = "ForestFiles/TempFiles/temp_%zu.fasta";

	//aligned
	const std::string aligned_path_format_string = "ForestFiles/TempFiles/temp_%d_%zu_%d.afa";

	//muscle command
	std::string muscle_command_string;// = "extra_tools\\muscle.exe -in %s -out %s";

	//WINDOWS DEPENDENCE???
	//remove dir command
	std::string clean_dir_format_string;// = "rm -v %s/*";
	
	//NCD
	const std::string compressed_file_format_string = "ForestFiles/TempFiles/CompressedFile.%s";

	//MFCompress
	//NOTE::could merge to 1 command and create derived MFC command by asking USER which...
	//takes as parameters - compression_type (1-3) + output filename + Original Input FASTA Filename
	//const std::string mfcompress_command_string = "extra_tools\\MFCompress-win64-1.01\\MFCompress-win64-1.01\\MFCompressC64.exe -%d -o %s %s";
	std::string mfc1_command_string;// = "extra_tools\\MFCompress-win64-1.01\\MFCompress-win64-1.01\\MFCompressC64.exe -1 -o %s %s";
	std::string mfc2_command_string;// = "extra_tools\\MFCompress-win64-1.01\\MFCompress-win64-1.01\\MFCompressC64.exe -2 -o %s %s";
	std::string mfc3_command_string;// = "extra_tools\\MFCompress-win64-1.01\\MFCompress-win64-1.01\\MFCompressC64.exe -3 -o %s %s";

	//TODO:: intiialize all unix/windows commands in SystemParameters::Initialize -- after System determined...
	//params -- zipped_file_name input_file_path
	std::string zip7_command_string;// = "extra_tools\\7-Zip\\7z.exe a %s %s > nul";
	//const std::string zip7_command_string_unix = ".extra_tools/7-Zip/7z a %s %s > /dev/null";

	
	//MRBAYES
	const char nexus_gap_char = '-';
	//NOTE:: CAN ONLY SPECIFY 1 'missing' char --->
		//TODO:: create a ambigious character swap function --> clean sequences (IF NEEDED)
	//ASK FOR AMBIGUOUS CHARs...
	const char nexus_missing_char = 'B';
	const int mrbayes_nruns = 2;
	const int mrbayes_nchains = 4;
	const int mrbayes_ngen = 20000;
	//ASK USER FOR SEQ TYPE (RNA/DNA/Protein) (allow mixed?...)
	std::string nexus_data_type_string;// = "protein";

	
	//TODO:: add batch number to nexus_file format string??? -- allow user to supply directory of tempfiles
	std::string mrbayes_files_dir;// = "ForestFiles/TempFiles/MrBayes";

	const std::string nexus_path_format_string = "ForestFiles/TempFiles/MrBayes/temp_%zu.nxs";//TODO:: fix to use mrbayes_file_dir VARIABLE...
	const std::string nexus_header_format_string = "#NEXUS\n[comment... data, etc....]\n\n\nBEGIN data;\n\tDIMENSIONS NTAX=%zu NCHAR=%d;\n\tFORMAT DATATYPE = %s GAP = %c MISSING = %c;\n\tMATRIX\n";
	
	std::string mrbayes_command_string;// = "extra_tools\\MrBayes-3.2.7-WIN\\bin\\mb.3.2.7-win64.exe %s";
	const std::string mrbayes_block_string = "ForestFiles/TempFiles/MrBayes/mrbayes_block.nxs";

	//Phylotools
	std::string clean_newick_regex;
};


#endif

