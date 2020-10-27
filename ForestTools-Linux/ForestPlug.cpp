/**************************************************************************************************************************
DeAngelo Wilson
August 5 2020

					ForestPlug
**************************************************************************************************************************/

#include "ForestPlug.h"


#include "SystemParameters.h"
#include "CalculatorFactory.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "DistanceMatrixObject.h"
#include "RunFlags.h"

ForestPlug::ForestPlug():
poRunFlags(new distanceMeasure::RunFlags())
{
	SystemParameters::Initialize();
	//SystemParameters::InitializeSystemDependentCommands();
}

ForestPlug::~ForestPlug()
{
	delete this->poRunFlags;

    SystemParameters::Terminate();

}

void ForestPlug::run()
{
    std::cout << "Hello World!\n";

    TryClearingTempFiles();

    const std::string sequence_dir = this->GetOriginalFastaInputPath();

    //set sequenceNames strategy --> pass to dmo -> fom (instead of sequence_names path)
    distanceMeasure::SequenceNamesStrategy* name_strategy = GetNamingStrategy();

    distanceMeasure::DistanceMeasureCalculator* dmc = CreateDistanceCalculator();

	//Construct DMO and FileObjectManager -- (NOTE:: file-objects initialized in DistanceMatrixObject::run)
    distanceMeasure::DistanceMatrixObject dmo(name_strategy, sequence_dir, dmc);

	//get sequenceList type
	int batch_flag;
    printf("Batch Run (0), Give SequenceLists File (1), Single Tree on a all sequences (2)\n");
    printf("Sequence Lists Method Number: ");
    std::cin >> batch_flag;
	
    std::string tree_sequences_list;

	if(batch_flag == 1)
	{

	    printf("Enter path to Sequence List file:");
        //file:: string of all sequence set combinations --> matrices to create
        std::cin >> tree_sequences_list;
	}


    /*********************************************************************************************
          for each entry in tree_sequence_list -- calculateDistanceMeasures + AllQuartetsMatrix + TREES...
    *********************************************************************************************/
    dmo.run(batch_flag, tree_sequences_list);
    /*
     *  Trees calculated + analyzed...
     */	
}

void ForestPlug::TryClearingTempFiles()
{
    std::string input;
    printf("If you are using new/different data, temporary files must be removed\n");
    printf("Would you like to clear TempFiles? No (0), Yes (1)\n");
    //std::cin >> clear_temp_files_flag;
    std::getline(std::cin, input, '\n');
    const int clear_temp_files_flag = std::stoi(input);

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

distanceMeasure::DistanceMeasureCalculator* ForestPlug::CreateDistanceCalculator() const
{
    int calc_method;
    //distanceMeasure::RunFlags* flags = new distanceMeasure::RunFlags();
    //NOTE:: SHOULD SET FLAG PARAMETERS THROUGH USER INPUT...
        //quartets? calc type bitmask, clustering?
    //Sets the Quartet gen flag + clustering flag
        //NOTE:: moved before Calc_Method input --> allows for MrBayes input prompt, immediately after calc selection
    SetRunFlags(this->poRunFlags);

    distanceMeasure::DistanceMeasureCalculator* dmc = nullptr;
    printf("%s\nMatrix Calculation Method Number: ", distanceMeasure::CalculatorFactory::Dump().c_str());
    std::cin >> calc_method;

    int batch_calculator_index;
    distanceMeasure::CalculatorFactory::GetBatchCalculatorIndex(batch_calculator_index);
    //check if bitmask should be set
    if (calc_method == batch_calculator_index)
    {
        //note:: could move to BatchCalculators constructor (like MrBayes) --> forces delayed init of member vars
        InitializeBatchCalculatorFlags(this->poRunFlags, batch_calculator_index);
    }


    //NOTE:: CANNOT CREATE CALCULATOR UNTIL ALL --RUN FLAGS-- received --> NEEDED FOR CONSTRUCTION SET***
    dmc = distanceMeasure::CalculatorFactory::Create(calc_method, this->poRunFlags);
    printf("=============================================================================\n");

    return dmc;
}

void ForestPlug::InitializeBatchCalculatorFlags(distanceMeasure::RunFlags* flags, const int batch_calculator_index) const
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

    std::cin >> batch_methods;
    for (auto it = batch_methods.begin(); it != batch_methods.end(); it++)
    {
        //for each char set corresponding bit position
            //convert calc number/name ==> calc bitmask (find calc -> get bitmask)
        const int index = *it - ASCII_INTEGER_DIFFERENCE;
        if (index == batch_calculator_index)
        {
            //BITMASK FOR ALL CALCULATORS...
            bitmask |= SystemParameters::GetAllCalculatorsMask();
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

    if (bit_count < 1)
    {
        //batch calc needs 2+ calcs for analysis
        printf("ERROR --> Switching to defaults\n");
        //===> TODO:: Give option to turn off analysis
        exit(0);
    }
    //get number of methods included in bitmask
    flags->calculator_count = bit_count;
}

void ForestPlug::SetRunFlags(distanceMeasure::RunFlags* flags)
{

    int quartets_gen_flag = 1;
    printf("Would you like to generate All quartet trees on the Sequence Sets?\n");
    printf("No (0), Yes (1)\n");
    std::cin >> quartets_gen_flag;

    bool generate_quartets = true;
    if (quartets_gen_flag < 1)
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
    switch (clustering_flag)
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
}

//TODO:: move this somewhere???
#include "SequenceListsGenerator.h"

void ForestPlug::SetSequenceListsFile(int batch_flag, const distanceMeasure::DistanceMatrixObject& dmo, std::string& sequence_lists_file)
{

    switch (batch_flag)
    {
    case 0:
    	//initialize batch sequence list params
        SystemParameters::InitializeSequenceSetParameters(dmo.getFileObjectManager().get_file_count());
    	
        //generate BATCH SequenceLists file
        sequence_lists_file = distanceMeasure::SequenceListsGenerator::GenerateBatchSequenceListsFile(dmo.getFileObjectManager().GetCurrentSetNames());
        break;
    case 1:
    	//do nothing -- sequence list already given...
        break;
    case 2:
        //generate single tree gen SequenceLists file
        sequence_lists_file = distanceMeasure::SequenceListsGenerator::GenerateSequenceListFile(dmo.getFileObjectManager().GetCurrentSetNames());
        break;
    default:
        //generate single tree gen SequenceLists file
        sequence_lists_file = distanceMeasure::SequenceListsGenerator::GenerateSequenceListFile(dmo.getFileObjectManager().GetCurrentSetNames());
    	break;
	}

}


//TODO:: MOVE THIS SOMEWHERE...
#include "SequenceNamesStrategy.h"
#include "SequenceNamesUnorderedFastaStrategy.h"
#include "SequenceNamesOrderedCustomStrategy.h"
#include "SequenceNamesDescriptionStrategy.h"
#include "SequenceNamesIDStrategy.h"
distanceMeasure::SequenceNamesStrategy* ForestPlug::GetNamingStrategy()
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

std::string ForestPlug::GetOriginalFastaInputPath()
{
    std::string sequence_dir;
    //receive relative/absolute path of file or directory of sequences to analyze
    //TODO:: REASLISTICALLy --  Use absolute path of directory (allow for changing of default directory?
    //
    //TODO:: ALLOW multiple fasta sequence paths to be given -- concatenate those together for 1
        //TODO::Should change to implicitly concatenate FASTA files -- if more than one path given
    std::string input;
    //printf("Is your FASTA data in 1 file? No (0), Yes (1)\n");

    printf("FASTA input types:\n\t 1 FASTA input (0)\n\t Multiple FASTA input files (1)\n\t Multiple Chromosomal FASTA input files (2)\n");
    //std::cin >> fasta_input_flag;
    std::getline(std::cin, input, '\n');
    const int fasta_input_flag = std::stoi(input);
	
	switch(fasta_input_flag)
	{
    case 0:
        sequence_dir = GetSingleFastaFilePath();
        break;
    case 1:
        sequence_dir = CombineMultipleFastaFileInputs();
        break;
    case 2:
        sequence_dir = CombineMultipleChromosomalFastaInputs();
        break;
    default:
        //throw std::exception("Invalid FASTA input type");
        printf("Invalid FASTA input type\n");
	    exit(0);
    }
	
    return sequence_dir;
}

std::string ForestPlug::GetSingleFastaFilePath()
{
    printf("Path to FASTA file: ");
    std::string path;
    std::cin >> path;
	
	return path;
}

std::string ForestPlug::CombineMultipleFastaFileInputs()
{
    printf("Please enter, separated by spaces, all your FASTA file paths:\n");
	std::string fasta_paths, sequence_dir;
	//ask user for file name
	//std::cin >> fasta_paths;//delimites input at " " space -- stores chars after space for next "std::cin" call...
    std::getline(std::cin, fasta_paths, '\n');
	
	//concatenate all inputs together --> creating 1 FASTA file --> pass to dmo
    //should allow all input paths on 1 line -> parse by looking for space
    //int more_input_flag = 1;
    //std::string fasta_paths;
    //while (more_input_flag)
    //{
    //    std::string path;
    //    //ask user for file name
    //    printf("Path to Sequences: ");
    //    std::cin >> path;
    //    fasta_paths.append(path + " ");

    //    printf("Do you have more FASTA file inputs? No (0), Yes (1)\n");
    //    std::cin >> more_input_flag;
    //}//NOTE:: could/should be done by user in 1 line

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
                if (intermediary_file.is_open())
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

	return original_fasta_path;
}

std::string ForestPlug::CombineMultipleChromosomalFastaInputs()
{
    printf("Please enter, separated by spaces, all your FASTA file paths:\n");
    std::string fasta_paths, sequence_dir;
    //ask user for file name
    //std::cin >> fasta_paths;
    std::getline(std::cin, fasta_paths, '\n');
	
	//iterate over all given paths
    std::vector<std::string> inputs;
    ForestPlug::split(fasta_paths, ' ', inputs);

	//create original fasta file for appending each input file to
    std::string original_fasta_path(SystemParameters::GetTempFilesDirectory());
    original_fasta_path.append("/original.fasta");
    //open file to append
    std::ofstream original_fasta_file(original_fasta_path, std::ios_base::binary);


	//for each input file -- 
    for (std::string path : inputs)
    {
    	//if invalid path
    	if(path.size() < 2)
    	{
            continue;
    	}
    	
	    std::ifstream input_file(path);
		//for each file -- remove all fasta descriptors and combine sequences into one large genome per file
	    if (input_file.is_open())
	    {
            std::string fasta_descriptor = ">"+ path + "\n";
	    	//append name of file sequences to original file as fasta descriptor
            original_fasta_file << fasta_descriptor;
	    	
	    	std::string line;
	        while (std::getline(input_file, line))
	        {
	        	//if not a sequence descriptor append to original FASTA file
	        	if(line.at(0) != '>')
	        	{
	        		original_fasta_file << line;
	        	}
            }
            original_fasta_file << "\n";
            input_file.close();
	    }
	    else printf("The file: %s could not be opened", path.c_str());
		
    }
    original_fasta_file.close();
	
    return original_fasta_path;
}

void ForestPlug::split(const std::string& s, char delim, std::vector<std::string>& result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        result.push_back(item);
    }
}
