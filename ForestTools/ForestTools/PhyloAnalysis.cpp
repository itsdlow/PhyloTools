/******************************************************************************
DeAngelo Wilson
January 3 2020

              PhyloAnalysis (namespace for phylogenetics analysis tools)
******************************************************************************/

#include "PhyloAnalysis.h"
#include "PhyloTools.h"
#include "Tree.h"

#include "SystemParameters.h" // included for AZUL_UNUSED_VAR() macro

namespace PhyloAnalysis
{
    //...lazy
    using namespace phylo;

	
    //prints to output the agreement between two given files (of quartetTrees)
    float computeQuartetAgreement(const std::string& quartetFilename1, const std::string& quartetFilename2)
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

        float res = static_cast<float>(matches) / static_cast<float>(total);
        return res;
    }

    /*

    # Given 2 largeTreeFiles, compute the agreement between quartets
    # directly induced from largeTree



    */

    float computeInducedQuartetAgreement(const std::string& largeTreeFilename1, const std::string& largeTreeFilename2)
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
        AZUL_UNUSED_VAR(largeListSequenceCount);
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

        float res = static_cast<float>(matches) / static_cast<float>(total);
        return res;
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
    float computeQuartetCompatibility(const std::string& largeTreeFilename, const std::string& quartetFilename)
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

        float res = static_cast<float>(matches) / static_cast<float>(total);
        return res;
    }
    //uses bipartition method to compute symmetric difference --
    //both arguments are largeTreeFileDirectories
    float computeSymmetricDifference(const std::string& largeTreeFilename1, const std::string& largeTreeFilename2)
    {
        PhyloTools ptools = PhyloTools();

        const std::string largeTreeNewick1 = ptools.getLargeTreeLine(largeTreeFilename1);
        const std::string largeTreeNewick2 = ptools.getLargeTreeLine(largeTreeFilename2);
        Tree tree_1 = ptools.parseNewickExpression(largeTreeNewick1);
        Tree tree_2 = ptools.parseNewickExpression(largeTreeNewick2);

        const PhyloTools::BipartitionList bipartitionList1 = ptools.listBipartitions(tree_1);
        const PhyloTools::BipartitionList bipartitionList2 = ptools.listBipartitions(tree_2);
        int differenceCount = ptools.bipartitionDistance(bipartitionList1, bipartitionList2);

        printf("The Difference Count is: %d\n", differenceCount);

        return static_cast<float>(differenceCount);
    }

//NEW ANALYSIS TOOLS

    float computeRFRate(const float differenceCount, const int sequenceCount)
    {
        return differenceCount / static_cast<float>(2 * sequenceCount - 6);
    }
	//gets RF rate between two tree files
    float computeCompatibilityMetric_LtoL(const std::string& largeTreeFilename1, const std::string& largeTreeFilename2)
    {
        PhyloTools ptools = PhyloTools();

        std::string largeTreeNewick1 = ptools.getLargeTreeLine(largeTreeFilename1);
        std::string largeTreeNewick2 = ptools.getLargeTreeLine(largeTreeFilename2);
        Tree tree_1 = ptools.parseNewickExpression(largeTreeNewick1);
        Tree tree_2 = ptools.parseNewickExpression(largeTreeNewick2);
        size_t sequenceCount = tree_1.leafNameList().size();

        const PhyloTools::BipartitionList bipartitionList1 = ptools.listBipartitions(tree_1);
        const PhyloTools::BipartitionList bipartitionList2 = ptools.listBipartitions(tree_2);

        const int differenceCount = ptools.bipartitionDistance(bipartitionList1, bipartitionList2);
        printf("The Difference Count is: %d\n", differenceCount);

        return computeRFRate(static_cast<float>(differenceCount), static_cast<int>(sequenceCount));
    }
	
	//computes a total (sum of) quartet RF distance -- between (IQ) large tree, induced quartets and (Q) direct quartets
		//averages the distance --> gets RF rate
			//(alternate:: sum of RF rates / total quartets)
	float computeCompatibilityMetric_IQtoQ(const std::string& largeTreeFilename, const std::string& quartetsFilename)
    {
        PhyloTools ptools = PhyloTools();

    	//get large tree
        std::string largeTreeNewick = ptools.getLargeTreeLine(largeTreeFilename);
        Tree largeTree = ptools.parseNewickExpression(largeTreeNewick);
        //get quartets file
        std::ifstream quartetsFile(quartetsFilename, std::ifstream::binary);
        if (!quartetsFile.is_open())
        {
            printf("computeCompatibilityMetricIQtoQ failed -- file: %s not found/opened\n", quartetsFilename.c_str());
            exit(0);
        }
        std::string line;
    	
        int totalBipartitionDistance = 0;
        int count = 0;
        //for all quartet tree combos...
    	while(std::getline(quartetsFile, line))
    	{
    		//get next directly-genned quartet tree
                        //if line is blank skip
            if (line.size() > 2)
            {
                //printf("newick_line: %s\n", line.c_str());

                Tree tree = ptools.parseNewickExpression(line);
                //tree.outputAdjacentList();
                //# A tree is a quartet iff it contains six vertices.
                if (tree.v() == 6)//GUARD:: unnecessary?
                {
                    //if read line, 'tree' is a proper quartet --> get RF distance (largeTree <--> tree)
                	//get leaf list
                    Sequences sequences = tree.leafNameList();
					//create homeomorphic
                    Tree subtree = ptools.createHomeoSubtree(largeTree, sequences);

                	//get bipartitions
                    const PhyloTools::BipartitionList inducedQuartetBipartitionList = ptools.listBipartitions(subtree);
                    const PhyloTools::BipartitionList quartetBipartitionList2 = ptools.listBipartitions(tree);
                    const int differenceCount = ptools.bipartitionDistance(inducedQuartetBipartitionList, quartetBipartitionList2);
                    printf("The Difference Count is: %d\n", differenceCount);

                    totalBipartitionDistance += differenceCount;
                    count++;
                }
                else
                {
                    printf("Quartet file: %s contained improper quartet, does not have 6 vertices\n", quartetsFilename.c_str());
                    exit(0);
                }

            }
    	}
		quartetsFile.close();

        //get average of bipartition distance
        const float avgDifferenceCount = static_cast<float>(totalBipartitionDistance) / static_cast<float>(count);
    	
        //WORKING WITH QUARTETS ONLY
        const int sequenceCount = 4;
        return computeRFRate(avgDifferenceCount, sequenceCount);
    }

    //computes a total (sum of) quartet RF distance -- between (IQ) large tree, induced quartets and (Q) direct quartets
    //averages the distance --> gets RF rate
        //(alternate:: sum of RF rates / total quartets)
    float computeCompatibilityMetric_IQtoIQ(const std::string& largeTreeFilename1, const std::string& largeTreeFilename2)
    {
        PhyloTools ptools = PhyloTools();

        Tree largeListTree1 = ptools.parseNewickExpression(ptools.getLargeTreeLine(largeTreeFilename1));
        Tree largeListTree2 = ptools.parseNewickExpression(ptools.getLargeTreeLine(largeTreeFilename2));

        //lists of all possible combinations of 4 of leafs
        std::vector<std::vector<std::string> > largeQuartetSequenceList;
        int largeListSequenceCount = ptools.comboFourOf(largeListTree1, largeQuartetSequenceList);
        AZUL_UNUSED_VAR(largeListSequenceCount);
        //assumes both trees are on same sequence set
        //std::vector<const std::vector<std::string> > largeQuartetSequenceList2(largeQuartetSequenceList1);

        int totalBipartitionDistance = 0;
        int count = 0;
        for (std::vector<std::vector<std::string> >::const_iterator it = largeQuartetSequenceList.begin(); it != largeQuartetSequenceList.end(); it++)
        {
            //printf("quartet: %s, %s, %s, %s\n",(*it).at(0).c_str(),(*it).at(1).c_str(),(*it).at(2).c_str(),(*it).at(3).c_str());

            Tree subtree1 = ptools.createHomeoSubtree(largeListTree1, *it);
            Tree subtree2 = ptools.createHomeoSubtree(largeListTree2, *it);
            //get bipartitions
            const PhyloTools::BipartitionList inducedQuartetBipartitionList = ptools.listBipartitions(subtree1);
            const PhyloTools::BipartitionList quartetBipartitionList = ptools.listBipartitions(subtree2);
            const int differenceCount = ptools.bipartitionDistance(inducedQuartetBipartitionList, quartetBipartitionList);
            printf("The Difference Count is: %d\n", differenceCount);

            totalBipartitionDistance += differenceCount;
            count++;
        }
        //get average of bipartition distance
        const float avgDifferenceCount = static_cast<float>(totalBipartitionDistance) / static_cast<float>(count);

        //WORKING WITH QUARTETS ONLY
        const int sequenceCount = 4;
        return computeRFRate(avgDifferenceCount, sequenceCount);
    }

    //computes a total (sum of) quartet RF distance -- between (IQ) large tree, induced quartets and (Q) direct quartets
    //averages the distance --> gets RF rate
        //(alternate:: sum of RF rates / total quartets)
    float computeCompatibilityMetric_QtoQ(const std::string& quartetsFilename1, const std::string& quartetsFilename2)
    {
        PhyloTools ptools = PhyloTools();

        //get quartets file
        std::ifstream quartetsFile1(quartetsFilename1, std::ifstream::binary);
        std::ifstream quartetsFile2(quartetsFilename2, std::ifstream::binary);
    	if (!quartetsFile1.is_open() || !quartetsFile2.is_open())
        {
            printf("computeCompatibilityMetricQtoQ failed -- file: %s || %s not found/opened\n", quartetsFilename1.c_str(), quartetsFilename2.c_str());
            exit(0);
        }
        std::string line1;
        std::string line2;
    	
        int totalBipartitionDistance = 0;
        int count = 0;
        //for all quartet tree combos...
        while (std::getline(quartetsFile1, line1) && std::getline(quartetsFile2, line2))
        {
            //get next directly-genned quartet tree
                        //if line is blank skip
            if (line1.size() > 2 && line2.size() > 2)
            {
                Tree tree1 = ptools.parseNewickExpression(line1);
                Tree tree2 = ptools.parseNewickExpression(line2);

                //tree.outputAdjacentList();
                //# A tree is a quartet iff it contains six vertices.
                if (tree1.v() == 6 && tree2.v() == 6)//GUARD:: unnecessary?
                {
                    //if read line, 'tree1' && 'tree2' are proper quarteta --> get RF distance (quartet <--> quartet)
                    //NOTE:: ASSUMES QUARTETS FILES ARE BUILT IN SAME ORDERING...
						//ADD GUARD??? --> unecessary? guranteed same ordering if files created by PhyloTools...?
                    //get bipartitions
                    const PhyloTools::BipartitionList quartetBipartitionList1 = ptools.listBipartitions(tree1);
                    const PhyloTools::BipartitionList quartetBipartitionList2 = ptools.listBipartitions(tree2);
                    const int differenceCount = ptools.bipartitionDistance(quartetBipartitionList1, quartetBipartitionList2);
                    printf("The Difference Count is: %d\n", differenceCount);

                    totalBipartitionDistance += differenceCount;
                    count++;
                }
                else
                {
                    printf("Quartet files: %s || %s contained improper quartet, does not have 6 vertices\n", quartetsFilename1.c_str(), quartetsFilename2.c_str());
                    exit(0);
                }

            }
        }
        quartetsFile1.close();
        quartetsFile2.close();

    	
        //get average of bipartition distance
        const float avgDifferenceCount = static_cast<float>(totalBipartitionDistance) / static_cast<float>(count);

        //WORKING WITH QUARTETS ONLY
        const int sequenceCount = 4;
        return computeRFRate(avgDifferenceCount, sequenceCount);
    }

}