/******************************************************************************
DeAngelo Wilson
January 3 2020

                        ForestTools (main)
******************************************************************************/


#include <iostream>
#include <string>
//NOTE::
#include <fstream>

#include "DistanceMatrixObject.h"
#include "RunFlags.h"
#include "SequenceListsGenerator.h"

#include "SequenceNamesStrategy.h"
#include "SequenceNamesUnorderedFastaStrategy.h"
#include "SequenceNamesOrderedCustomStrategy.h"
#include "SequenceNamesDescriptionStrategy.h"
#include "SequenceNamesIDStrategy.h"

//singletons...
#include "SystemParameters.h"
#include "CalculatorFactory.h"
//#include "DistanceMeasureCalculator.h"
//#include "PValueDistanceCalculator.h"
//#include "LcsDistanceCalculator.h"
//#include "MrBayesDistanceCalculator.h"
//#include "NcdDistanceCalculator.h"
//#include "BatchDistanceCalculators.h"


constexpr auto ASCII_INTEGER_DIFFERENCE = 48;

//forward decl
void TryClearingTempFiles();
distanceMeasure::DistanceMeasureCalculator* GetDistanceCalculator();
std::string SetSequenceListsFile(int batch_flag, const distanceMeasure::DistanceMatrixObject& dmo);
distanceMeasure::SequenceNamesStrategy* GetNamingStrategy();

//returns path to initial-full FASTA file
std::string GetOriginalFastaInputPath();

//TODO::
    // 0) Refactor LcsCalculator code*
    // 3) How to get amino acid sequences
    // 5) GUARDS!!!
            //safe-failing/execption safe
            //Matrixes not created...
                //Bad file names?
                //failed alignment?
            //Trees not created...
                //failed fastme?
    // 6) FileObject::sequence_size --> long -- allow for 3billion bp sequences
    

/*****   (using sequence data) create matrix --> tree (fastme)   ****/
int main()
{
    std::cout << "Hello World!\n";
    SystemParameters::InitializeSystemDependentCommands();
	
    TryClearingTempFiles();
	
    const std::string sequence_dir = GetOriginalFastaInputPath();



	//set sequenceNames strategy --> pass to dmo -> fom (instead of sequence_names path)
    distanceMeasure::SequenceNamesStrategy* name_strategy = GetNamingStrategy();

	distanceMeasure::DistanceMeasureCalculator* dmc = GetDistanceCalculator();

	//get sequenceList type
    int batch_flag;
    printf("Batch Run (0), Give SequenceLists File (1), Single Tree on a all sequences (2)\n");
    printf("Sequence Lists Method Number: ");
    //file:: string of all sequence set combinations --> matrices to create
    std::cin >> batch_flag;
	
    std::string tree_sequences_list;
    if (batch_flag == 1)
    {
	    printf("Enter path to Sequence List file:");
	    //file:: string of all sequence set combinations --> matrices to create
	    std::cin >> tree_sequences_list;
    }
	//TODO::CHECK IF USER WANTS CLUSTERING

	
	
	//GET ALL USER INPUT BEFORE CONSTRUCTING dmo---------------------------------------------------------------------------
    //intializes / prepares all file objects (sequences) -- given a DistanceMeasureCalculator.
    distanceMeasure::DistanceMatrixObject dmo(name_strategy, sequence_dir, dmc);
	
    //CREATE SEQUENCE LISTS FILE
    SystemParameters::Initialize(dmo.getFileObjectManager().get_file_count());
    if(batch_flag != 1)
    {
	    tree_sequences_list = SetSequenceListsFile(batch_flag, dmo);
    }

  /*********************************************************************************************
        for each entry in tree_sequence_list -- calculateDistanceMeasures + AllQuartetsMatrix + TREES...
  *********************************************************************************************/
    dmo.batch_matrix_calculation(tree_sequences_list);
	/*
	 *  Trees calculated + analyzed...
	 */
    SystemParameters::Terminate();
}


void TryClearingTempFiles()
{
    int clear_temp_files_flag;
    printf("If you are using new/different data, temporary files must be removed\n");
    printf("Would you like to clear TempFiles? No (0), Yes (1)\n");
    std::cin >> clear_temp_files_flag;

    if (clear_temp_files_flag)
    {
        char clean_mrbayes_dir_cmd[200];
        char clean_temp_dir_cmd[200];

    	//note:: extract to system paramters...
        sprintf(clean_mrbayes_dir_cmd, SystemParameters::GetCleanDirectoryCommandString().c_str(), SystemParameters::GetMrBayesFilesDirectory().c_str());
        sprintf(clean_temp_dir_cmd, SystemParameters::GetCleanDirectoryCommandString().c_str(), SystemParameters::GetTempFilesDirectory().c_str());

        //remove files from MrBayes + TempFiles
        system(clean_mrbayes_dir_cmd);
        system(clean_temp_dir_cmd);

    }
}

std::string GetOriginalFastaInputPath()
{
    std::string sequence_dir;
    //receive relative/absolute path of file or directory of sequences to analyze
    //TODO:: REASLISTICALLy --  Use absolute path of directory (allow for changing of default directory?
    //
    //TODO:: ALLOW multiple fasta sequence paths to be given -- concatenate those together for 1
        //TODO::Should change to implicitly concatenate FASTA files -- if more than one path given
    int multiple_fasta_inputs_flag;
    printf("Is your FASTA data in 1 file? No (0), Yes (1)\n");
    std::cin >> multiple_fasta_inputs_flag;

    if (multiple_fasta_inputs_flag)
    {
        printf("Path to FASTA file: ");
        std::cin >> sequence_dir;
    }
    else
    {
        //printf("Please enter, separated by spaces, all your FASTA file paths:\n");
        //std::string fasta_paths;
        //ask user for file name
        //std::cin >> fasta_paths;
        //concatenate all inputs together --> creating 1 FASTA file --> pass to dmo
            //should allow all input paths on 1 line -> parse by looking for space
        int more_input_flag = 1;
        std::string fasta_paths;
    	while(more_input_flag)
        {
            std::string path;
        	//ask user for file name
            printf("Path to Sequences: ");
            std::cin >> path;
            fasta_paths.append(path + " ");
    		
            printf("Do you have more FASTA file inputs? No (0), Yes (1)\n");
            std::cin >> more_input_flag;
        }//should be done by user in 1 line

        //*********************************************************************************************
        //DO AFTER TAKING IN ALL USER INPUT (after asking batch sequenceLists...)
        //concatenate
        std::string original_fasta_path(SystemParameters::GetTempFilesDirectory());
        original_fasta_path.append("/original.fasta");
        //open file to append
        std::ofstream original_fasta_file(original_fasta_path, std::ios_base::binary);

        if (original_fasta_file.is_open())
        {
            //go through entire string
            std::string path;
            for (auto it = fasta_paths.begin(); it != fasta_paths.end(); it++)
            {
                //if break in string
                if (*it == ' ')
                {
                    //open path file -- read + append to new FASTA
                    std::ifstream intermediary_file(path, std::ios_base::binary);
                	if(intermediary_file.is_open())
                	{
                		path.clear();
	                    //of_a.seekp(0, std::ios_base::end);//NOT NEEDED? -- never closing file until finished...
	                    original_fasta_file << intermediary_file.rdbuf();	
                	}
                    else
                    {
                        printf("Could not open FASTA file at '%s'\n", path.c_str());
                        exit(0);
                    }
                }
                else
                {
                    path.push_back(*it);
                }
            }
        }
        original_fasta_file.close();
        sequence_dir = original_fasta_path;
    }
    return sequence_dir;
}

distanceMeasure::DistanceMeasureCalculator* GetDistanceCalculator()
{
    int calc_method;
    distanceMeasure::RunFlags* flags = new distanceMeasure::RunFlags();
	//NOTE:: SHOULD SET FLAG PARAMETERS THROUGH USER INPUT...
		//quartets? calc type bitmask, clustering?

    distanceMeasure::DistanceMeasureCalculator* dmc = nullptr;
    printf("%s\nMatrix Calculation Method Number: ", distanceMeasure::CalculatorFactory::Dump().c_str());
    std::cin >> calc_method;

    int batch_calculator_index;
	distanceMeasure::CalculatorFactory::GetBatchCalculatorIndex(batch_calculator_index);
	//check if bitmask should be set
	if(calc_method == batch_calculator_index)
    {
        printf("==============================================================\n");
		//determine calculators to use
			//NOTE:: type of analysis determined by batch calc + quartet generation
        ////BATCH NCD (multiple NCD calculations)
        //---> adjust BatchCalculators to be able to run just_all types of NCD
        std::string batch_methods;
        unsigned int bitmask = 0;

        printf("Enter each method number that you would like to generate a tree with.\n");
        printf("%s\nMatrix Calculation Method Number: ", distanceMeasure::CalculatorFactory::Dump().c_str());

        //NOT IMPLEMENTED :: batch calculator bitmask -- allow for entering all '#' for calculators to analyze (1234 == 0 --> all calcs)
        std::cin >> batch_methods;
        for (auto it = batch_methods.begin(); it != batch_methods.end(); it++)
        {
            //for each char set corresponding bit position
                //convert calc number/name ==> calc bitmask (find calc -> get bitmask)
            int index = *it - ASCII_INTEGER_DIFFERENCE;
			if(index == batch_calculator_index)
			{
				//BITMASK FOR ALL CALCULATORS...
				bitmask |=  SystemParameters::GetAllCalculatorsMask();
			}
			else
			{
			    //bit mask for given-current (*it) calculator
			    bitmask |= SystemParameters::GetCalculatorMask(index);
			}
        }
        //calc count == size of batch_method string (unless all methods...)
		//  || total number of bits set in mask
        unsigned int mask = bitmask;
        int bit_count = 0;
		while (mask != 0)
        {
            const unsigned int bit = mask & 1;
            if (bit == 1)
            {
                bit_count++;
            }
            mask >>= 1;
        }

        //get calculator count...
        flags->calculators_bitmask = bitmask;
        //set after checking bit mask

		if(bit_count < 1)
        {
            //batch calc needs 2+ calcs for analysis
            printf("ERROR --> Switching to defaults\n");
				//===> TODO:: Give option to turn off analysis
            exit(0);
        }
		//get number of methods included in bitmask
        flags->calculator_count = bit_count;
    }
	
	//NOTE:: --> ASSUMPTION TAKEN --> (otherwise must check flag in BatchCalculatorAnalyzer)
    int quartets_gen_flag = 1;
	printf("Would you like to generate All quartet trees on the Sequence Sets?\n");
    printf("No (0), Yes (1)\n");
    std::cin >> quartets_gen_flag;
	
    bool generate_quartets = true;
	if(quartets_gen_flag < 1)
	{
        generate_quartets = false;
	}//ew
    //set calc run flags blackboard
	flags->generate_quartets = quartets_gen_flag;

	
    //CLUSTERING???
    int clustering_flag;
    printf("Would you like to an additional tree with clustered like sequences?\n");
	//NOTE:: CANNOT PERFORM ANALYSIS ON TREEs that have been clustered -- clusters can vary from method to method...
    printf("None (0), Strict/Less clusters (1), Loose/More clusters (2)\n");
    std::cin >> clustering_flag;
    float closeness_limit = 0.0f;
    switch(clustering_flag)
    {
    //case 0:
    //	//identical sequences are grouped into single cluster
    //    closeness_limit = 0.0f;
    //    break;
    case 1:
        //sequences that are nearly identical grouped into  single cluster
        closeness_limit = 0.1f;
        break;
    case 2:
        //sequences that are nearly identical grouped into  single cluster
        closeness_limit = 0.2f;
        break;
    default:
        break;
    }
	//set calc run flags blackboard
    flags->closeness_factor = closeness_limit;

	
	//NOTE:: CANNOT CREATE CALCULATOR UNTIL ALL -- RUN FLAGS -- NEEDED FOR CONSTRUCTION SET***
    dmc = distanceMeasure::CalculatorFactory::Create(calc_method, flags);
    printf("=============================================================================\n");

    return dmc;
	
}

std::string SetSequenceListsFile(int batch_flag, const distanceMeasure::DistanceMatrixObject& dmo)
{
    std::string tree_sequences_list;

    if (batch_flag == 0)
    {
        //generate BATCH SequenceLists file
        tree_sequences_list = distanceMeasure::SequenceListsGenerator::GenerateBatchSequenceListsFile(dmo.getFileObjectManager().GetCurrentSetNames());
    }
    else
    {
        //generate single tree gen SequenceLists file
        tree_sequences_list = distanceMeasure::SequenceListsGenerator::GenerateSequenceListFile(dmo.getFileObjectManager().GetCurrentSetNames());
    }
    return tree_sequences_list;
}

distanceMeasure::SequenceNamesStrategy* GetNamingStrategy()
{
    //TODO:: allow multiple ways
	/*
	 * (1) -- Default -- use whole taxa name line
	 * (2) Use accenssion number
	 * (3) supply SequenceNames file
	 * (4) supply ordered alternative names
	 */
    int names_type;

    printf("Description Line (0), Accession ID (1), Input FASTA Sequence Names file (2), Input Ordered Alternative Names (3)\n");
    printf("Sequence Names Type Number: ");
    std::cin >> names_type;
	std::string path;
    int count;
	
    distanceMeasure::SequenceNamesStrategy* strategy = nullptr;
    switch (names_type)
    {
        //default
    case 0:
        strategy = new distanceMeasure::SequenceNamesDescriptionStrategy();

        printf("Total number of sequences: ");
        std::cin >> count;
        strategy->SetSequenceCount(count);
        break;
    case 1:
        strategy = new distanceMeasure::SequenceNamesIDStrategy();

        printf("Total number of sequences: ");
        std::cin >> count;
        strategy->SetSequenceCount(count);
        break;
    case 2:
        strategy = new distanceMeasure::SequenceNamesUnorderedFastaStrategy();
    	
        printf("Path to sequence names file: ");
        std::cin >> path;
    	strategy->SetSequenceNamesPath(path);
        break;
    case 3:
        strategy = new distanceMeasure::SequenceNamesOrderedCustomStrategy();
    	
        printf("Path to sequence names file: ");
        std::cin >> path;
        strategy->SetSequenceNamesPath(path);
        break;
    default:
        break;
    }
	
    return strategy;
}

