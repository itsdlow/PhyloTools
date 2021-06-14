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
#include <cassert>

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
	
    distanceMeasure::DistanceMeasureCalculator* dmc = CreateDistanceCalculator();//can convert to static...

	//Construct DMO and FileObjectManager -- (NOTE:: file-objects and manager initialized in DistanceMatrixObject::run)
    distanceMeasure::DistanceMatrixObject dmo(dmc);


    std::vector<InputSequenceFileSet> batch_files;
    int batch_files_flag = 1;
    while(batch_files_flag)
    {
        InputSequenceFileSet inputFileSet;
    	this->GetOriginalFastaInputPath(inputFileSet);//can convert to static...
	    ForestPlug::GetNamingStrategy(inputFileSet);

    	//NOTE:: mutually exclusive
    	ForestPlug::GetCompareTreePath(inputFileSet);
	    ForestPlug::GetSequenceListStrategy(inputFileSet);

    	
    	batch_files.push_back(inputFileSet);

        std::string input;
    	//continue (batch?)
        printf("\n");
        printf("Do you have more files for analysis? No (0), Yes (1)\n");
        std::getline(std::cin, input, '\n');
        batch_files_flag = std::stoi(input);
    }

    /*********************************************************************************************
          for each entry in tree_sequence_list -- calculateDistanceMeasures + AllQuartetsMatrix + TREES...
    *********************************************************************************************/
    dmo.run(batch_files);
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
        ForestPlug::ClearTempFiles();
    }
}

void ForestPlug::ClearTempFiles()
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

distanceMeasure::DistanceMeasureCalculator* ForestPlug::CreateDistanceCalculator() const
{
    std::string input;
    //Sets the Quartet gen flag + clustering flag
        //NOTE:: moved before Calc_Method input --> allows for MrBayes input prompt, immediately after calc selection
    SetRunFlags(this->poRunFlags);

    distanceMeasure::DistanceMeasureCalculator* dmc = nullptr;
    printf("%s\nMatrix Calculation Method Number: ", distanceMeasure::CalculatorFactory::Dump_NonBatch().c_str());
    //std::cin >> calc_method;
    std::getline(std::cin, input, '\n');
    const int calc_method = std::stoi(input);

    //TODO:: assert that calc_method is a "legal" NonBatch Method (NOT CompareTree)
	
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

    std::getline(std::cin, batch_methods, '\n');
    batch_methods = SystemParameters::Trim(batch_methods);
	
    //NOTE:: if user selects CompareTree --> must set RunFlags...
    int compare_calculator_index;
    distanceMeasure::CalculatorFactory::GetCompareTreeCalculatorIndex(compare_calculator_index);
	
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
        else if (index == compare_calculator_index)
        {
        	//NOTE:: there can only be 1 CompareTree Calculator (guaranteed by bitmask calc creation)
        	
            //bit mask for given-current (*it) calculator
            bitmask |= SystemParameters::GetCalculatorMask(index);
        	
            //NOTE:: mutually exclusive with SequenceList generation...
            flags->SetCompareTreeCalculator();
        }
    	//TODO:: else if(index == pvalue_calculator_index) ==> ask user if they need to align sequences
    	
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
    std::string input;
    int quartets_gen_flag = 1;
    printf("Would you like to generate All quartet trees on the Sequence Sets?\n");
    printf("No (0), Yes (1)\n");
    //std::cin >> quartets_gen_flag;
    std::getline(std::cin, input, '\n');
    quartets_gen_flag = std::stoi(input);

    bool generate_quartets = true;
    if (quartets_gen_flag < 1)
    {
        generate_quartets = false;
    }//ew
    //set calc run flags blackboard
    flags->generate_quartets = quartets_gen_flag;


    //CLUSTERING???
    printf("Would you like to an additional tree with clustered like sequences?\n");
    //NOTE:: CANNOT PERFORM ANALYSIS ON TREEs that have been clustered -- clusters can vary from method to method...
    printf("None (0), Strict/Less clusters (1), Loose/More clusters (2)\n");
    //std::cin >> clustering_flag;
    std::getline(std::cin, input, '\n');
    const int clustering_flag = std::stoi(input);

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


void ForestPlug::GetCompareTreePath(InputSequenceFileSet& inputFileSet)
{
    //todo:: convert to state pattern...
	if(this->poRunFlags->compare_tree_calculator)
	{
		printf("Path to CompareTree Newick file: ");
		std::string path;
		std::getline(std::cin, path, '\n');

		inputFileSet.compareTreePath = SystemParameters::Trim(path);
	}

}

void ForestPlug::GetSequenceListStrategy(InputSequenceFileSet& inputFileSet)
{
    int batch_flag = 2;
	std::string tree_sequences_list;

	if(this->poRunFlags->compare_tree_calculator == false)
	{
		//get sequenceList type
	    std::string input;
	    printf("Batch Run (0), Give SequenceLists File (1), Single Tree on a all sequences (2)\n");
	    printf("Sequence Lists Method Number: ");
	    std::getline(std::cin, input, '\n');
	    batch_flag = std::stoi(input);
		

	    if (batch_flag == 1)
	    {

	        printf("Enter path to Sequence List file:");
	        //file:: string of all sequence set combinations --> matrices to create
	        std::getline(std::cin, tree_sequences_list, '\n');
	    }

	}
	
    inputFileSet.sequenceListBatchFlag = batch_flag;
	inputFileSet.sequenceListPath = tree_sequences_list;

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
#include "SequenceNamesNexusStrategy.h"
void ForestPlug::GetNamingStrategy(InputSequenceFileSet& inputFileSet)
{
    //TODO:: allow multiple ways
    /*
     * (1) -- Default -- use whole taxa name line
     * (2) Use accenssion number
     * (3) supply SequenceNames file
     * (4) supply ordered alternative names
     * (5) use supplied TaxaNames in Nexus file
     */

	//NOTE:: nexus format has single Naming strategy...
    if(inputFileSet.formatting == distanceMeasure::InputSequencesFormatType::NEXUS)
    {
        inputFileSet.namesStrategy = new distanceMeasure::SequenceNamesNexusStrategy(inputFileSet.path);
    }
    else
    {

       
        std::string input;
        printf("Description Line (0), Accession ID (1), Input FASTA Sequence Names file (2), Input Ordered Alternative Names (3)\n");
        printf("Sequence Names Type Number: ");
        //std::cin >> names_type;
        std::getline(std::cin, input, '\n');
        const int names_type = std::stoi(input);

        std::string path;
        int count;
    	
        distanceMeasure::SequenceNamesStrategy* strategy = nullptr;
        switch (names_type)
        {
            //default
        case 0:
            strategy = new distanceMeasure::SequenceNamesDescriptionStrategy();

            printf("Total number of sequences: ");
            //std::cin >> count;
            std::getline(std::cin, input, '\n');
            count = std::stoi(input);
        	strategy->SetSequenceCount(count);
            break;
        case 1:
            strategy = new distanceMeasure::SequenceNamesIDStrategy();

            printf("Total number of sequences: ");
            std::getline(std::cin, input, '\n');
            count = std::stoi(input);
        	strategy->SetSequenceCount(count);
            break;
        case 2:
            strategy = new distanceMeasure::SequenceNamesUnorderedFastaStrategy();

            printf("Path to sequence names file: ");
            std::getline(std::cin, path, '\n');
            strategy->SetSequenceNamesPath(path);
            break;
        case 3:
            strategy = new distanceMeasure::SequenceNamesOrderedCustomStrategy();

            printf("Path to sequence names file: ");
            std::getline(std::cin, path, '\n');
            strategy->SetSequenceNamesPath(path);
            break;
        default:
            break;
        }

        inputFileSet.namesStrategy = strategy;
    }
}

void ForestPlug::GetOriginalFastaInputPath(InputSequenceFileSet& inputFileSet)
{
    //receive relative/absolute path of file or directory of sequences to analyze
    //TODO:: REASLISTICALLy --  Use absolute path of directory (allow for changing of default directory?
    //
    //TODO:: ALLOW multiple fasta sequence paths to be given -- concatenate those together for 1
        //TODO::Should change to implicitly concatenate FASTA files -- if more than one path given
    std::string input;
    //printf("Is your FASTA data in 1 file? No (0), Yes (1)\n");

    printf("Input Sequences types:\n\t Single Input Sequence File (0)\n\t Multiple FASTA input files (1)\n\t Multiple Chromosomal FASTA input files (2)\n");
    //std::cin >> fasta_input_flag;
    std::getline(std::cin, input, '\n');
    const int fasta_input_flag = std::stoi(input);
	
	switch(fasta_input_flag)
	{
    case 0:
        this->GetSingleInputSequencesFilePath(inputFileSet);
        //pre process Nexus formatted file?
		
        break;
    case 1:
        this->CombineMultipleFastaFileInputs(inputFileSet);
        break;
    case 2:
        this->CombineMultipleChromosomalFastaInputs(inputFileSet);
        break;
    //case 3:
    //    sequence_dir = CombineMultipleGeneFastaInputs();
    //    break;
    default:
        //throw std::exception("Invalid FASTA input type");
        printf("Invalid FASTA input type\n");
		exit(0);
    }
}

void ForestPlug::GetSingleInputSequencesFilePath(InputSequenceFileSet& inputFileSet)
{
    printf("Input Sequences Format Types:\n\t FASTA (0)\n\t NEXUS (1)\n");
    std::string format;
    std::getline(std::cin, format, '\n');
    const int input_formatting_flag = std::stoi(format);

    distanceMeasure::InputSequencesFormatType formatting = distanceMeasure::InputSequencesFormatType::FASTA;
    switch(input_formatting_flag)
    {
    case 0:
        formatting = distanceMeasure::InputSequencesFormatType::FASTA;
        break;
    case 1:
        formatting = distanceMeasure::InputSequencesFormatType::NEXUS;
        break;
    default:
        printf("Invalid Sequence Format type\n");
        assert(false);
    }
	
    printf("Path to Input file: ");
    std::string path;
    std::getline(std::cin, path, '\n');

    inputFileSet.path = path;
    inputFileSet.formatting = formatting;
}

void ForestPlug::CombineMultipleFastaFileInputs(InputSequenceFileSet& inputFileSet)
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

    inputFileSet.path = original_fasta_path;
    inputFileSet.formatting = distanceMeasure::InputSequencesFormatType::FASTA;
}

void ForestPlug::CombineMultipleChromosomalFastaInputs(InputSequenceFileSet& inputFileSet)
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

    //TODO:: should be done "asynchronously" ==> do this after user gives all inputs...
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
	
    inputFileSet.path = original_fasta_path;
    inputFileSet.formatting = distanceMeasure::InputSequencesFormatType::FASTA;
}



//std::string ForestPlug::CombineMultipleGeneFastaInputs()
//{
//    printf("Please enter, separated by spaces, all your FASTA file paths:\n");
//    std::string fasta_paths, sequence_dir;
//    //ask user for file name
//    //std::cin >> fasta_paths;
//    std::getline(std::cin, fasta_paths, '\n');
//
//    //iterate over all given paths
//    std::vector<std::string> inputs;
//    ForestPlug::split(fasta_paths, ' ', inputs);
//
//    //create original fasta file for appending each input file to
//    std::string original_fasta_path(SystemParameters::GetTempFilesDirectory());
//    original_fasta_path.append("/original.fasta");
//    //open file to append
//    std::ofstream original_fasta_file(original_fasta_path, std::ios_base::binary);
//
//
//    //for each input file -- 
//    for (std::string path : inputs)
//    {
//        //if invalid path
//        if (path.size() < 2)
//        {
//            continue;
//        }
//
//        std::ifstream input_file(path);
//        //for each file -- remove all fasta descriptors and combine sequences into one large genome per file
//        if (input_file.is_open())
//        {
//            std::string fasta_descriptor = ">" + path + "\n";
//            //append name of file sequences to original file as fasta descriptor
//            original_fasta_file << fasta_descriptor;
//
//            std::string line;
//            while (std::getline(input_file, line))
//            {
//                //if not a sequence descriptor append to original FASTA file
//                if (line.at(0) != '>')
//                {
//                    original_fasta_file << line;
//                }
//            }
//            original_fasta_file << "\n";
//            input_file.close();
//        }
//        else printf("The file: %s could not be opened", path.c_str());
//
//    }
//    original_fasta_file.close();
//
//    return original_fasta_path;
//}

void ForestPlug::split(const std::string& s, char delim, std::vector<std::string>& result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        result.push_back(item);
    }
}
