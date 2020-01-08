/******************************************************************************
DeAngelo Wilson
January 3 2020

                        ForestTools (main)
******************************************************************************/


#include <iostream>
#include <string>

#include "DistanceMatrixObject.h"
#include "PValueDistanceCalculator.h"

int main()
{
    std::cout << "Hello World!\n";
    /*****   (using sequence data) create matrix --> tree (fastme)   ****/
    //Command line arguments:: 
    //1. directory path for Sequnces folder
    //2. number of sequence files

    //distanceMeasure::SequenceProcessor sequenceProcessorType = distanceMeasure::SequenceProcessor::FileProcessing;
    int method = 0;
    int sequenceCount;
    std::string sequence_names_dir;
    std::string sequence_dir;
    std::string tree_sequences_list;



    //recieve path of file of -- ALL sequence names -- same (EXACT) names that appear in fasta_sequence files**********
        /*NOTE:: MRBayes --> uses .nex format
              "Single_Word_Sequence_Name      (whitespace)        ATTAGCCGATGCAGTC...(SEQUENCE)"
        */
    printf("Path to sequence names file: ");
    std::cin >> sequence_names_dir;
        //if no sequence names provided...
            //default processing?

    //receive relative/absolute path of file or directory of sequences to analyze
        //TODO:: REASLISTICALLy --  Use absolute path of directory
    printf("Path to Sequences: ");
    std::cin >> sequence_dir;


    //recieve Number of sequences to read
    printf("Number of Sequences: ");
    std::cin >> sequenceCount;
    //recieve matrix_calculation method (LCS, P-Value, MrBayes, NCD)
    printf("LCS (0), P_Value (1), MrBayes (2), NCD (3)\n");
    printf("Matrix Calculation Method Number: ");
    std::cin >> method;
        //enum class Matrix_Calculator_Type{}
    //SequenceProcessorType --> determined by method 
        //FILEPROCESSOR (aligned):: MRBayes, P_Value
        //DIRECTORYPROCESSOR (non-aligned):: NCD, LCS
            //OR (request) Number of files > 1 --> DIRECTORY PROCESSOR?

  
    //intializes / prepares all file objects (sequences)
        //for generic distance measure method to work --> must supply distance calc (w/ normalize func!!!)
    distanceMeasure::DistanceMatrixObject dmo(sequence_names_dir, sequence_dir, distanceMeasure::SequenceProcessorType::FileProcessor, sequenceCount, new distanceMeasure::PValueDistanceCalculator());
    
    /******************************************/
    printf("Path to Sequence List File: ");
    //file:: string of all sequence combinations --> matrixes to create
    std::cin >> tree_sequences_list;


          /*********************************************************************************************
                for each entry in tree_sequence_list -- calculateDistanceMeasures + AllQuartetsMatrix
          *********************************************************************************************/
    //Output.txt (LargeTree-Matrix)
    dmo.calculateDistanceMeasures();

    //FOR TESTING PURPOSES ONLY -- sets a static distance Measure array for caulcating quartets
    //dmo.setCalculateDistanceMeasureTEST(getStaticDistanceMeasureArray());

    //quartets.txt (QuartetsTree-Matrix)
    dmo.calculateAllQuartetsDistanceMatrix();
    //results written to "output.txt" + "quartets.txt" upon DistanceMAtrix destructions (writeResults() called)

    //TODO
        // 0) Refactor LcsCalculator code*
        // 1) Naming convention????
        //      1.5) Processing of sequence list??? --> matrix -> tree (fastme) 
        // 2) Fastme itegration ( CreateProcessA || system() )
        // 3) How to get amino acid sequences
        // 4) How to align sequences using... Muscle?
        //          - Checking (when required) for aligned sequences
        // 5) GUARDS!!!
                //safe-failing/execption safe
        // 6) FileObject::sequence_size --> long -- allow for 3billion bp sequence






    /******    ANALYZE CREATED TREES!!!!     *******/

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
}