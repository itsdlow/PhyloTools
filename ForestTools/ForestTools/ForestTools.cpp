/******************************************************************************
DeAngelo Wilson
January 3 2020

                        ForestTools (main)
******************************************************************************/


#include <iostream>
#include <string>

#include "DistanceMatrixObject.h"

#include "DistanceMeasureCalculator.h"
#include "PValueDistanceCalculator.h"
#include "LcsDistanceCalculator.h"
#include "MrBayesDistanceCalculator.h"
#include "NcdDistanceCalculator.h"
#include "BatchDistanceCalculators.h"

#include "SequenceListsGenerator.h"

#include "SequenceNamesStrategy.h"
#include "SequenceNamesUnorderedFastaStrategy.h"
#include "SequenceNamesOrderedCustomStrategy.h"
#include "SequenceNamesDescriptionStrategy.h"
#include "SequenceNamesIDStrategy.h"

#include "SystemParameters.h"



//forward decl
void TryClearingTempFiles();
distanceMeasure::DistanceMeasureCalculator* GetDistanceCalculator();
std::string SetSequenceListsFile(int batch_flag, const distanceMeasure::DistanceMatrixObject& dmo);
distanceMeasure::SequenceNamesStrategy* GetNamingStrategy();

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

	
    //distanceMeasure::SequenceProcessor sequenceProcessorType = distanceMeasure::SequenceProcessor::FileProcessing;
    std::string sequence_dir;

	//receive relative/absolute path of file or directory of sequences to analyze
        //TODO:: REASLISTICALLy --  Use absolute path of directory (allow for changing of default directory?
        //
    //TODO:: ALLOW multiple fasta sequence paths to be given -- concatenate those together for 1
    printf("Path to Sequences: ");
    std::cin >> sequence_dir;


	//set sequenceNames strategy --> pass to dmo -> fom (instead of sequence_names path)
    distanceMeasure::SequenceNamesStrategy* name_strategy = GetNamingStrategy();

	distanceMeasure::DistanceMeasureCalculator* dmc = GetDistanceCalculator();

	//get sequenceList type
    int batch_flag;
    printf("Batch Run (0), Give SequenceLists File (1), Single Tree on a all sequences (2)\n");
    printf("Sequence Lists Method Number: ");
    //file:: string of all sequence set combinations --> matrices to create
    std::cin >> batch_flag;

	//GET ALL USER INPUT BEFORE CONSTRUCTING dmo---------------------------------------------------------------------------
    //intializes / prepares all file objects (sequences) -- given a DistanceMeasureCalculator.
    distanceMeasure::DistanceMatrixObject dmo(name_strategy, sequence_dir, dmc);

	
    //CREATE SEQUENCE LISTS FILE
    SystemParameters::Initialize(dmo.getFileObjectManager().get_file_count());
    const std::string tree_sequences_list = SetSequenceListsFile(batch_flag, dmo);

  /*********************************************************************************************
        for each entry in tree_sequence_list -- calculateDistanceMeasures + AllQuartetsMatrix + TREES...
  *********************************************************************************************/
    dmo.batch_matrix_calculation(tree_sequences_list);

	    /******   (batch) ANALYZE CREATED TREES!!!!     *******/
	    //-----> only analyzed for BatchDistanceCalculator
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

        sprintf_s(clean_mrbayes_dir_cmd, SystemParameters::GetCleanDirectoryCommandString().c_str(), SystemParameters::GetMrBayesFilesDirectory().c_str());
        sprintf_s(clean_temp_dir_cmd, SystemParameters::GetCleanDirectoryCommandString().c_str(), SystemParameters::GetTempFilesDirectory().c_str());

        //remove files from MrBayes + TempFiles
        system(clean_mrbayes_dir_cmd);
        system(clean_temp_dir_cmd);

    }
}

distanceMeasure::DistanceMeasureCalculator* GetDistanceCalculator()
{
    int calc_method;

    //recieve matrix_calculation method (LCS, P-Value, MrBayes, NCD)
    printf("All Methods (0), LCS (1), P_Value (2), MrBayes (3), NCD (4)\n");
    printf("Matrix Calculation Method Number: ");
    std::cin >> calc_method;
    int quartets_gen_flag;
    printf("Would you like to generate All quartet trees on the Sequence Sets?\n");
    printf("No (0), Yes (1)\n");
    std::cin >> quartets_gen_flag;
	
    distanceMeasure::DistanceMeasureCalculator* dmc = nullptr;

    bool generate_quartets = true;
	if(quartets_gen_flag < 1)
	{
        generate_quartets = false;
	}
    distanceMeasure::RunFlags* flags = new distanceMeasure::RunFlags(generate_quartets);

	
    switch (calc_method)
    {
    case 0:
        //get calculator count...
            //use bitmasking?
         //TODO:: flags->calculators_bitmask = mask;
        flags->calculator_count = SystemParameters::GetCalculatorsCount();
        dmc = new distanceMeasure::BatchDistanceCalculators(flags);
        break;
    case 1:
        dmc = new distanceMeasure::LcsDistanceCalculator(flags);
        break;
    case 2:
        dmc = new distanceMeasure::PValueDistanceCalculator(flags);
        break;
    case 3:
        dmc = new distanceMeasure::MrBayesDistanceCalculator(flags);
        break;
    case 4:
        int compressor_flag;
        printf("Compressor Flag Method: 7Zip (1), MFC1 (2), MFC2 (3), MFC3 (4), Winzip (5)\n");
        //get compressor to use for calculation
        std::cin >> compressor_flag;
        dmc = new distanceMeasure::NcdDistanceCalculator(flags, compressor_flag);
        break;
    // case 5:
          ////BATCH NCD (multiple NCD calculations)
          //---> adjust BatchCalculators to be able to run just_all types of NCD
    default:
        break;
    }
	
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
    else if (batch_flag == 1)
    {
        printf("Enter path to Sequence List file:");
        //file:: string of all sequence set combinations --> matrices to create
        std::cin >> tree_sequences_list;
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

