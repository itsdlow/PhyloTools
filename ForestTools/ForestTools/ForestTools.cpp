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

#include "SystemParameters.h"
#include "SequenceListsGenerator.h"

int main()
{
    std::cout << "Hello World!\n";
    /*****   (using sequence data) create matrix --> tree (fastme)   ****/

    //distanceMeasure::SequenceProcessor sequenceProcessorType = distanceMeasure::SequenceProcessor::FileProcessing;
    int method = 0;
    //int sequenceCount;
    std::string sequence_names_dir;
    std::string sequence_dir;
    std::string tree_sequences_list;

	//receive relative/absolute path of file or directory of sequences to analyze
        //TODO:: REASLISTICALLy --  Use absolute path of directory (allow for changing of default directory?
    printf("Path to Sequences: ");
    std::cin >> sequence_dir;


    printf("Path to sequence names file: ");
    std::cin >> sequence_names_dir;
        //if no sequence names provided...
            //default processing?
    //recieve Number of sequences to read
    //printf("Number of Sequences: ");
    //std::cin >> sequenceCount;
    
    //recieve matrix_calculation method (LCS, P-Value, MrBayes, NCD)
    printf("All Methods (0), LCS (1), P_Value (2), MrBayes (3), NCD (4)\n");
    printf("Matrix Calculation Method Number: ");
	std::cin >> method;
    printf("\n");

	distanceMeasure::DistanceMeasureCalculator* dmc = nullptr;
	switch (method)
	{
    case 0:
		//get calculator count...
			//use bitmasking?
			
        dmc = new distanceMeasure::BatchDistanceCalculators(SystemParameters::GetCalculatorsCount());
		break;
	case 1:
		dmc = new distanceMeasure::LcsDistanceCalculator();
		break;
	case 2:
		dmc = new distanceMeasure::PValueDistanceCalculator();
		break;
	case 3:
		dmc = new distanceMeasure::MrBayesDistanceCalculator();
		break;
	case 4:
		//TODO:: NEEDS TO BE GIVEN compression flags...
			//default runs all compressions
        int compressor_flag;
        printf("Compressor Flag Method: 7Zip (1), MFC1 (2), MFC2 (3), MFC3 (4), Winzip (5)\n");
		//get compressor to use for calculation
        std::cin >> compressor_flag;
		dmc = new distanceMeasure::NcdDistanceCalculator(compressor_flag);
		break;
  // case 5:
		////BATCH NCD (multiple NCD calculations)
		//---> adjust BatchCalculators to be able to run just_all types of NCD
	default:
		break;
	}
    //intializes / prepares all file objects (sequences) -- given a DistanceMeasureCalculator.
    distanceMeasure::DistanceMatrixObject dmo(sequence_names_dir, sequence_dir, dmc);
    
    /*******************   Batch run (OPTIONAL)   ***********************/
    //CREATE SEQUENCE LISTS FILE
    //get sequence count
    SystemParameters::Initialize(dmo.getFileObjectManager().get_file_count());
    int batch_flag;
	printf("Batch Run (0), Give SequenceLists File (1), Single Tree on a all sequences (2)\n");
    //file:: string of all sequence combinations --> matrixes to create
    std::cin >> batch_flag; 
    if(batch_flag == 0)
    {
	    //generate BATCH SequenceLists file
        tree_sequences_list = distanceMeasure::SequenceListsGenerator::GenerateBatchSequenceListsFile(dmo.getFileObjectManager().GetCurrentSetNames());
    }
    else if (batch_flag == 1)
    {
        printf("Enter path to Sequence List file:");
        //file:: string of all sequence combinations --> matrixes to create
        std::cin >> tree_sequences_list;
    }
    else
    {
	    //generate single tree gen SequenceLists file
        tree_sequences_list = distanceMeasure::SequenceListsGenerator::GenerateSequenceListFile(dmo.getFileObjectManager().GetCurrentSetNames());
    }

  /*********************************************************************************************
        for each entry in tree_sequence_list -- calculateDistanceMeasures + AllQuartetsMatrix + TREES...
  *********************************************************************************************/
    dmo.batch_matrix_calculation(tree_sequences_list);

	    /******   (batch) ANALYZE CREATED TREES!!!!     *******/
	    //-----> only analyzed for BatchDistanceCalculator
}
		
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