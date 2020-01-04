//
//  PhyloTree.hpp
//  ForestTools
//
//  Created by DeAngelo Deshawn Wilson on 8/21/19.
//  Copyright © 2019 DeAngelo Deshawn Wilson. All rights reserved.
//

#ifndef PhyloTree_hpp
#define PhyloTree_hpp

#include <stdio.h>
#include "Tree.h"
#include <unordered_map>
#include <fstream>




namespace phylo {



    //key value in treeIndex
    typedef const std::vector<std::string> Sequences;

    // custom hash can be a standalone function object:
    struct SequenceHash
    {
        //to work on non-specific c++ compiler "remove no except specifier" -*********
        std::size_t operator()(Sequences const& k) const
        {

            size_t tmp = std::hash<std::string>()(*(k.begin()));
            for (phylo::Sequences::const_iterator it = ++k.begin(); it != k.end(); it++)
            {
                size_t h = std::hash<std::string>()(*it);
                tmp ^= (h << 1);
            }
            return tmp;
        }
    };


    //class PhyloTree
    class PhyloTools
    {
    public:
        //Big 4
        PhyloTools();

        //public phylogenetic tree helper functions (Tools)

        //a type (pair of pairs) to define the split between 4 sequences (Quartet)
        //typedef std::pair<std::pair<std::string, std::string>, std::pair<std::string,std::string>> quartetPair;
        struct quartetPair
        {
            std::pair<std::string, std::string> pair1;
            std::pair<std::string, std::string> pair2;

            quartetPair()
            {
            }
            quartetPair(std::pair<std::string, std::string> p1, std::pair<std::string, std::string> p2) :
                pair1(p1), pair2(p2)
            {
            }
            bool operator==(const quartetPair& other)
            {
                if (this->pair1 == other.pair1 && this->pair2 == other.pair2)
                {
                    return true;
                }
                return false;
            }
        };


        //typedef std::vector<std::pair<std::vector<std::string>, quartetPair>> treeIndex;
            //sorted sequence names + quartetPair division
        typedef std::unordered_map<Sequences, quartetPair, SequenceHash> treeIndex;
        //must keep vectors (KEY) sorted!!!! otherwise equal comparison will fail??

        typedef std::pair<std::vector<std::string>, std::vector<std::string> > Bipartition;
        typedef std::vector<Bipartition> BipartitionList;

        std::string getLargeTreeLine(const std::string& largeTreeFilename) const;


        Tree parseNewickExpression(const std::string& e) const;

        treeIndex buildQuartetsPairingIndex(const std::string& quartetTreesFilename) const;
        quartetPair quartetPairDivision(const Tree& tree) const; //deletion of trees handled here "delete &tree"
        Tree createHomeoSubtree(const Tree& tree, const std::vector<std::string>& retainedLabels) const;
        //returns Sequence count, calling leaves.size()
        int comboFourOf(const Tree& largeListTree, std::vector<std::vector<std::string> >& largeQuartetSequenceList) const;

        //symmetric difference
        const BipartitionList listBipartitions(Tree tree) const;
        int symmetricDifference(const BipartitionList& bi1, const BipartitionList& bi2) const;

    private:
        std::string cleanNewick(const std::string& e) const;
        std::string parseNewickHelper(const std::string& e, Tree& tree) const;

        //converts vector to list_string
        std::string to_s(const std::vector<std::string>& v) const;

    };

}


#endif /* PhyloTree_hpp */
