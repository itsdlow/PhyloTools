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
#include "BatchDistanceCalculators.h"

int main()
{
    std::cout << "Hello World!\n";
    /*****   (using sequence data) create matrix --> tree (fastme)   ****/
    //Command line arguments:: 
    //1. directory path for Sequnces folder
    //2. number of sequence files

    //distanceMeasure::SequenceProcessor sequenceProcessorType = distanceMeasure::SequenceProcessor::FileProcessing;
    int method = 0;
    //int sequenceCount;
    std::string sequence_names_dir;
    std::string sequence_dir;
    std::string tree_sequences_list;



    //recieve path of file of -- ALL sequence names -- same (EXACT) names that appear in fasta_sequence files**********
        /*NOTE:: MRBayes --> uses .nex format
              "Single_Word_Sequence_Name      (whitespace)        ATTAGCCGATGCAGTC...(SEQUENCE)"
        */

	//receive relative/absolute path of file or directory of sequences to analyze
        //TODO:: REASLISTICALLy --  Use absolute path of directory
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
			
        dmc = new distanceMeasure::BatchDistanceCalculators(2);
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
	//case 4:
	//	dmc = new distanceMeasure::NcdDistanceCalculator();
	//	break;
	default:
		break;
	}
    //intializes / prepares all file objects (sequences)
        //for generic distance measure method to work --> must supply distance calc (w/ normalize func!!!)
    distanceMeasure::DistanceMatrixObject dmo(sequence_names_dir, sequence_dir, dmc);
    
    /*******************   TODO:: Batch run (OPTIONAL)   ***********************/
    printf("Path to Sequence List File: ");
    //file:: string of all sequence combinations --> matrixes to create
    std::cin >> tree_sequences_list;



  /*********************************************************************************************
        for each entry in tree_sequence_list -- calculateDistanceMeasures + AllQuartetsMatrix
  *********************************************************************************************/
    dmo.batch_matrix_calculation(tree_sequences_list);

	//NOT IMPLEMENTED
    /******   (batch) ANALYZE CREATED TREES!!!!     *******/

    //std::string fileDir1 = std::string(argv[1]);
    //std::string fileDir2 = std::string(argv[2]);
    //printf("tree file directories are... \n\t%s + %s\n", fileDir1.c_str(), fileDir2.c_str());
    /*
    phylo::PhyloAnalysis pa = phylo::PhyloAnalysis();
    pa.computeQuartetAgreement(fileDir1, fileDir2);
    */

    //phylo::computeQuartetCompatability(fileDir1, fileDir2);
    //phylo::computeQuartetAgreement(fileDir1, fileDir2);
    //phylo::computeSymmetricDifference(fileDir1, fileDir2);
    //phylo::computeInducedQuartetAgreement(fileDir1, fileDir2);

	
    //TODO::
        // 0) Refactor LcsCalculator code*
        // 1) Naming convention????
        //      1.5) Processing of sequence list??? --> matrix -> tree (fastme) 
        // 2) Fastme itegration ( CreateProcessA || system() )
        // 3) How to get amino acid sequences
        // 4) How to align sequences using... Muscle?
        //          - Checking (when required) for aligned sequences
        // 5) GUARDS!!!
                //safe-failing/execption safe
                //Matrixes not created...
					//Bad file names?
					//failed alignment?
				//Trees not created...
					//failed fastme?
        // 6) FileObject::sequence_size --> long -- allow for 3billion bp sequences

}
