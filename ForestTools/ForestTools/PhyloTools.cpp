//
//  PhyloTree.cpp
//  ForestTools
//
//  Created by DeAngelo Deshawn Wilson on 8/21/19.
//  Copyright © 2019 DeAngelo Deshawn Wilson. All rights reserved.
//

#include "PhyloTools.h"
#include <memory>
#include <regex>


namespace phylo {

    PhyloTools::PhyloTools()
    {

    }


    PhyloTools::treeIndex PhyloTools::buildQuartetsPairingIndex(const std::string& quartetTreesFilename) const
    {
        treeIndex ti;

        std::ifstream f1(quartetTreesFilename, std::ifstream::binary);
        //f1.open("QuartetTreesMrBayes.newick", std::ifstream::binary);

        if (!f1.is_open())
        {
            printf("file not found/opened\n");
            return ti;
        }

        std::string line;
        //std::getline(f1, line);
        //loop through each line of files
        while (std::getline(f1, line))
        {
            //printf("line: %s\n", line.c_str());
            /*
             # Skip comment lines --> COMMENT LINES?
             if line =~ /^#/ then next end
            */
            //if line is blank skip
            if (line.size() > 2)
            {
                //printf("newick_line: %s\n", line.c_str());

                Tree tree = this->parseNewickExpression(line);
                //tree.outputAdjacentList();
                //# A tree is a quartet iff it contains six vertices.
                if (tree.v() == 6)
                {
                    //printf("BuildQuartetPair:: tree is quartet\n");
                    //sorted in leafNameList()
                    Sequences sequences = tree.leafNameList();

                    //quartetPair pair = this->quartetPairDivision(tree);
                    ti.insert(std::make_pair(sequences, this->quartetPairDivision(tree)));
                    //ti[sequences] = this->quartetPairDivision(tree);
                    //printf("BuildQuartetPair:: inserted seuqence,quartet pair\n");
                }

            }
        }
        f1.close();
        return ti;
    }

    PhyloTools::quartetPair PhyloTools::quartetPairDivision(const Tree& tree) const
    {
        Sequences sequences = tree.leafNameList();
        if (sequences.size() != 4)
        {
            printf("quartetPairDivision:: tree not quartet FAILED\n");
            return quartetPair();
        }
        else
        {

            if (tree.isSiblings(sequences.at(0), sequences.at(1)))
            {
                return quartetPair(std::make_pair(sequences.at(0), sequences.at(1)), std::make_pair(sequences.at(2), sequences.at(3)));//std::make_pair(std::make_pair(sequences.at(0), sequences.at(1)), std::make_pair(sequences.at(2), sequences.at(3)));
            }
            else if (tree.isSiblings(sequences.at(0), sequences.at(2)))
            {
                return quartetPair(std::make_pair(sequences.at(0), sequences.at(2)), std::make_pair(sequences.at(1), sequences.at(3)));
            }
            else
            {
                return quartetPair(std::make_pair(sequences.at(0), sequences.at(3)), std::make_pair(sequences.at(1), sequences.at(2)));;
            }
        }
    }

    Tree PhyloTools::createHomeoSubtree(const Tree& tree, const std::vector<std::string>& retainedLabels) const
    {
        //printf("quartet: %s, %s, %s, %s\n",retainedLabels.at(0).c_str(),retainedLabels.at(1).c_str(),retainedLabels.at(2).c_str(),retainedLabels.at(3).c_str());

        Tree inducedTree = Tree(tree);
        bool finished = false;
        //const std::vector<const std::string> vertices = inducedTree.vertices();
            //**********       const-element containers (allocators) --> Illegal in VS 2015...        **************

        while (!finished)
        {
            finished = true;
            const std::vector<std::string> vertices = inducedTree.vertices();
            for (std::vector<std::string>::const_iterator it = vertices.begin(); it != vertices.end(); it++)
            {
                if (inducedTree.isLeaf(*it))
                {
                    //if current vertex is NOT in the desired quartet
                    if (!std::binary_search(retainedLabels.begin(), retainedLabels.end(), *it))
                    {
                        finished = false;
                        inducedTree.deleteVertex(*it);
                    }
                }
                else if (inducedTree.getDegree(*it) == 2)
                {
                    const std::vector<std::string> adjacents = inducedTree.getAdjacents(*it);
                    /*test*******************
                    if(adjacents.size() > 2)
                    {
                        inducedTree.outputAdjacentList();
                        printf("createHomeoSubtree:: ERROR degree!=2\n");
                    }
                    */
                    const std::string w = adjacents.at(0);
                    const std::string u = adjacents.at(1);
                    inducedTree.deleteVertex(*it);
                    //does adjList need to be resorted?????
                    inducedTree.addEdge(w, u);
                    inducedTree.sortAdjacenyList();
                    finished = false;
                }
            }
        }
        /*
            if (inducedTree.e() != 5)
            {
                printf("createHomeoSubtree:: induced tree has %d edges.\n", inducedTree.e());
                inducedTree.outputAdjacentList();
                return Tree();
            }
        */
        return inducedTree;
    }

    int PhyloTools::comboFourOf(const Tree& largeListTree, std::vector<std::vector<std::string> >& largeQuartetSequenceList) const
    {
        //list of all possible combinations of 4 of leafs
        std::vector<std::string> leaves = largeListTree.leafNameList();
        int largeListSequenceCount = static_cast<int>(leaves.size());
        //produce vectors of all combinations of 4 on LargeTreeSequences
        for (int i = 0; i < largeListSequenceCount; i++)
        {
            for (int j = i + 1; j < largeListSequenceCount; j++)
            {
                for (int k = j + 1; k < largeListSequenceCount; k++)
                {
                    for (int l = k + 1; l < largeListSequenceCount; l++)
                    {
                        std::vector<std::string> quartet;
                        quartet.push_back(leaves.at(i));
                        quartet.push_back(leaves.at(j));
                        quartet.push_back(leaves.at(k));
                        quartet.push_back(leaves.at(l));
                        std::sort(quartet.begin(), quartet.end());
                        largeQuartetSequenceList.push_back(quartet);
                    }
                }
            }
        }
        return largeListSequenceCount;
    }
    //newick epxressions must be termianted with semi-colon
    std::string PhyloTools::cleanNewick(const std::string& e) const
    {
        //  # Remove branch lengths (i.e., :digits after sequence name)

        std::regex reg(":[0-9]+\\.?[0-9]*");

        std::string c = std::regex_replace(e, reg, "");
        //c.erase(std::remove(c.begin(), c.end(), '\n'), c.end());
        //printf("regex:: %s + back: %c - %c\n",c.c_str(), c.c_str()[c.size()-1], c.c_str()[c.size()-2]);

        //assume trailing semi-colon, remove last char
        c.pop_back();

        //ensure semi colon removed ** (mrbayes extra white space last char - error)
        if (c.back() == ';')
        {
            c.pop_back();
        }
        //  # Remove trailing semicolon, if present

        //printf("clean_newick: %s\n",c.c_str());

        return c;
    }


    Tree PhyloTools::parseNewickExpression(const std::string& e) const
    {
        //clean newick expression "e"
        std::string cleanNewickExpression = this->cleanNewick(e);

        Tree tree = Tree();
        std::string rootVertex = parseNewickHelper(cleanNewickExpression, tree);

        tree.sortAdjacenyList();

        printf("parseNewickExpression:: Tree parsed\n");
        return tree;
    }

    std::string PhyloTools::parseNewickHelper(const std::string& e, Tree& tree) const
    {
        //printf("e: %s\n", e.c_str());
        //if the first character of e is not a "("
        if (e.c_str()[0] != '(')
        {
            //printf("parseNewickHelper:: adding vertex __ %s\n", e.c_str());
            tree.addVertex(e);
            return e;
        }

        std::string internalVertex = "I" + std::to_string(tree.counter.getCount());
        //std::snprintf(output, size, "I%d")
        //printf("%s\n", internalVertex.c_str());
        tree.addVertex(internalVertex);
        //printf("e:: %s\n",e.c_str());

        // "-2" take to 2nd-to-last char - excluding first and last parenthesis
        std::string eNoParens = e.substr(1, e.size() - 2);
        //printf("noParens:: %s\n",eNoParens.c_str());
        //tree.
        int start = 0;
        int depth = 0;
        std::vector<std::string> subexpression;
        //loop through newick string
        for (unsigned int i = 0; i < eNoParens.size() - 1; i++)
        {
            if (eNoParens.c_str()[i] == '(')
            {
                depth++;
            }
            else if (eNoParens.c_str()[i] == ')')
            {
                depth--;
            }
            else if (eNoParens.c_str()[i] == ',' && depth == 0)
            {
                //dont do (i-1) because i is index, (implicit " - 1" --> need total length of substring
                int subExpressionSize = i - start;
                //printf("substring: %s\n", eNoParens.substr(start, subExpressionSize).c_str());
                subexpression.push_back(eNoParens.substr(start, subExpressionSize));
                start = i + 1;
            }
        }
        //add last vertex to subexpression (after last comma)
        //[0,1,2,3,4,5,6,7,8,9,10,11,12,13] --> 14 - 5 = 9
        int subExpressionSize = static_cast<int>(eNoParens.size()) - start;
        subexpression.push_back(eNoParens.substr(start, subExpressionSize));
        //printf("last expression ::  %s\n", subexpression.back().c_str());

        for (std::vector<std::string>::iterator it = subexpression.begin(); it != subexpression.end(); it++)
        {
            tree.addEdge(internalVertex, parseNewickHelper(*it, tree));
        }

        return internalVertex;
    }




    //symmetric difference
    const PhyloTools::BipartitionList PhyloTools::listBipartitions(Tree tree) const
    {
        BipartitionList bipartitionList;
        const std::vector<std::string> vertices = tree.vertices();

        for (std::vector<std::string>::const_iterator i = vertices.begin(); i != vertices.end(); i++)
        {
            for (std::vector<std::string>::const_iterator j = i + 1; j != vertices.end(); j++)
            {
                //printf("i = %s , j = %s\n", (*i).c_str(), (*j).c_str());

                /*
                 BREAK -- ENDS iteration of for loop (DOES NOT WORK LIKE NEXT)

                 //Filter out edges that touch leaves*
                 if (tree.isLeaf(*i) || tree.isLeaf(*j)){break;}
                 //skip if vertex's are not adjacent????
                 if(!tree.isAdjacent(*i, *j)){break;}
                */
                if (!tree.isLeaf(*i) && !tree.isLeaf(*j) && tree.isAdjacent(*i, *j))
                {
                    tree.deleteEdge(*i, *j);
                    std::vector<std::string> iLeaves = tree.leafNameList(*i);
                    //printf("iLeaves:: %s\n", to_s(iLeaves).c_str());
                    std::vector<std::string> jLeaves = tree.leafNameList(*j);
                    //printf("iLeaves#%lu -- jLeaves#%lu\n", iLeaves.size(), jLeaves.size());

                    //sort Bipartition-pairing
                    if (iLeaves < jLeaves)
                    {
                        bipartitionList.push_back(std::make_pair(iLeaves, jLeaves));
                    }
                    else
                    {
                        bipartitionList.push_back(std::make_pair(jLeaves, iLeaves));
                    }
                    tree.addEdge(*i, *j);
                    tree.sortAdjacenyList();
                }

            }
        }

        std::sort(bipartitionList.begin(), bipartitionList.end());
        return bipartitionList;
    }

    std::string PhyloTools::to_s(const std::vector<std::string>& v) const
    {
        std::string s;
        s.append("[");
        for (auto it = v.begin(); it != v.end(); it++)
        {
            s.append(*it);
            s.append(", ");
        }
        s.append("]");
        return s;
    }

    int PhyloTools::symmetricDifference(const BipartitionList& tree1BipartitionList, const BipartitionList& tree2BipartitionList) const
    {


        /*
        int sequenceCount = 16;
        int max_sym_diff = (2 * sequenceCount) - 6;//for size 16 tree STATIC!!! (2n - 6)

        //vector which holds all Different (unique) Bipartition-pairs
        BipartitionList uniqueBipartitions(max_sym_diff);
        BipartitionList::iterator end = std::set_symmetric_difference(bi1.begin(), bi1.end(), bi2.begin(), bi2.end(), uniqueBipartitions.begin());
        uniqueBipartitions.resize(end-uniqueBipartitions.begin());
        */
        //printf("The symmetric difference has %lu elements:\n", v.size());
        //return static_cast<int>(uniqueBipartitions.size());

        int diffCount = 0;


        for (BipartitionList::const_iterator it = tree1BipartitionList.begin(); it != tree1BipartitionList.end(); it++)
        {
            //t1 bipartition not in t2-BipartitionList
            if (!std::binary_search(tree2BipartitionList.begin(), tree2BipartitionList.end(), *it))
            {
                diffCount++;
            }

        }
        for (BipartitionList::const_iterator it = tree2BipartitionList.begin(); it != tree2BipartitionList.end(); it++)
        {
            if (!std::binary_search(tree1BipartitionList.begin(), tree1BipartitionList.end(), *it))
            {
                diffCount++;
            }

        }

        return diffCount;
    }

    std::string PhyloTools::getLargeTreeLine(const std::string& largeTreeFilename) const
    {
        //constuct file ifstream
        std::ifstream f1(largeTreeFilename, std::ifstream::binary);
        //f1.open("LargeTreeMrBayes.newick", std::ifstream::binary);

        if (!f1.is_open())
        {
            printf("file not found/opened\n");
            return std::string();
        }

        std::string line;
        //large tree file must be on 1st line of textfile
        std::getline(f1, line);
        f1.close();

        return line;


    }


}


