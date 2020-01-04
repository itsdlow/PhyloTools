//
//  Tree.hpp
//  ForestTools
//
//  Created by DeAngelo Deshawn Wilson on 8/20/19.
//  Copyright © 2019 DeAngelo Deshawn Wilson. All rights reserved.
//

#ifndef Tree_hpp
#define Tree_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <utility>

namespace phylo {

    //vector of vectors
    //typedef std::pair<std::string, std::vector<std::string>> adjPair;
    //multimap (simulating) vector 
    typedef std::pair<std::string, std::string> adjPair;

    class AdjPairKeyCompare
    {
    public:
        bool operator()(const adjPair& lhs, const adjPair& rhs) const
        {
            return keyLess(lhs.first, rhs.first);
        }
        bool operator()(const adjPair& lhs, const adjPair::first_type& k) const
        {
            return keyLess(lhs.first, k);
        }
        bool operator()(const adjPair::first_type& k, const adjPair& rhs) const
        {
            return keyLess(k, rhs.first);
        }


    private:
        bool keyLess(const adjPair::first_type& k1, const adjPair::first_type& k2) const
        {
            return k1 < k2;
        }
    };
    class AdjPairCompare
    {
    public:
        bool operator()(const adjPair& lhs, const adjPair& rhs) const
        {
            return pairLess(lhs, rhs);
        }
        /*
        bool operator()(const adjPair& lhs, const adjPair::first_type& k) const
        {
            return keyLess(lhs.first, k);
        }
        bool operator()(const adjPair::first_type& k, const adjPair& rhs) const
        {
            return keyLess(k, rhs.first);
        }
        */

    private:
        bool pairLess(const adjPair& k1, const adjPair& k2) const
        {
            if (k1.first == k2.first)
            {
                return k1.second < k2.second;
            }
            return k1.first < k2.first;
        }
    };




    class Tree
    {
    public:
        //BIG 4
        Tree();
        Tree(const Tree& t);
        //Tree& operator=(const Tree& rhs);
        //pass by value for compiler optimization???
        Tree& operator=(Tree rhs);
        ~Tree();

        void swap(Tree& rhs);
        //returns a vector of all vertices in sorted order
        const std::vector<std::string> vertices() const;

        //returns count of vertices
        int v() const;
        //returns number of edges in tree
        int e() const;

        //strings can be up to 64 chars long --> (FastME PHYLIB limit)
            //iterates through adjList --> if vertex not in result vect -> pushback (sort)
        //finds all leafs (degree == 1) of tree
        const std::vector<std::string> leafNameList() const;
        //finds all leafs reachable from vertex
        //make leaf name list const????
        const std::vector<std::string> leafNameList(const std::string& vertex) const;

        //TODO
        //converts the "Tree" to a newick expression --> return string-Tree
        std::string to_newick() const;
        //std::string to_s() const;

        //returns a new (copy) including all edges of "vertex"
            //must loop iterate through adjList until no more "vertex" entries --> pushback all entries to result vect
        const std::vector<std::string> getAdjacents(const std::string& vertex) const;
        int getDegree(const std::string&) const;
        //int degree(std::vector<std::string>& adjacents) const;

        //return a pair of strings with names of siblings, or empty
        const std::pair<const std::string, const std::string> findSiblings() const;

        //iteraetes through adjList, returns true if edge is part of a "vertex" pair-entry
        bool isAdjacent(const std::string& vertex, const std::string& edge) const;
        //returns true if "v" degree == 1
        bool isLeaf(const std::string& v) const;
        bool isSiblings(const std::string& v, const std::string& w) const;

        void outputAdjacentList() const;

        /*NON-Const methods*/
        void addEdge(const std::string& v, const std::string& e);
        void addVertex(const std::string& v);
        void deleteEdge(const std::string& v, const std::string& e);
        void deleteVertex(const std::string& v);

        //tree sorted in parseNewickExpression -- tree adjList assumed sorted
        void sortAdjacenyList();

        //PUBLIC MEMBERs OF CLASS
        class Counter
        {
            int count = 0;
        public:
            //void inc(){count++;};
            void reset() { count = 0; };
            int getCount() { return count++; };
        }counter;

    private:
        //PRIVATE MEMBERS (ADJACENCY LIST)-------------------------------------------------

        //a pair <K, V> which holds the string of a Vertex (K) and a string of a edge (V)
        // reserve space based on system parameters ???
        //a vector simulating a multimap --> adjancecy list
        std::vector<adjPair> adjacencyList;

        //COULD ALSO BE DONE WITH STATIC 2D array --> perform sequence count CHOOSE 4 --> get dimesions
        //idea!
        //map<sequenceName (std::String) , TreeNode* (singly-linked-list)>



        //recursive helper functions

        //return vector of all vertex degrees (sorted)
        std::vector<int> degrees() const;

        void leafNameListHelper(const std::string& v, std::vector<std::string>& leaves, std::vector<std::string>& visited) const;

        std::string& to_newickHelper(const std::string& v, std::string& res, std::vector<std::string>& visited) const;

        //used in deleteEdge() --> helper to f
        void deleteEdgeHelper(const std::string& v, const std::string& e);










    };

}



#endif /* Tree_hpp */
