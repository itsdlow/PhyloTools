/******************************************************************************
DeAngelo Wilson
January 3 2020

              PhyloAnalysis (namespace for phylogenetics analysis tools)
******************************************************************************/

#include "PhyloAnalysis.h"
#include "PhyloTools.h"
#include "Tree.h"

namespace PhyloAnalysis
{

    using namespace phylo;
    //prints to output the agreement between two given files (of quartetTrees)
    void computeQuartetAgreement(std::string quartetFilename1, std::string quartetFilename2)
    {
        PhyloTools ptools = PhyloTools();

        printf("Creating quartet tree indicies for each file\n");
        PhyloTools::treeIndex quartetPairingIndices1 = ptools.buildQuartetsPairingIndex(quartetFilename1);
        printf("-------------------------------------------\n\n\n\n\n\n\n-------------------------------------------\n");
        PhyloTools::treeIndex quartetPairingIndices2 = ptools.buildQuartetsPairingIndex(quartetFilename2);
        printf("Quartets Pairing done::\n");


        int total = 0;
        int matches = 0;
        //loop through all pairings of quartets
        for (PhyloTools::treeIndex::iterator it = quartetPairingIndices1.begin(); it != quartetPairingIndices1.end(); it++)
        {

            PhyloTools::quartetPair pairing1 = quartetPairingIndices1[it->first];
            PhyloTools::quartetPair pairing2 = quartetPairingIndices2[it->first];
            total++;
            /*
            //CHECK IF PAIRING EQUALITY TEEST IS ACCURATE!!!
            printf("Pairing1 :: pair1: %s, %s -- pair2 :: %s, %s\n",pairing1.pair1.first.c_str(), pairing1.pair1.second.c_str(), pairing1.pair2.first.c_str(), pairing1.pair2.second.c_str());
            printf("Pairing2 :: pair1: %s, %s -- pair2 :: %s, %s\n\n",pairing2.pair1.first.c_str(), pairing2.pair1.second.c_str(), pairing2.pair2.first.c_str(), pairing2.pair2.second.c_str());
            */
            if (pairing1 == pairing2)
            {
                matches++;
            }

        }

        printf("Total number of quartet trees: %6d\nNumber of identical quartet trees: %6d\nAgreement ratio: %6.4f\n", total, matches, 1.0 * matches / total);

    }

    /*

    # Given 2 largeTreeFiles, compute the agreement between quartets
    # directly induced from largeTree



    */

    void computeInducedQuartetAgreement(std::string largeTreeFilename1, std::string largeTreeFilename2)
    {

        //compied from agreement
        PhyloTools ptools = PhyloTools();

        //large tree processing done in PhyloTools::getLargeTreeLine(std::string)
        Tree largeListTree1 = ptools.parseNewickExpression(ptools.getLargeTreeLine(largeTreeFilename1));
        printf("largeTreeNewick1: %s\n", largeListTree1.to_newick().c_str());

        Tree largeListTree2 = ptools.parseNewickExpression(ptools.getLargeTreeLine(largeTreeFilename2));
        printf("largeTreeNewick2: %s\n", largeListTree2.to_newick().c_str());

        //lists of all possible combinations of 4 of leafs
        std::vector<std::vector<std::string> > largeQuartetSequenceList;
        int largeListSequenceCount = ptools.comboFourOf(largeListTree1, largeQuartetSequenceList);
        //assumes both trees are on same sequence set
        //std::vector<const std::vector<std::string> > largeQuartetSequenceList2(largeQuartetSequenceList1);

        int total = 0;
        int matches = 0;
        for (std::vector<std::vector<std::string> >::const_iterator it = largeQuartetSequenceList.begin(); it != largeQuartetSequenceList.end(); it++)
        {
            //printf("quartet: %s, %s, %s, %s\n",(*it).at(0).c_str(),(*it).at(1).c_str(),(*it).at(2).c_str(),(*it).at(3).c_str());

            Tree subtree1 = ptools.createHomeoSubtree(largeListTree1, *it);
            PhyloTools::quartetPair largeQuartetPairing1 = ptools.quartetPairDivision(subtree1);
            Tree subtree2 = ptools.createHomeoSubtree(largeListTree2, *it);
            PhyloTools::quartetPair largeQuartetPairing2 = ptools.quartetPairDivision(subtree2);
            total++;
            if (largeQuartetPairing1 == largeQuartetPairing2)
            {
                matches++;
            }


        }

        printf("Total number of quartet trees: %6d\nNumber of identical quartet trees: %6d\nAgreement ratio: %6.4f\n", total, matches, 1.0 * matches / total);


    }

    /*
     #
     # Given a tree on more than four sequences, its quartet
     # compatibility is a count of how many homeomorphic quartet
     # trees induced by that larger tree match quartet trees
     # directly generated on those same four sequences.
     #

     # The first command line argument should be the name of the file containing
     # all of the quartet trees.  The second argument is the name of the file containing
     # tree on a large list of sequences.
     */
     //prints to output the compatability ratio between a large sequence tree file and derived quartet trees on sequences
    void computeQuartetCompatability(std::string quartetFilename, std::string largeTreeFilename)
    {
        PhyloTools ptools = PhyloTools();

        PhyloTools::treeIndex quartetTreeIndices = ptools.buildQuartetsPairingIndex(quartetFilename);
        printf("Quartet Pairing done::\n");

        //large tree processing done in PhyloTools::getLargeTreeLine(std::string)
        Tree largeListTree = ptools.parseNewickExpression(ptools.getLargeTreeLine(largeTreeFilename));
        printf("largeTreeNewick: %s\n", largeListTree.to_newick().c_str());

        //lists of all possible combinations of 4 of leafs
        std::vector<std::vector<std::string> > largeQuartetSequenceList;
        int largeListSequenceCount = ptools.comboFourOf(largeListTree, largeQuartetSequenceList);

        int total = 0;
        int matches = 0;
        for (std::vector<std::vector<std::string> >::const_iterator it = largeQuartetSequenceList.begin(); it != largeQuartetSequenceList.end(); it++)
        {
            //printf("quartet: %s, %s, %s, %s\n",(*it).at(0).c_str(),(*it).at(1).c_str(),(*it).at(2).c_str(),(*it).at(3).c_str());

            Tree subtree = ptools.createHomeoSubtree(largeListTree, *it);
            PhyloTools::quartetPair largeQuartetPairing = ptools.quartetPairDivision(subtree);
            total++;
            if (largeQuartetPairing == quartetTreeIndices[*it])
            {
                matches++;
            }


        }
        printf("Number of sequences in large tree: %2d\nTotal number of quartet trees: %6d\nNumber of identical quartet trees: %6d\nCompatibility ratio: %6.4f\n", largeListSequenceCount, total, matches, 1.0 * matches / total);
    }
    //uses bipartition method to compute symmetric difference --
    //both arguments are largeTreeFileDirectories
    void computeSymmetricDifference(std::string largeTreeFilename1, std::string largeTreeFilename2)
    {
        PhyloTools ptools = PhyloTools();

        std::string largeTreeNewick1 = ptools.getLargeTreeLine(largeTreeFilename1);
        std::string largeTreeNewick2 = ptools.getLargeTreeLine(largeTreeFilename2);
        const PhyloTools::BipartitionList bipartitionList1 = ptools.listBipartitions(ptools.parseNewickExpression(largeTreeNewick1));
        const PhyloTools::BipartitionList bipartitionList2 = ptools.listBipartitions(ptools.parseNewickExpression(largeTreeNewick2));

        int differenceCount = ptools.symmetricDifference(bipartitionList1, bipartitionList2);

        printf("The Difference Count is: %d\n", differenceCount);

    }
}